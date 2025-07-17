# Building and Flashing grblHAL for Robot Arm

This guide shows how to build and flash grblHAL firmware for the 5-6 DOF robot arm project using Raspberry Pi Pico 2 (RP2350).

## Prerequisites

### Required Tools
- **Git** for cloning repositories
- **CMake** (version 3.13 or later)
- **ARM GCC toolchain** (version 10.3 or later)
- **VS Code** with Pico extension (recommended)
- **Python 3** for build scripts

### Hardware Required
- **Raspberry Pi Pico 2** (RP2350)
- **USB cable** (USB-A to USB-C)
- **Computer** (Windows, macOS, or Linux)

## Setup Development Environment

### Option 1: VS Code with Pico Extension (Recommended)

1. **Install VS Code**: Download from [code.visualstudio.com](https://code.visualstudio.com/)

2. **Install Pico Extension**:
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X)
   - Search "Raspberry Pi Pico"
   - Install the official extension

3. **Configure Extension**: The extension will auto-configure paths and toolchain

### Option 2: Manual Toolchain Setup

1. **Install ARM GCC Toolchain**:
   ```bash
   # Ubuntu/Debian
   sudo apt update
   sudo apt install gcc-arm-none-eabi cmake build-essential
   
   # macOS (with Homebrew)
   brew install arm-none-eabi-gcc cmake
   
   # Windows (use MSYS2 or install from ARM website)
   # Download from: https://developer.arm.com/downloads/-/gnu-rm
   ```

2. **Install Pico SDK**:
   ```bash
   git clone https://github.com/raspberrypi/pico-sdk.git
   cd pico-sdk
   git submodule update --init
   export PICO_SDK_PATH=/path/to/pico-sdk
   ```

## Obtain grblHAL Source Code

### Option 1: Fork and Clone (Recommended for Development)

