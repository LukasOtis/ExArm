#!/usr/bin/env python3
"""
GRBL Command Format Test
Tests the exact command format expected by grblHAL
"""

import serial
import time

def test_grbl_commands():
    """Test GRBL command formats and responses"""

    port = '/dev/cu.usbmodem11301'
    baud = 115200

    print("🔍 GRBL Command Format Test")
    print(f"📡 Port: {port}")
    print(f"⚙️  Baud: {baud}")
    print("=" * 50)

    try:
        with serial.Serial(port, baud, timeout=2) as ser:
            print("✅ Connected to grblHAL")

            # Test sequence with exact formatting
            test_commands = [
                # Basic commands that should work
                ("?", "Status query"),
                ("$$", "Settings query"),
                ("$H", "Help command"),

                # Custom M-codes that should work with our firmware
                ("M15", "Read limit switches"),
                ("M16", "Read safety inputs"),
                ("M12 P0", "Set servo to 0°"),
                ("M13", "Emergency stop"),

                # Standard G-codes that should work
                ("G0 X5 F100", "Move X axis"),
                ("G0 X0", "Return to origin"),
            ]

            for command, description in test_commands:
                print(f"\n📤 Testing: {description}")
                print(f"   Command: '{command}'")
                print(f"   Hex: {command.encode().hex()}")
                print(f"   Length: {len(command)} bytes")

                # Send with proper line ending
                full_command = command + '\n'
                ser.write(full_command.encode())

                # Wait for response
                time.sleep(0.5)

                if ser.in_waiting > 0:
                    response = ser.read(ser.in_waiting).decode('utf-8', errors='ignore').strip()
                    if response:
                        print(f"   ✅ Response: '{response}'")
                    else:
                        print("   ⚠️  Empty response")
                else:
                    print("   ❌ No response")

                # Wait before next command
                time.sleep(0.2)

            print("\n🔧 Troubleshooting Results:")
            print("=" * 50)

            # Test with different line endings
            print("\n📋 Line Ending Tests:")
            for ending_name, ending in [("LF", "\n"), ("CRLF", "\r\n")]:
                test_cmd = "M15"
                full_cmd = test_cmd + ending
                print(f"Testing {ending_name}: '{test_cmd}' + {repr(ending)}")
                ser.write(full_cmd.encode())
                time.sleep(0.5)
                if ser.in_waiting > 0:
                    response = ser.read(ser.in_waiting).decode('utf-8', errors='ignore').strip()
                    print(f"   Response: '{response}'")

            # Test timing
            print("\n📋 Timing Tests:")
            for delay in [0.1, 0.5, 1.0]:
                print(f"Testing with {delay}s delay:")
                ser.write(b"M15\n")
                time.sleep(delay)
                if ser.in_waiting > 0:
                    response = ser.read(ser.in_waiting).decode('utf-8', errors='ignore').strip()
                    print(f"   Response: '{response}'")
                else:
                    print("   No response")

    except serial.SerialException as e:
        print(f"❌ Serial connection failed: {e}")
        print("\n🔧 Connection troubleshooting:")
        print("1. Check if Pico is connected: ls /dev/cu.usbmodem*")
        print("2. Verify Pico is in normal mode (not BOOTSEL)")
        print("3. Try: sudo chmod 666 /dev/cu.usbmodem11301")
    except Exception as e:
        print(f"❌ Error: {e}")

if __name__ == "__main__":
    test_grbl_commands()
