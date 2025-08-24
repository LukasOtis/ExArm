# Interface Breakout Board

Secondary board providing easy connections and testing access for the ExArm robot controller system.

## Purpose

The interface board serves as a user-friendly breakout for the main controller board, providing:
- Easy-to-use terminal blocks for motor and sensor connections
- Test points for signal monitoring and debugging
- Visual status indicators
- Configuration jumpers and switches
- Standard connector pinouts for common peripherals

## Features

### Motor Connection Interface
- **Stepper Motor Outputs**: 5x terminal blocks
  - Step/Direction/Enable signals
  - Clearly labeled (X, Y, Z, A, B axes)
  - LED indicators for step pulses
- **Servo Output**: 1x 3-pin servo connector
  - Standard servo pinout (Signal, +5V, GND)
  - Compatible with standard hobby servos

### Sensor Input Interface
- **Limit Switch Inputs**: 6x terminal blocks
  - 3-pin connections (Signal, +3.3V, GND)
  - Pull-up resistors integrated
  - LED indicators for switch states
- **Safety Inputs**: Dedicated connectors
  - Emergency stop (normally closed)
  - Safety door switch
  - Hardware enable/disable jumpers

### Test and Debug Features
- **Test Points**: All critical signals accessible
- **Oscilloscope Headers**: Easy probe connection
- **Signal Monitoring**: LED indicators for all digital signals
- **Power Monitoring**: Voltage and current test points

### Configuration Options
- **Jumper Selectable Options**:
  - Pull-up resistor enable/disable
  - Signal routing options
  - Power source selection
- **DIP Switches**: Runtime configuration
- **Address Selection**: For multiple board systems

## Electrical Specifications

### Input/Output Levels
- **Logic Levels**: 3.3V and 5V compatible
- **Input Current**: <1mA per input
- **Output Current**: 20mA max per output
- **Input Impedance**: >10kΩ (with pull-ups)

### Power Requirements
- **Supply Voltage**: 5V ±5%
- **Current Consumption**: <100mA (without loads)
- **Power Input**: 2.1mm barrel jack or terminal block

### Signal Integrity
- **Rise/Fall Times**: <100ns
- **Propagation Delay**: <50ns
- **EMI Filtering**: RC filters on all inputs
- **ESD Protection**: ±15kV on all external connections

## Connector Pinouts

### Stepper Motor Connectors (5x)
```
Pin 1: Step Signal (5V logic)
Pin 2: Direction Signal (5V logic)  
Pin 3: Enable Signal (5V logic, active low)
Pin 4: +5V Power
Pin 5: Ground
```

### Servo Connector
```
Pin 1: Signal (3.3V PWM, 50Hz)
Pin 2: +5V Power
Pin 3: Ground
```

### Limit Switch Connectors (6x)
```
Pin 1: Signal Input (3.3V logic)
Pin 2: +3.3V Power
Pin 3: Ground
```

### Communication Connector
```
Pin 1: UART TX (3.3V)
Pin 2: UART RX (3.3V)
Pin 3: Ground
Pin 4: +3.3V Power
```

## LED Indicators

### Status LEDs
- **Power**: Green LED for power good
- **Activity**: Blue LED for communication activity
- **Error**: Red LED for error conditions

### Signal LEDs
- **Step Signals**: 5x yellow LEDs (X, Y, Z, A, B)
- **Limit Switches**: 6x green LEDs for switch states
- **Safety**: 2x red LEDs (E-stop, safety door)

## Physical Specifications

### Board Dimensions
- **Size**: 100mm x 80mm (estimated)
- **Thickness**: 1.6mm standard PCB
- **Mounting**: 4x M3 mounting holes

### Connectors
- **Terminal Blocks**: 5mm pitch screw terminals
- **Headers**: 2.54mm pitch pin headers
- **Servo Connector**: Standard 0.1" servo header

## Design Considerations

### Signal Routing
- **Star Ground**: Single point grounding for noise reduction
- **Separate Planes**: Digital and analog ground separation
- **Shielding**: Guard traces around sensitive signals

### Mechanical
- **Strain Relief**: All external cables properly supported
- **Connector Orientation**: Logical layout for easy wiring
- **Label Clarity**: Large, clear labels for all connections

### Testing
- **Boundary Scan**: Test points for automated testing
- **Functional Test**: Built-in test patterns
- **Burn-in**: Extended temperature testing capability

## Assembly Instructions

1. **SMT Components**: Automated placement and reflow
2. **Through-hole Components**: Manual insertion and wave solder
3. **Connectors**: Final assembly step
4. **Testing**: Functional test before shipment

## Quality Control

### Inspection Points
- **Visual Inspection**: Component placement and solder quality
- **Electrical Test**: Continuity and isolation testing
- **Functional Test**: End-to-end signal verification
- **Environmental Test**: Temperature and humidity cycling

### Test Procedures
- **Power-on Test**: All voltage rails within specification
- **Signal Integrity**: Rise/fall times and noise margins
- **Connector Test**: All connections verified
- **LED Test**: All indicators functional

## Current Status

- [ ] Requirements definition
- [ ] Schematic design
- [ ] PCB layout
- [ ] Prototype manufacturing
- [ ] Testing and validation
- [ ] Production qualification

## Design Files

- **Schematic**: `schematic/interface_board.sch`
- **PCB Layout**: `pcb_layout/interface_board.kicad_pcb`
- **Assembly**: `assembly/assembly_instructions.pdf`
- **Test**: `docs/test_procedures.pdf`

## Integration Notes

- **Main Controller**: Connects via ribbon cable or wire harness
- **Mounting**: Can be mounted in same enclosure or separately
- **Cables**: Standard cables for common motor types
- **Documentation**: Clear labeling and documentation for end users 