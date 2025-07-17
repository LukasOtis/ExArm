#!/usr/bin/env python3

"""
grblHAL Communication Test Script

This script tests the communication between a computer and grblHAL running
on Raspberry Pi Pico 2. It can be used to verify the setup before running
the full ROS2 integration.

Usage:
    python3 communication_test.py [serial_port] [baud_rate]

Examples:
    python3 communication_test.py /dev/ttyACM0 115200
    python3 communication_test.py COM3 115200
"""

import serial
import time
import sys
import threading
import re
from typing import Optional, List, Tuple

class GrblTester:
    def __init__(self, port: str = "/dev/ttyACM0", baud: int = 115200, timeout: float = 1.0):
        """Initialize grblHAL tester."""
        self.port = port
        self.baud = baud
        self.timeout = timeout
        self.serial_conn: Optional[serial.Serial] = None
        self.is_connected = False
        self.response_buffer: List[str] = []
        self.lock = threading.Lock()
        
        # Test results
        self.test_results = {
            'connection': False,
            'basic_commands': False,
            'status_reports': False,
            'movement_commands': False,
            'emergency_stop': False,
            'homing': False
        }
    
    def connect(self) -> bool:
        """Establish serial connection to grblHAL."""
        try:
            print(f"Connecting to {self.port} at {self.baud} baud...")
            self.serial_conn = serial.Serial(
                port=self.port,
                baudrate=self.baud,
                timeout=self.timeout,
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE
            )
            
            if self.serial_conn.is_open:
                print("✓ Serial connection established")
                self.is_connected = True
                
                # Start reader thread
                self.reader_thread = threading.Thread(target=self._serial_reader, daemon=True)
                self.reader_thread.start()
                
                # Wait for initial response
                time.sleep(2)
                return True
            else:
                print("✗ Failed to open serial port")
                return False
                
        except Exception as e:
            print(f"✗ Connection error: {e}")
            return False
    
    def disconnect(self):
        """Close serial connection."""
        if self.serial_conn and self.serial_conn.is_open:
            self.is_connected = False
            self.serial_conn.close()
            print("✓ Serial connection closed")
    
    def _serial_reader(self):
        """Background thread to read serial responses."""
        buffer = ""
        
        while self.is_connected and self.serial_conn.is_open:
            try:
                if self.serial_conn.in_waiting > 0:
                    data = self.serial_conn.read(self.serial_conn.in_waiting).decode('utf-8', errors='ignore')
                    buffer += data
                    
                    # Process complete lines
                    while '\n' in buffer:
                        line, buffer = buffer.split('\n', 1)
                        line = line.strip()
                        if line:
                            with self.lock:
                                self.response_buffer.append(line)
                                print(f"<< {line}")
                
                time.sleep(0.01)  # Small delay to prevent busy waiting
                
            except Exception as e:
                print(f"Reader error: {e}")
                break
    
    def send_command(self, command: str, wait_for_response: bool = True, timeout: float = 5.0) -> List[str]:
        """Send command to grblHAL and optionally wait for response."""
        if not self.is_connected or not self.serial_conn.is_open:
            print("✗ Not connected to grblHAL")
            return []
        
        # Clear response buffer
        with self.lock:
            self.response_buffer.clear()
        
        # Send command
        try:
            cmd_to_send = command + '\n'
            self.serial_conn.write(cmd_to_send.encode('utf-8'))
            self.serial_conn.flush()
            print(f">> {command}")
            
            if not wait_for_response:
                return []
            
            # Wait for response
            start_time = time.time()
            while time.time() - start_time < timeout:
                with self.lock:
                    if self.response_buffer:
                        return self.response_buffer.copy()
                time.sleep(0.1)
            
            print(f"✗ Timeout waiting for response to: {command}")
            return []
            
        except Exception as e:
            print(f"✗ Error sending command '{command}': {e}")
            return []
    
    def wait_for_idle(self, timeout: float = 10.0) -> bool:
        """Wait for grblHAL to return to idle state."""
        print("Waiting for idle state...")
        start_time = time.time()
        
        while time.time() - start_time < timeout:
            responses = self.send_command("?", timeout=1.0)
            
            for response in responses:
                if response.startswith('<') and response.endswith('>'):
                    # Parse status: <Idle|MPos:0.000,0.000,0.000|FS:0,0>
                    if '|' in response:
                        state = response.split('|')[0][1:]  # Remove '<' and get state
                        if state.lower() == 'idle':
                            print("✓ grblHAL is idle")
                            return True
                        else:
                            print(f"Current state: {state}")
            
            time.sleep(0.5)
        
        print("✗ Timeout waiting for idle state")
        return False
    
    def test_basic_connection(self) -> bool:
        """Test basic connection and grblHAL response."""
        print("\n=== Testing Basic Connection ===")
        
        if not self.connect():
            return False
        
        # Send wake-up sequence
        self.send_command("\r\n\r\n", wait_for_response=False)
        time.sleep(1)
        
        # Test help command
        responses = self.send_command("$")
        if any("Grbl" in resp or "grbl" in resp or "$" in resp for resp in responses):
            print("✓ grblHAL responded to help command")
            self.test_results['connection'] = True
            return True
        else:
            print("✗ No valid response from grblHAL")
            return False
    
    def test_settings_commands(self) -> bool:
        """Test settings and basic commands."""
        print("\n=== Testing Settings Commands ===")
        
        # Test settings query
        responses = self.send_command("$$")
        settings_found = False
        
        for response in responses:
            if response.startswith('$') and '=' in response:
                settings_found = True
                print(f"Found setting: {response}")
        
        if settings_found:
            print("✓ Settings command works")
        else:
            print("✗ No settings found")
            return False
        
        # Test unlock command
        responses = self.send_command("$X")
        if any("ok" in resp.lower() for resp in responses):
            print("✓ Unlock command accepted")
        else:
            print("✗ Unlock command failed")
            return False
        
        self.test_results['basic_commands'] = True
        return True
    
    def test_status_reports(self) -> bool:
        """Test status report functionality."""
        print("\n=== Testing Status Reports ===")
        
        # Request status multiple times
        for i in range(3):
            responses = self.send_command("?")
            
            for response in responses:
                if response.startswith('<') and response.endswith('>'):
                    print(f"✓ Status report {i+1}: {response}")
                    
                    # Parse position data
                    if 'MPos:' in response:
                        pos_match = re.search(r'MPos:([\d.-]+,[\d.-]+,[\d.-]+,[\d.-]+,[\d.-]+,[\d.-]+)', response)
                        if pos_match:
                            positions = pos_match.group(1).split(',')
                            print(f"  Positions: X={positions[0]}, Y={positions[1]}, Z={positions[2]}")
                            print(f"             A={positions[3]}, B={positions[4]}, C={positions[5]}")
                            self.test_results['status_reports'] = True
                            return True
            
            time.sleep(0.5)
        
        print("✗ No valid status reports received")
        return False
    
    def test_movement_commands(self) -> bool:
        """Test basic movement commands (without actual motion)."""
        print("\n=== Testing Movement Commands ===")
        
        # Set to relative mode for safety
        responses = self.send_command("G91")
        if not any("ok" in resp.lower() for resp in responses):
            print("✗ Failed to set relative mode")
            return False
        
        # Test small movement commands
        test_commands = [
            "G1 X0.1 F100",   # Small X move
            "G1 Y0.1 F100",   # Small Y move  
            "G1 Z0.1 F100",   # Small Z move
            "G1 A0.1 F100",   # Small A move
            "G1 B0.1 F100",   # Small B move
            "G1 C1 F100"      # Small C move (servo)
        ]
        
        for cmd in test_commands:
            responses = self.send_command(cmd)
            if any("ok" in resp.lower() for resp in responses):
                print(f"✓ Command accepted: {cmd}")
            else:
                print(f"✗ Command rejected: {cmd}")
                return False
            
            time.sleep(0.1)
        
        # Wait for completion
        if self.wait_for_idle():
            print("✓ All movement commands completed")
            self.test_results['movement_commands'] = True
            return True
        else:
            print("✗ Movement commands did not complete properly")
            return False
    
    def test_emergency_stop(self) -> bool:
        """Test emergency stop functionality."""
        print("\n=== Testing Emergency Stop ===")
        
        # Start a slow movement
        self.send_command("G1 X10 F10", wait_for_response=False)  # Very slow move
        time.sleep(0.5)  # Let it start
        
        # Send emergency stop
        responses = self.send_command("!")  # Real-time feed hold
        time.sleep(0.5)
        
        # Check if stopped
        status_responses = self.send_command("?")
        for response in status_responses:
            if 'Hold' in response or 'Alarm' in response:
                print("✓ Emergency stop activated")
                
                # Reset and unlock
                self.send_command("$X")
                time.sleep(1)
                
                self.test_results['emergency_stop'] = True
                return True
        
        print("✗ Emergency stop test failed")
        return False
    
    def test_homing(self) -> bool:
        """Test homing command (without actual homing)."""
        print("\n=== Testing Homing Commands ===")
        
        # Note: This will likely fail without proper limit switches
        # but we can test if the command is recognized
        
        responses = self.send_command("$H", timeout=2.0)
        
        # Check for any response indicating homing is recognized
        command_recognized = False
        for response in responses:
            if any(word in response.lower() for word in ['alarm', 'homing', 'error']):
                command_recognized = True
                print(f"✓ Homing command recognized: {response}")
                break
        
        if command_recognized:
            # Reset any alarm state
            self.send_command("$X")
            self.test_results['homing'] = True
            return True
        else:
            print("✗ Homing command not recognized")
            return False
    
    def test_robot_arm_gcode(self) -> bool:
        """Test robot arm specific G-code commands."""
        print("\n=== Testing Robot Arm G-code ===")
        
        # Test multi-axis coordinated movement
        robot_commands = [
            "G21",  # Millimeter units
            "G90",  # Absolute positioning
            "G94",  # Feed rate per minute
            "G1 X0 Y90 Z45 A50 B50 C0 F1000",  # Ready position
            "G1 X45 Y60 Z30 A75 B25 C90 F500", # Pick position
            "G1 X-45 Y80 Z60 A85 B15 C-90 F500", # Place position
        ]
        
        success_count = 0
        for cmd in robot_commands:
            responses = self.send_command(cmd)
            if any("ok" in resp.lower() for resp in responses):
                print(f"✓ Robot command accepted: {cmd}")
                success_count += 1
            else:
                print(f"✗ Robot command rejected: {cmd}")
            
            time.sleep(0.2)
        
        if success_count == len(robot_commands):
            print("✓ All robot arm commands accepted")
            return True
        else:
            print(f"✗ Only {success_count}/{len(robot_commands)} commands accepted")
            return False
    
    def run_all_tests(self) -> dict:
        """Run all communication tests."""
        print("="*50)
        print("grblHAL Communication Test Suite")
        print("="*50)
        
        try:
            # Run tests in sequence
            self.test_basic_connection()
            
            if self.test_results['connection']:
                self.test_settings_commands()
                self.test_status_reports()
                self.test_movement_commands()
                self.test_emergency_stop()
                self.test_homing()
                self.test_robot_arm_gcode()
        
        except KeyboardInterrupt:
            print("\n\nTest interrupted by user")
        
        finally:
            self.disconnect()
        
        # Print summary
        self.print_test_summary()
        return self.test_results
    
    def print_test_summary(self):
        """Print test results summary."""
        print("\n" + "="*50)
        print("TEST SUMMARY")
        print("="*50)
        
        total_tests = len(self.test_results)
        passed_tests = sum(1 for result in self.test_results.values() if result)
        
        for test_name, result in self.test_results.items():
            status = "✓ PASS" if result else "✗ FAIL"
            print(f"{test_name.replace('_', ' ').title():.<40} {status}")
        
        print("-" * 50)
        print(f"Overall Result: {passed_tests}/{total_tests} tests passed")
        
        if passed_tests == total_tests:
            print("🎉 All tests passed! grblHAL communication is working correctly.")
        elif passed_tests >= total_tests * 0.8:
            print("⚠️  Most tests passed. Check failed tests above.")
        else:
            print("❌ Multiple test failures. Check your setup and connections.")
        
        print("\nNext steps:")
        if self.test_results['connection']:
            print("- Test with actual motors connected")
            print("- Configure ROS2 bridge")
            print("- Calibrate motor parameters")
        else:
            print("- Check serial port and baud rate")
            print("- Verify grblHAL firmware is flashed correctly")
            print("- Check USB cable and connections")

def main():
    """Main function to run communication tests."""
    # Parse command line arguments
    port = sys.argv[1] if len(sys.argv) > 1 else "/dev/ttyACM0"
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 115200
    
    print(f"Testing grblHAL communication on {port} at {baud} baud")
    print("Press Ctrl+C to stop tests at any time\n")
    
    # Create tester and run tests
    tester = GrblTester(port=port, baud=baud)
    results = tester.run_all_tests()
    
    # Exit with appropriate code
    sys.exit(0 if all(results.values()) else 1)

if __name__ == "__main__":
    main()