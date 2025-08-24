# Electrical System Design

This directory contains system-level electrical design documentation for the ExArm robot controller, including high-level schematics, wiring diagrams, and component specifications.

## Directory Contents

### System Schematics
**Location**: `schematics/`
**Purpose**: High-level electrical system diagrams

**Contents**:
- Power distribution architecture
- Signal flow diagrams
- Inter-board connections
- System grounding scheme
- EMI/EMC considerations

### Wiring Diagrams
**Location**: `wiring_diagrams/`
**Purpose**: Detailed cable and harness specifications

**Contents**:
- Cable harness diagrams
- Connector pinout assignments
- Wire gauge and color coding
- Routing and installation guidelines
- Cable length specifications

### Component Specifications
**Location**: `component_specs/`
**Purpose**: Component selection criteria and datasheets

**Contents**:
- Component selection rationale
- Key parameter comparisons
- Supplier information and part numbers
- Environmental ratings and certifications
- Alternative component options

## System Architecture

### Power Distribution
```
24V Main Power → Buck Converters → 5V Logic + 12V Motors
                ↓
            3.3V Logic (from Pico2 regulator)
```

**Power Rails**:
- **24V**: Main input power (stepper motors)
- **12V**: Alternative motor voltage (configurable)
- **5V**: Logic power for drivers and interfaces
- **3.3V**: Microcontroller and sensor power

### Signal Architecture
```
ROS2 (RPi) ←→ UART ←→ Pico2 ←→ Level Shifters ←→ Motor Drivers
                        ↓
                    Sensors & Safety
```

**Signal Types**:
- **Digital Control**: Step/direction signals (5V logic)
- **PWM**: Servo control (3.3V logic)
- **Digital Input**: Limit switches and safety (3.3V logic)
- **Communication**: UART and USB (3.3V logic)

## Electrical Requirements

### Power Requirements
| Component | Voltage | Current | Power |
|-----------|---------|---------|-------|
| Pico2 Controller | 3.3V | 200mA | 0.7W |
| Level Shifters | 5V | 100mA | 0.5W |
| Stepper Drivers | 5V | 500mA | 2.5W |
| Servo Motor | 5V | 1000mA | 5.0W |
| **Total Logic** | **5V** | **1.8A** | **9W** |
| Stepper Motors | 24V | 2000mA | 48W |
| **Total System** | **24V** | **4A** | **96W** |

### Signal Specifications
| Signal Type | Voltage | Frequency | Rise Time |
|-------------|---------|-----------|-----------|
| Step Pulses | 5V | 0-50kHz | <1μs |
| Direction | 5V | DC | <1μs |
| PWM Servo | 3.3V | 50Hz | <10μs |
| Limit Switches | 3.3V | DC | <100μs |
| UART | 3.3V | 115kbps | <1μs |

### Environmental Ratings
- **Operating Temperature**: 0°C to +60°C
- **Storage Temperature**: -20°C to +80°C
- **Humidity**: 5% to 95% non-condensing
- **Vibration**: 2G peak, 10-2000Hz
- **EMC**: EN 55011 Class B emissions, EN 55024 immunity

## Safety Requirements

### Electrical Safety
- **Isolation**: Logic and power grounds properly isolated
- **Protection**: Overcurrent protection on all power rails
- **ESD**: ±15kV protection on all external connections
- **Fault Tolerance**: Fail-safe operation on power loss

### Functional Safety
- **Emergency Stop**: Hardware-based power cutoff
- **Limit Switches**: Independent safety monitoring
- **Watchdog**: Communication timeout handling
- **Diagnostics**: Comprehensive fault detection and reporting

### EMC Compliance
- **Emissions**: EN 55011 Class B (residential environments)
- **Immunity**: EN 55024 (industrial immunity standards)
- **Automotive**: ISO 7637 transient immunity (if applicable)

## Design Standards

### Electrical Design
- **Wire Gauge**: AWG 18 minimum for power, AWG 24 for signals
- **Connector Rating**: 125% of maximum current
- **Voltage Rating**: 2x operating voltage minimum
- **Temperature Rating**: 105°C minimum for automotive applications

### Signal Integrity
- **Impedance Control**: ±10% for critical signals
- **Crosstalk**: -40dB minimum between adjacent signals
- **Noise Margin**: 30% minimum for digital signals
- **EMI Filtering**: RC filters on all external interfaces

### Grounding
- **Single Point Ground**: Star grounding configuration
- **Ground Plane**: Continuous ground plane on all PCBs
- **Shield Grounding**: Cable shields grounded at one end only
- **Safety Ground**: Chassis connected to earth ground

## Component Selection Criteria

### General Requirements
- **Automotive Grade**: AEC-Q100 qualified where available
- **Industrial Temperature**: -40°C to +85°C minimum
- **RoHS Compliance**: Lead-free and environmentally friendly
- **Availability**: Multiple suppliers and long-term availability

### Specific Components

#### Power Components
- **Buck Converters**: >90% efficiency, current limiting
- **Capacitors**: Low ESR, high ripple current rating
- **Inductors**: Shielded construction, low DC resistance
- **Fuses**: Fast-acting, appropriate I²t rating

#### Signal Components
- **Level Shifters**: 74HCT245 or equivalent
- **Connectors**: Automotive-grade, sealed where required
- **Cables**: Twisted pair for differential signals
- **Filters**: Ferrite beads and RC networks

## Testing and Validation

### Electrical Testing
- **Insulation Resistance**: >10MΩ at 500VDC
- **Dielectric Strength**: 1500VAC for 60 seconds
- **Ground Continuity**: <0.1Ω resistance
- **Power Supply Regulation**: ±1% line and load regulation

### Signal Quality
- **Rise/Fall Times**: <1μs for digital signals
- **Overshoot/Undershoot**: <10% of signal amplitude
- **Crosstalk**: -40dB minimum between channels
- **EMI**: Pre-compliance testing to applicable standards

### Environmental Testing
- **Temperature Cycling**: -40°C to +85°C, 100 cycles
- **Humidity**: 95% RH at 40°C for 96 hours
- **Vibration**: Per IEC 60068-2-6
- **Shock**: Per IEC 60068-2-27

## Documentation Standards

### Schematic Documentation
- **Standards**: IEEE 315 symbols, IEC 60617 alternates
- **Clarity**: Logical signal flow, minimal wire crossings
- **Annotations**: Clear component designators and values
- **Revisions**: Proper revision control and change tracking

### Cable Documentation
- **Wire Lists**: Complete from-to connectivity
- **Color Coding**: Consistent color scheme
- **Specifications**: Wire gauge, insulation type, shield requirements
- **Testing**: Continuity and insulation testing procedures

## Current Status

- [ ] System requirements definition
- [ ] Power architecture design
- [ ] Signal architecture design
- [ ] Component selection
- [ ] Schematic creation
- [ ] Wiring diagram creation
- [ ] Design review and validation

## File Organization

```
electrical/
├── schematics/
│   ├── power_distribution.pdf
│   ├── signal_architecture.pdf
│   └── grounding_scheme.pdf
├── wiring_diagrams/
│   ├── main_harness.pdf
│   ├── motor_connections.pdf
│   └── sensor_wiring.pdf
└── component_specs/
    ├── power_components/
    ├── signal_components/
    └── connector_specs/
```

## Integration Notes

- **PCB Design**: Electrical requirements feed into PCB specifications
- **Mechanical**: Connector and cable routing requirements
- **Software**: Signal timing and interface specifications
- **Testing**: Electrical test procedures and acceptance criteria 