1. **Fork the repository**: Go to [github.com/grblHAL/RP2040](https://github.com/grblHAL/RP2040) and click "Fork"

2. **Clone your fork**:
   ```bash
   git clone https://github.com/YOUR_USERNAME/RP2040.git grblhal-rp2040
   cd grblhal-rp2040
   git submodule update --init --recursive
   ```

### Option 2: Direct Clone

```bash
git clone https://github.com/grblHAL/RP2040.git grblhal-rp2040
cd grblhal-rp2040
git submodule update --init --recursive
```

## Configure for Robot Arm

### 1. Copy Configuration Files

Copy our custom configuration files to the grblHAL source:

```bash
# Copy main configuration
cp ../grblhal_config.h ./Inc/my_machine.h

# Copy board mapping
cp ../board_map.h ./Inc/robot_arm_map.h
```

### 2. Select Board Configuration

Edit `CMakeLists.txt` to select our robot arm configuration:

```cmake
# Around line 15-20, change:
set(BOARD_NAME "robot_arm_map.h")
# or
add_compile_definitions(BOARD_FILE="robot_arm_map.h")
```

### 3. Enable Required Features

In `my_machine.h`, ensure these features are enabled:
```c
#define N_AXIS 6                    // 6-axis support
#define ENABLE_SERVO_CONTROL        // PWM servo control
#define ENABLE_SAFETY_DOOR_INPUT    // Safety features
#define ENABLE_LIMITS_INPUT         // Limit switches
```

## Build Process

### Using VS Code (Recommended)

1. **Open Project**: Open the grblhal-rp2040 folder in VS Code

2. **Select Board**: 
   - Press Ctrl+Shift+P
   - Type "Pico: Configure"
   - Select "Raspberry Pi Pico 2" (RP2350)

3. **Select Configuration**:
   - Choose "Release" for production build
   - Choose "Debug" for development

4. **Build**:
   - Press Ctrl+Shift+P
   - Type "Pico: Compile"
   - Wait for build to complete

### Using Command Line

1. **Create Build Directory**:
   ```bash
   mkdir build
   cd build
   ```

2. **Configure CMake**:
   ```bash
   # For Pico 2 (RP2350)
   cmake -DPICO_BOARD=pico2 -DCMAKE_BUILD_TYPE=Release ..
   
   # Alternative with specific board file
   cmake -DBOARD_FILE=robot_arm_map.h -DCMAKE_BUILD_TYPE=Release ..
   ```

3. **Build Firmware**:
   ```bash
   make -j4  # Use 4 cores for faster build
   ```

4. **Verify Build**:
   ```bash
   ls *.uf2  # Should show grblHAL.uf2 file
   ```

## Flash Firmware to Pico 2

### Method 1: Drag and Drop (Easiest)

1. **Enter Bootloader Mode**:
   - Hold **BOOTSEL** button on Pico 2
   - Connect USB cable to computer
   - Release **BOOTSEL** button
   - Pico 2 appears as USB drive "RPI-RP2"

2. **Copy Firmware**:
   - Drag `grblHAL.uf2` file to the RPI-RP2 drive
   - Pico 2 will automatically reboot with new firmware

### Method 2: Using picotool

1. **Install picotool**:
   ```bash
   # Build from source (recommended)
   git clone https://github.com/raspberrypi/picotool.git
   cd picotool
   mkdir build && cd build
   cmake .. && make
   sudo make install
   ```

2. **Flash Firmware**:
   ```bash
   # Put Pico in bootloader mode first
   picotool load grblHAL.uf2 -f
   picotool reboot
   ```

### Method 3: VS Code Integration

1. **Select Target**: Choose "Upload to Pico" from command palette
2. **Connect Pico**: Follow prompts to connect in bootloader mode
3. **Flash**: Extension handles the flashing process automatically

## Verify Installation

### 1. Check USB Connection

After flashing, the Pico 2 should appear as a serial device:

```bash
# Linux/macOS
ls /dev/tty* | grep -E "(ACM|USB)"
# Should show something like /dev/ttyACM0

# Windows (PowerShell)
Get-WmiObject -Class Win32_SerialPort | Select-Object Name,DeviceID
```

### 2. Test Serial Communication

Using a terminal program (PuTTY, screen, minicom):

```bash
# Linux/macOS
screen /dev/ttyACM0 115200

# Or using minicom
minicom -D /dev/ttyACM0 -b 115200
```

Expected response:
```
Grbl 1.1h ['$' for help]
```

### 3. Basic Commands Test

In the terminal, type:
```gcode
$$                  # Show settings
?                   # Request status  
$X                  # Unlock (if alarmed)
G1 X10 F100        # Test movement command
```

## Troubleshooting

### Build Errors

**Error**: "PICO_SDK_PATH not set"
```bash
export PICO_SDK_PATH=/path/to/pico-sdk
```

**Error**: "arm-none-eabi-gcc not found"
- Install ARM GCC toolchain
- Check PATH environment variable

**Error**: CMake version too old
- Install CMake 3.13 or later
- Use `cmake --version` to check

### Flash Errors

**Problem**: Pico not detected as USB drive
- Try different USB cable
- Hold BOOTSEL longer
- Check USB port

**Problem**: .uf2 file not copied
- Ensure file is valid (check size > 0)
- Try copying manually
- Use different file manager

### Communication Errors

**Problem**: No serial device appears
- Check if firmware flashed correctly
- Try different USB port
- Verify driver installation (Windows)

**Problem**: "Permission denied" on Linux
```bash
sudo usermod -a -G dialout $USER
# Logout and login again
```

**Problem**: Garbled output
- Check baud rate (should be 115200)
- Verify correct serial port
- Try different terminal program

## Custom Configuration

### Modifying Pin Assignments

Edit `robot_arm_map.h`:
```c
// Change step pins
#define X_STEP_PIN      2    // Change to your pin
#define Y_STEP_PIN      4    // Change to your pin
// etc.
```

### Adjusting Motor Settings

Edit `my_machine.h`:
```c
// Change steps per unit
#define DEFAULT_X_STEPS_PER_UNIT 200.0    // Adjust for your motor
#define DEFAULT_Y_STEPS_PER_UNIT 200.0    // Adjust for your motor
// etc.
```

### Adding Custom G-codes

In `my_machine.h`:
```c
// Enable custom M-codes
#define ENABLE_CUSTOM_GCODE_M10     // Your custom function
```

Then implement in the source code.

## Advanced Options

### Debug Build

For development and debugging:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DPICO_BOARD=pico2 ..
make -j4
```

### Optimize for Size

For minimal flash usage:
```bash
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DPICO_BOARD=pico2 ..
make -j4
```

### Enable Debugging

Add to `my_machine.h`:
```c
#define ENABLE_DEBUG_OUTPUT
#define DEBUGOUT 1              // Enable debug messages
```

## Updating Firmware

### Update grblHAL Core

```bash
cd grblhal-rp2040
git pull origin master
git submodule update --recursive
# Rebuild and reflash
```

### Apply Configuration Changes

1. Modify configuration files
2. Rebuild firmware
3. Flash to Pico 2
4. Verify operation

## Next Steps

After successful firmware installation:

1. **Configure ROS2 Bridge**: Set up the ROS2 communication package
2. **Calibrate Motors**: Run motor calibration procedures
3. **Test Safety Systems**: Verify emergency stops and limits
4. **Tune Parameters**: Adjust acceleration and feed rates

## Support Resources

- **grblHAL Documentation**: [github.com/grblHAL/core/wiki](https://github.com/grblHAL/core/wiki)
- **Pico SDK Guide**: [raspberrypi.github.io/pico-sdk-doxygen](https://raspberrypi.github.io/pico-sdk-doxygen/)
- **Community Forum**: Join grblHAL discussions for help
- **Our Project Issues**: Report bugs in the robot arm repository