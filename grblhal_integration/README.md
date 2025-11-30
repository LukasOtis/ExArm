# grblHAL Integration for Robot Arm Control

> Note: Firmware docs moved. For building, features, M-codes and pinout, see:
> `grblhal_integration/firmware_GRBL/README.md`

This directory contains the complete grblHAL implementation for a 5-axis robot arm controller running on Raspberry Pi Pico 2 (RP2350). The project provides a minimal, robot arm-optimized version of grblHAL with CNC-specific features removed.

## Project Status

✅ **grblHAL Core**: Integrated and building  
✅ **Compilation**: RP2350 (CMake/Ninja) build working  
✅ **Configuration**: Robot arm optimized with CNC features disabled  
✅ **Documentation**: See `firmware_GRBL/README.md` for up-to-date details  
🔄 **Hardware Testing**: Ready for physical integration  
🔄 **ROS2 Bridge**: Awaiting hardware validation  

## Directory Structure

```
grblhal_integration/
├── README.md                           # This file - integration overview
├── firmware_GRBL/                      # Pico 2 firmware (authoritative docs inside)
│   ├── README.md                       # Build, features, pinout (current)
│   ├── CMakeLists.txt                  # RP2350 production build
│   ├── boards/                         # Board pin mappings
│   ├── grbl/                           # grblHAL core
│   └── ...                             # drivers, plugins, storage, etc.
├── ros2_bridge/                        # ROS2 <-> grblHAL communication
│   └── grbl_interface/
├── gcode_reference/                    # G-code documentation
│   └── axis_mapping.md
├── testing/
│   └── communication_test.py
└── BUILD_INSTRUCTIONS.md               # Pointers to firmware_GRBL/README.md
```

## Architecture Overview

### System Design
```
ROS2 (Raspberry Pi 4)        grblHAL (Pico 2 RP2350)
┌─────────────────┐          ┌─────────────────┐
│  Joint Commands │   G-code │  Step/Direction │  Level    ┌─────────────┐
│  [J1..J5]       │ ───────> │  Signals        │ Shifter ─>│ Stepper     │
│                 │   USB/   │                 │  3.3→5V   │ Drivers     │
│  Joint States   │ <─────── │  Status Reports │           └─────────────┘
└─────────────────┘   UART   └─────────────────┘
                                      │
                                      ├── PWM ──> Servo / Aux PWM
                                      ├── GPIO ─> Limit / Aux IO
                                      └── GPIO ─> Emergency Stop
```

For detailed build steps, implemented M-codes (M280/M68/M281), and the exact board pinout, refer to `firmware_GRBL/README.md`.

## Notes
- This overview intentionally omits low-level details to avoid duplication. The authoritative, current information lives in `firmware_GRBL/README.md`.