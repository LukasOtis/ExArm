# grblHAL Integration for Robot Arm Control

This directory contains all the grblHAL implementation files, configurations, and ROS2 integration code for the 5-6 DOF excavator-style robot arm project.

## Directory Structure

```
grblhal_integration/
├── README.md                           # This file
├── firmware/                           # grblHAL firmware files
│   ├── grblhal_config.h               # Main grblHAL configuration
│   ├── board_map.h                    # Pin mapping for RP2350
│   ├── build_instructions.md          # How to compile and flash
│   └── precompiled/                   # Ready-to-flash .uf2 files
├── ros2_bridge/                       # ROS2 <-> grblHAL communication
│   ├── grbl_interface/                # ROS2 package
│   │   ├── src/
│   │   │   ├── grbl_controller.cpp    # Main controller node
│   │   │   ├── grbl_translator.cpp    # Joint positions to G-code
│   │   │   └── grbl_monitor.cpp       # Safety and status monitoring
│   │   ├── include/
│   │   ├── CMakeLists.txt
│   │   └── package.xml
│   └── launch/                        # Launch files
├── gcode_reference/                   # G-code documentation
│   ├── supported_commands.md          # grblHAL G-code reference
│   ├── axis_mapping.md                # How joints map to G-code axes
│   └── examples/                      # Example G-code sequences
├── configuration/                     # Configuration files
│   ├── grbl_settings.txt              # grblHAL $ settings
│   ├── axis_parameters.yaml           # Joint/axis parameters for ROS2
│   └── safety_limits.yaml             # Safety and limit configurations
└── testing/                           # Test scripts and validation
    ├── communication_test.py          # Test Pi <-> Pico communication
    ├── axis_calibration.py            # Individual axis testing
    └── safety_validation.py           # Emergency stop testing
```

## Quick Start

1. **Flash grblHAL Firmware**: See `firmware/build_instructions.md`
2. **Build ROS2 Bridge**: See `ros2_bridge/README.md`  
3. **Configure Settings**: Use files in `configuration/`
4. **Test Communication**: Run scripts in `testing/`

## Architecture Overview

```
ROS2 (Raspberry Pi)          grblHAL (Pico 2 RP2350)
┌─────────────────┐          ┌─────────────────┐
│  Joint Angles   │   G-code │  Step/Direction │
│  [J1,J2,J3...]  │ ───────> │  Signals        │
└─────────────────┘          └─────────────────┘
         │                            │
         │                            │
┌─────────────────┐          ┌─────────────────┐
│   Kinematics    │          │   Servo PWM     │
│   Planning      │          │   Control       │
└─────────────────┘          └─────────────────┘
```

## Axis Configuration

**Physical Robot Joints → grblHAL Axes Mapping:**

| Joint | Type | grblHAL Axis | G-code | Description |
|-------|------|-------------|--------|-------------|
| J1 | Stepper | X | G1 X### | Base rotation |
| J2 | Stepper | Y | G1 Y### | Shoulder |
| J3 | Stepper | Z | G1 Z### | Elbow |
| J4 | Stepper | A | G1 A### | Linear actuator 1 |
| J5 | Stepper | B | G1 B### | Linear actuator 2 |
| J6 | Servo | C (PWM) | G1 C### S### | End effector rotation |

## Communication Protocol

**ROS2 → grblHAL**: Joint trajectories converted to G-code commands
**grblHAL → ROS2**: Status reports parsed to joint states

Example:
```
ROS2: [10.5, -5.2, 15.0, 20.0, -10.0, 90.0] degrees
G-code: G1 X10.5 Y-5.2 Z15.0 A20.0 B-10.0 C90 S1500 F1000
```

## Next Steps

1. Review the configuration files in each subdirectory
2. Modify `grblhal_config.h` for your specific hardware
3. Build and test the ROS2 bridge package
4. Calibrate axis parameters using the testing scripts