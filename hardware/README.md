# Hardware Design & PCB Development

This directory contains all hardware design files for the ExArm robot arm project, including PCB design, mechanical components, and electrical system documentation.

## Directory Structure

```
hardware/
├── pcb/                    # PCB design files
│   ├── controller_board/   # Main Pico2 controller PCB
│   ├── interface_board/    # Level shifter/breakout board
│   ├── power_board/        # Power distribution (optional)
│   └── libraries/          # Custom KiCad libraries/footprints
├── mechanical/             # Mechanical design files
│   ├── enclosures/        # Board enclosures/cases
│   ├── mounting/          # Mounting brackets/hardware
│   └── connectors/        # Connector specifications
└── electrical/            # System electrical design
    ├── schematics/        # System-level schematics
    ├── wiring_diagrams/   # Cable/harness diagrams
    └── component_specs/   # Component datasheets & selection
```

## PCB Development Workflow

### Design Tools
- **Recommended**: KiCad (free, open source, excellent for robot projects)
- **Alternative**: Altium Designer, Eagle, or other professional EDA tools

### Development Process
1. **Requirements**: Define board specifications and pin requirements
2. **Schematic**: Create circuit schematic with proper connectivity
3. **Layout**: Design PCB layout with proper signal integrity
4. **Review**: Design review and validation
5. **Manufacturing**: Generate Gerber files and order PCBs
6. **Assembly**: Component placement and soldering
7. **Testing**: Functional testing and validation

## Current Hardware Requirements

Based on the grblHAL integration requirements:

### Main Controller Board (Pico2-based)
- **MCU**: Raspberry Pi Pico 2 (RP2350)
- **Motor Control**: 5x stepper motor outputs (step/direction)
- **Servo Control**: 1x PWM servo output
- **Level Shifting**: 3.3V → 5V conversion for stepper drivers
- **I/O**: Limit switches, emergency stop, status LEDs
- **Communication**: USB + UART for ROS2 bridge

### Key Design Considerations
- **Logic Level Conversion**: Critical 3.3V/5V compatibility
- **Power Distribution**: Separate power domains for logic and motors
- **Safety Systems**: Hardware emergency stop implementation
- **Signal Integrity**: Proper grounding and shielding for step signals
- **Thermal Management**: Heat dissipation for power components

## Getting Started

1. **Review Requirements**: See `../grblhal_integration/hardware_requirements.md`
2. **Choose Tools**: Install KiCad or preferred EDA software
3. **Start with Controller Board**: Main Pico2 controller is highest priority
4. **Reference Existing Work**: Use grblHAL pin assignments as starting point

## Links to Related Documentation

- [grblHAL Hardware Requirements](../grblhal_integration/hardware_requirements.md)
- [Robot Arm Hardware Analysis](../robot_arm_hardware_analysis.md)  
- [grblHAL Firmware Configuration](../grblhal_integration/firmware/grblhal_config.h)
- [Pin Assignments](../grblhal_integration/firmware/boards/)

## Manufacturing Partners

*To be added when PCB designs are ready for production*

## Testing & Validation

*Documentation for hardware testing procedures will be added as designs progress* 