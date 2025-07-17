# grblHAL RP2040/RP2350 - Robot Arm Optimized

A **streamlined grblHAL driver** for the Raspberry Pi Pico/Pico2 (RP2040/RP2350) processors, specifically optimized for **5-6 DOF robot arm control**.

This repository has been cleaned and optimized from the original grblHAL RP2040 driver, removing ~80% of unnecessary files while maintaining all essential functionality for robot arm applications.

## 🤖 Target Application

**5-6 DOF Excavator-Style Robot Arm:**
- RP Pico2 (RP2350) controller
- 5 stepper motors (joints J1-J5)  
- 1 servo motor (joint J6 end effector)
- ROS2 integration via USB serial
- G-code command interface

## 📁 Repository Structure

├── README.md # This file ├── CMakeLists.txt # Main build configuration ├── pico_sdk_import.cmake # Pico SDK integration ├── my_machine.h # Main configuration file ├── main.c # Application entry point ├── driver.c/h # Core hardware driver ├── driver.json # Driver capabilities ├── driverPIO

.pio # PIO state machine code ├── serial.c/h # Serial communication ├── usb_serial.c/h # USB CDC serial ├── stdio_usb_descriptors.c # USB descriptors ├── i2c.c/h # I2C communication ├── spi.c/h # SPI communication
├── ioports.c # Digital I/O ports ├── boards/ # Board pin mappings │ ├── RP2350B_5X_map.h # 5-axis RP2350 mapping │ ├── generic_map_8axis.h # 8-axis generic mapping │ └── generic_map.h # Basic 3-axis mapping ├── grbl/ # Core grbl framework (empty) ├── motors/ # Motor control framework (empty) ├── plugins/ # Plugin framework (empty) └── robot_arm_grblhal_analysis.md # Analysis of cleanup performed

// For RP2350 

## 🗑️ What Was Removed

The following components were removed as they're not needed for robot arm applications:

### CNC-Specific Features
- `spindle/` - Spindle control
- `plasma/` - Plasma cutting
- `laser/` - Laser cutting/engraving
- `embroidery/` - Embroidery machines

### Networking & Communication  
- `networking/` - WiFi/Ethernet
- `webui/` - Web interface
- `bluetooth/` - Bluetooth connectivity
- All related source files (`wifi.c`, `w5x00_ll_driver.c`, etc.)

### Storage Systems
- `sdcard/` - SD card support
- `fatfs/` - FAT filesystem
- `littlefs/` - Little filesystem

### Input/UI Devices
- `keypad/` - Physical keypads
- Advanced USB configuration

### Motor Driver Specifics
- `trinamic/` - TMC stepper drivers
- `tmc_uart.c` - TMC UART communication

### Other Features
- `fans/` - Cooling fan control
- `eeprom/` - External EEPROM
- Most board-specific mappings
- Documentation and IDE files

## 🔧 Configuration for Robot Arm

### Board Selection
Edit `my_machine.h` to enable the appropriate board:

```c
// For RP2350 5-axis board (recommended)
#define BOARD_RP23U5XBB

// OR for custom 8-axis configuration
// #define BOARD_GENERIC_8AXIS