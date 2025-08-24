# PCB Design Files

This directory contains all PCB design files for the ExArm robot controller system.

## Board Designs

### Controller Board (Primary)
**Location**: `controller_board/`
**Purpose**: Main robot controller based on Raspberry Pi Pico 2

**Key Features**:
- Raspberry Pi Pico 2 (RP2350) mounting
- 5x stepper motor driver interfaces with level shifting
- 3x servo PWM output
- Limit switch inputs with pull-up resistors
- Emergency stop and safety inputs
- USB and UART communication
- Power input and distribution

**Critical Requirements**:
- 3.3V → 5V level conversion for stepper drivers
- Proper power isolation between logic and motor power
- Hardware emergency stop implementation
- ESD protection on all external connections

### Interface Board (Secondary)
**Location**: `interface_board/`
**Purpose**: Breakout board for easy connections and testing


### Power Board (Optional)
**Location**: `power_board/`
**Purpose**: Dedicated power distribution and regulation


## Design Guidelines

### Schematic Design
- Follow grblHAL pin assignments from firmware configuration
- Include proper decoupling capacitors
- Add test points for critical signals
- Implement ESD protection on all I/O

### PCB Layout
- Separate analog and digital grounds
- Keep high-frequency signals short and properly terminated
- Use ground planes for noise reduction
- Minimize via count on critical signals

### Component Selection
- Use automotive/industrial grade components where appropriate
- Ensure components are readily available and not obsolete
- Consider thermal requirements and power dissipation
- Select connectors rated for expected current and cycles

## File Organization

Each board directory contains:
```
board_type/
├── board_name/          # KiCad files (.sch, .pro)Schemati & Layout (.kicad_pcb)
├── gerbers/           # Manufacturing files (Gerber, drill, pick & place)
├── 3d_models/         # Custom component 3D models
├── assembly/          # Assembly drawings and instructions
├── docs/              # Board-specific documentation
├── BOM.csv           # Bill of Materials
└── README.md         # Board specifications and notes
```

## Version Control

- Commit design files at major milestones
- Tag releases when sending to manufacturing
- Include PDF exports of schematics for review
- Document all design changes in commit messages

## Manufacturing Notes

- Target PCB specifications: 2-layer for simple boards, 4-layer for complex
- Standard PCB thickness: 1.6mm
- Minimum trace width: 0.1mm (4 mil)
- Minimum via size: 0.2mm (8 mil)
- Standard finish: HASL or ENIG for fine pitch components 