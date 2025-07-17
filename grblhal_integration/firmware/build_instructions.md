# grblHAL Robot Arm - Build Instructions

This document provides complete instructions for building the minimal grblHAL firmware for robot arm control on both host systems (for testing) and the Raspberry Pi Pico 2 (RP2350) target.

## Overview

Our minimal grblHAL implementation provides two build systems:

1. **Host Build (Makefile)** - For testing grblHAL core functionality
2. **RP2350 Build (CMake + Pico SDK)** - For actual robot arm hardware

## Prerequisites

### For Host Build (Testing)
```bash
# Required tools
gcc
make

# On Ubuntu/Debian
sudo apt update
sudo apt install build-essential

# On macOS
xcode-select --install
```

### For RP2350 Build (Target Hardware)
```bash
# Required tools
cmake (>= 3.13)
gcc-arm-none-eabi
git

# On Ubuntu/Debian
sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential git

# On macOS
brew install cmake gcc-arm-embedded git
```

## Build System 1: Host Testing (Makefile)

Use this for testing grblHAL core compilation and functionality without hardware.

### Quick Start
```bash
cd grblhal_integration/firmware

# Test compilation (expects driver errors - that's normal)
make

# View build information
make info

# Clean build artifacts
make clean
```

### What This Tests
✅ **grblHAL Core Compilation**: All 36 core files compile successfully  
✅ **Dependency Resolution**: All headers and libraries link correctly  
✅ **Configuration Validation**: Robot arm config applied correctly  
❌ **Driver Compilation**: Expected to fail (needs Pico SDK)  

### Expected Output
```
grblHAL core files: 36
Driver files: 7  
Total files: 43

✅ grblHAL core compiles successfully (warnings normal)
❌ Driver fails: needs Pico SDK (expected)
```

## Build System 2: RP2350 Target (CMake)

Use this for building actual firmware for Raspberry Pi Pico 2 hardware.

### Step 1: Install Pico SDK

```bash
# Create development directory
mkdir -p ~/pico
cd ~/pico

# Clone Pico SDK
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init

# Set environment variable (add to ~/.bashrc for permanent)
export PICO_SDK_PATH=~/pico/pico-sdk
```

### Step 2: Build for RP2350

```bash
cd grblhal_integration/firmware

# Create build directory
mkdir build
cd build

# Configure for RP2350 (Pico 2)
cmake -DPICO_BOARD=pico2 ..

# Build firmware
make -j4

# View size information
make size
```

### Step 3: Flash to Pico 2

```bash
# Install picotool (if not installed)
cd ~/pico
git clone https://github.com/raspberrypi/picotool.git
cd picotool
mkdir build && cd build
cmake ..
make -j4
sudo make install

# Flash firmware to Pico 2
# 1. Hold BOOTSEL while connecting Pico 2 to USB
# 2. Release BOOTSEL - Pico appears as USB drive
make flash

# Or manually copy UF2 file
cp grblhal_robot_arm.uf2 /media/RPI-RP2/
```

## Build Outputs

### Host Build (Makefile)
- `grblhal_robot_arm` - Host executable (testing only)
- `*.o` - Object files for analysis

### RP2350 Build (CMake)  
- `grblhal_robot_arm.elf` - ELF firmware file
- `grblhal_robot_arm.uf2` - UF2 file for flashing  
- `grblhal_robot_arm.bin` - Raw binary firmware
- `grblhal_robot_arm.hex` - Intel hex firmware
- `grblhal_robot_arm.map` - Memory map for debugging

## Configuration

### Robot Arm Settings
Configuration is automatically applied through:
- `grblhal_config.h` - Main robot arm hardware config
- `grbl/robot_arm_config.h` - grblHAL core overrides  
- `my_machine.h` - RP2350 pin assignments

### Key Features Enabled
- 6-axis control (5 steppers + 1 servo)
- Robot arm motion planning
- Safety systems (limits, e-stop)
- Real-time status reporting
- USB/UART communication

### Key Features Disabled  
- Laser control
- Plasma cutting
- Automatic tool changes
- CNC machining features
- Network interfaces

## Troubleshooting

### Common Issues

**"malloc.h not found"**
```bash
# Solution: Use stdlib.h instead (already fixed in our build)
```

**"pico/time.h not found"**
```bash
# Solution: Use CMake build with Pico SDK for RP2350 target
export PICO_SDK_PATH=/path/to/pico-sdk
```

**"N_AXIS redefinition"**
```bash
# Solution: Robot arm config properly overrides (already handled)
```

### Memory Usage Verification
```bash
# Check firmware size fits RP2350
cd build
arm-none-eabi-size grblhal_robot_arm.elf

# Expected output should show:
#   text    data     bss     dec     hex
#  <200KB   <8KB   <32KB   <240KB  (fits in 512KB flash + 264KB RAM)
```

### Serial Communication Test
```bash
# Connect via USB (adjust device as needed)
minicom -D /dev/ttyACM0 -b 115200

# Test grblHAL communication
> $$ 
[Shows grblHAL settings]

> ?
<Idle|MPos:0.000,0.000,0.000,0.000,0.000,0.000|FS:0,0>
```

## Development Workflow

### Recommended Process
1. **Host Testing**: Use Makefile to verify core compilation
2. **Configuration**: Adjust `grblhal_config.h` for hardware setup  
3. **RP2350 Build**: Use CMake to build for target hardware
4. **Hardware Test**: Flash and test with actual robot arm
5. **ROS2 Integration**: Connect via UART to Raspberry Pi

### Quick Development Cycle
```bash
# Make changes to source files
vim grblhal_config.h

# Test host compilation
make clean && make

# Build for RP2350
cd build && make -j4

# Flash and test
make flash && make monitor
```

## Integration with ROS2

The compiled firmware provides a grblHAL-compatible interface for ROS2 integration:

- **Communication**: USB CDC or UART at 115200 baud
- **Protocol**: Standard grblHAL G-code commands
- **Real-time**: 4Hz status reports for ROS2 feedback
- **Safety**: Hardware limits and emergency stops

See the ROS2 workspace documentation for bridge implementation details.

## Success Criteria

✅ **Host Build Success**: grblHAL core compiles with warnings only  
✅ **RP2350 Build Success**: Firmware builds to UF2 file  
✅ **Size Verification**: Firmware fits in RP2350 memory  
✅ **Communication Test**: grblHAL responds to commands  
✅ **Robot Arm Ready**: 6-axis control functional  

This build system provides a complete path from source code to working robot arm controller firmware. 