# Pico2 Robot Controller Board

Main controller board for the ExArm robot arm system, based on Raspberry Pi Pico 2 (RP2350).

## Board Specifications

### Microcontroller
- **MCU**: Raspberry Pi Pico 2 (RP2350B)
- **Architecture**: Dual-core ARM Cortex-M33 OR RISC-V Hazard3 @150MHz
- **Memory**: 520KB SRAM, 4MB Flash
- **I/O**: 26 GPIO pins, 16 PWM channels, 3 PIO blocks
- **Erratum**: A2 E9 - requires ≤8.2kΩ pull-downs for reliable input operation

### Motor Control Outputs
- **Stepper Motors**: 5 axes (X, Y, Z, A, B)
  - Step/Direction pairs: GPIO 2/3, 4/5, 6/7, 8/9, 10/11 (paired for optimal routing)
  - Shared enable: GPIO 12
- **PWM Outputs**: 3 independent channels
  - Servo control: GPIO 13 (50Hz PWM, 1-2ms pulse width)
  - Spindle/Laser: GPIO 14 (1-25kHz variable frequency)
  - Fan/Coolant: GPIO 16 (independent frequency control)

### Input/Output
- **Limit Switches**: 5 inputs with pull-up resistors
  - X, Y, Z, A, B limits: GPIO 15, 17-20 (B limit on GPIO 20)
- **General Inputs**:
  - `c_input`: GPIO 21 (custom input, isolated)
- **Safety Inputs**:
  - Emergency stop: GPIO 22 (active-low, normally closed)
- **General-Purpose Outputs**:
  - `output_a`: GPIO 26 (programmable output)
  - `output_b`: GPIO 27 (programmable output)
  - `output_c`: GPIO 28 (programmable output)

### Level Shifting
- **Critical Requirement**: 3.3V (Pico2) → 5V (stepper drivers)
- **Solution**: 74HCT245 octal bus transceivers (2x required)
- **Signals**: All step/direction signals must be level shifted

### Power Requirements
- **Logic Power**: 3.3V @ 250mA (from Pico2 regulator, higher due to RP2350B performance)
- **I/O Power**: 5V @ 500mA (for level shifters and outputs)
- **Motor Power**: 12V/24V (pass-through to stepper drivers)
- **Spindle Power**: 12V/24V (for spindle motor controller)

### Communication
- **USB**: Built-in Pico2 USB-C connector (primary communication)
- **UART**: GPIO 0 (TX), GPIO 1 (RX) for ROS2 bridge
- **UART Header**: 4-pin JST-XH connector (GND, 3V3, TX, RX) for development/debugging
- **Debug**: SWD pins available for programming/debugging

## Pin Assignment Table (RP2350B Optimized)

| GPIO | Pico Pin# | Function | Direction | Voltage | PWM Slice | Notes |
|------|-----------|----------|-----------|---------|-----------|-------|
| 0 | 1 | UART TX | Output | 3.3V | 0A | ROS2 bridge communication |
| 1 | 2 | UART RX | Input | 3.3V | 0B | ROS2 bridge communication |
| 2 | 4 | X Step | Output | 5V* | 1A | Base rotation stepper |
| 7 | 10 | X Direction | Output | 5V* | 1B | Base rotation direction |
| 3 | 5 | Y Step | Output | 5V* | 2A | Shoulder stepper |
| 8 | 11 | Y Direction | Output | 5V* | 2B | Shoulder direction |
| 4 | 6 | Z Step | Output | 5V* | 3A | Elbow stepper |
| 9 | 12 | Z Direction | Output | 5V* | 3B | Elbow direction |
| 5 | 7 | A Step | Output | 5V* | 4A | Linear actuator 1 |
| 10 | 9 | A Direction | Output | 5V* | 4B | Linear actuator 1 direction |
| 6 | 14 | B Step | Output | 5V* | 5A | Linear actuator 2 |
| 11 | 15 | B Direction | Output | 5V* | 5B | Linear actuator 2 direction |
| 12 | 16 | Enable (shared) | Output | 5V* | 6A | All stepper enables, 4.7kΩ pull-up |
| 13 | 17 | PWM A: Servo PWM | Output | 3.3V | 6B | End effector servo (50Hz) |
| 14 | 19 | PWM B: Spindle/Laser PWM | Output | 5V* | 7A | Variable speed (1-25kHz) |
| 15 | 20 | X Limit | Input | 3.3V | 7B | X-axis limit, 4.7kΩ pull-down |
| 16 | 21 | PWM C: Fan/Coolant PWM | Output | 3.3V | 0A | Independent frequency control |
| 17 | 22 | Y Limit | Input | 3.3V | 0B | Y-axis limit, 4.7kΩ pull-down |
| 18 | 24 | Z Limit | Input | 3.3V | 1A | Z-axis limit, 4.7kΩ pull-down |
| 19 | 25 | A Limit | Input | 3.3V | 1B | A-axis limit, 4.7kΩ pull-down |
| 20 | 26 | B Limit | Input | 3.3V | 2A | B-axis limit, 4.7kΩ pull-down |
| 21 | 27 | C Input (`c_input`) | Input | 3.3V | 2B | Isolated general-purpose input |
| 22 | 29 | Emergency Stop (`estop_all`) | Input | 3.3V | 3A | Active-low, normally closed |
| 26 | 31 | Output_A | Output | 3.3V | 5A | Programmable output |
| 27 | 32 | Output_B | Output | 3.3V | 5B | Programmable output |
| 28 | 34 | Output_C | Output | 3.3V | 6A | Programmable output |

*\* Requires level shifting from 3.3V to 5V*  
**Notes**: 
- PWM slice assignments verified against [RP2350 official pinout](https://learn.adafruit.com/adafruit-feather-rp2350/pinouts)
- Pull-down resistors ≤4.7kΩ required for RP2350B A2 E9 erratum compliance
- PWM slice conflicts noted where some outputs share slices with step signals

## Design Approach: Pico2 + Custom Interface Board

### Development Strategy
**Modular Design**: Pico2 connected to custom PCB via headers
- **Advantages**: 
  - Easy development and debugging
  - Pico2 can be replaced if damaged
  - Cost-effective prototyping
  - Access to Pico2's built-in USB and debugging
- **Considerations**:
  - Trace length <10cm (easily achievable)
  - Use ground plane for signal integrity
  - Add series resistance (22-47Ω) on fast signals if needed

### Signal Isolation Architecture
```
Pico2 GPIO → Digital Isolators → 5V/12V/24V Outputs
         ↓
    Opto-isolators ← External Inputs (Endstops, Safety)
         ↓
    ADC Inputs ← Analog Sensors (Probe, Current)
```

**Professional Isolation**: ADuM1200ARZ digital isolators
- **Advantages**: Galvanic isolation, noise immunity, safety
- **Speed**: 1Mbps+ (more than adequate for step signals)
- **Voltage**: 3.3V MCU side, 5V driver side

### Power Distribution
- **3.3V Rail**: From Pico2 onboard regulator
- **5V Rail**: External supply for logic and motors
- **Motor Power**: 12V or 24V, isolated from logic
- **Spindle Power**: 12V or 24V for spindle motor controller

### PIO (Programmable I/O) Advantages
**grblHAL + RP2350B PIO Benefits for Complex Control**:
- **Hardware Timing**: PIO generates step pulses independently of CPU
- **Jitter-Free**: No software delays or interrupt latency issues  
- **Flexible Pins**: Any GPIO can be assigned to any PIO function
- **Precise Control**: Hardware state machines handle timing
- **Enhanced Capability**: 3 PIO blocks (vs 2 in RP2040) for more parallel processing
- **Multi-Channel PWM**: Independent PWM for spindle, servo, coolant
- **Signal Pairing**: Step/direction pairs for optimal signal integrity


```

**RP2350B Architecture Choice**:
- **ARM Cortex-M33**: Maximum grblHAL compatibility and performance
- **RISC-V Hazard3**: Open architecture option for advanced users
- Both cores support the same pin assignments and functionality

### Input Protection
- **ESD Protection**: TVS diodes on all external connections
- **Opto-isolation**: PC817 for 12V/24V endstop inputs  
- **Pull-down Resistors**: 4.7kΩ (RP2350B E9 erratum compliance)
- **Pull-up Resistors**: 4.7kΩ on stepper enable, 10kΩ on driver side
- **Filtering**: RC filters on critical inputs

## Bill of Materials (Industrial Grade)

### Core Components
| Component | Quantity | Part Number | Description | Package | Notes |
|-----------|----------|-------------|-------------|---------|-------|
| Raspberry Pi Pico 2 | 1 | - | Main microcontroller | Module | Connected via headers for development |

### 1. Stepper Motor Outputs (Step/Dir/Enable)
| Component | Quantity | Part Number | Description | Package | Notes |
|-----------|----------|-------------|-------------|---------|-------|
| Digital Isolator | 3 | ADuM1200ARZ | Dual-channel isolator | SOIC-8 | 3.3V MCU → 5V driver side |
| Digital Isolator | 1 | ISO7221BDR | Alternative option | SOIC-8 | For enable signals |
| Pull-up Resistors | 6 | - | 1kΩ-10kΩ | 0805 | On 5V output side |
| Logic Buffer | 2 | SN74HCT125N | Quad buffer | DIP-14/SOIC-14 | Strong 5V output drive |
| TVS Diode Array | 2 | SMAJ5.0A | ESD protection | SMA | Protect step/dir outputs |
| IDC Connector | 1 | 640456-2 (TE) | 2x6 header | IDC | Ribbon cable to drivers |

### 2. PWM-Controlled Outputs (Servo, Spindle, Coolant)
| Component | Quantity | Part Number | Description | Package | Notes |
|-----------|----------|-------------|-------------|---------|-------|
| Digital Isolator | 2 | ADuM1200ARZ | PWM isolation | SOIC-8 | Spindle + coolant |
| Gate Driver | 3 | TC4420 | MOSFET driver | SOIC-8 | For PWM switching |
| Output MOSFET | 3 | IRLZ44N | Logic-level N-FET | TO-220 | 12-24V load switching |
| Flyback Diode | 3 | SS14 | Schottky diode | SMA | Across inductive loads |
| Pull-up Resistor | 3 | - | 4.7kΩ-10kΩ | 0805 | To 12V rail if needed |
| PWM Connectors | 3 | JST-XH 3-pin | B3B-XH-A | Servo, spindle, coolant |

### 3. Digital Inputs (Endstops, Safety) - RP2350B E9 Compliant
| Component | Quantity | Part Number | Description | Package | Notes |
|-----------|----------|-------------|-------------|---------|-------|
| Opto-isolator | 6 | PC817 | Transistor output | DIP-4 | 12V/24V endstop inputs |
| Input Resistors | 6 | - | 1kΩ-4.7kΩ | 0805 | Sets opto LED current |
| Pull-down Resistors | 6 | - | 4.7kΩ | 0805 | MCU side, E9 erratum compliance |
| Pull-up Resistor | 1 | - | 4.7kΩ | 0805 | Stepper enable (GPIO 12) |
| TVS Diodes | 4 | SMAJ12A | Input protection | SMA | For noisy environments |

### 4. CNC Control Outputs (M-Code Functions)
| Component | Quantity | Part Number | Description | Package | Notes |
|-----------|----------|-------------|-------------|---------|-------|
| Output Driver | 2 | ULN2003A | Darlington array | DIP-16 | M7/M62 relay drivers |
| Protection Diode | 2 | 1N4148 | Fast switching | SOD-123 | Relay flyback protection |
| Pull-up Resistor | 2 | - | 10kΩ | 0805 | Output enable pull-ups |
| Relay Connectors | 2 | JST-XH 3-pin | B3B-XH-A | M7/M62 output control |

### 5. General-Purpose I/O (LEDs, Relays)
| Component | Quantity | Part Number | Description | Package | Notes |
|-----------|----------|-------------|-------------|---------|-------|
| GPIO Buffer | 1 | ULN2003A | Darlington array | DIP-16 | For relay/LED drive |
| Status LEDs | 4 | LTL-307EE | Indicator LEDs | 3mm | System status display |
| LED Resistors | 4 | - | 330Ω-1kΩ | 0805 | Current limiting |
| TVS Protection | 2 | USBLC6-2 | ESD clamp | SOT-23-6 | USB and I/O protection |

### 6. Power Supply and Regulation
| Component | Quantity | Part Number | Description | Package | Notes |
|-----------|----------|-------------|-------------|---------|-------|
| 5V Regulator | 1 | LMR33630 | Buck converter | SOIC-8 | From 12-36V input |
| 12V Regulator | 1 | L78M12 | Linear reg (if needed) | TO-220 | For 12V PWM rail |
| Input Filter Cap | 2 | - | 100µF 63V | Electrolytic | Input voltage smoothing |
| Bypass Caps | 15 | - | 100nF | 0805 | Decoupling capacitors |
| TVS Input | 1 | SMAJ48A | Input protection | SMA | Reverse/surge protection |
| Reverse Protection | 1 | FDS4435 | P-channel MOSFET | SOIC-8 | Input polarity protection |

### 7. Connectors and Headers
| Component | Quantity | Type | Description | Notes |
|-----------|----------|------|-------------|-------|
| Pico2 Headers | 2 | 2.54mm pin socket | 20-pin sockets | For removable Pico2 |
| Motor Connectors | 1 | IDC 2x6 | DF11-12DP | Ribbon cable connection |
| Endstop Headers | 5 | JST-XH 3-pin | B3B-XH-A | Limit switch connections |
| PWM Headers | 3 | JST-XH 3-pin | B3B-XH-A | Servo, spindle, coolant |
| CNC Output Headers | 2 | JST-XH 3-pin | B3B-XH-A | M7/M62 relay controls |
| UART Header | 1 | JST-XH 4-pin | B4B-XH-A | Serial communication/debug |
| Expansion Headers | 2 | JST-XH 3-pin | B3B-XH-A | GPIO 27/28 spare pins |
| Power Input | 1 | XT30 or screw | Terminal block | Low-profile power |
| Programming | 1 | Tag-Connect | TC2030-IDC | For SWD debugging |

## Current Status

- [ ] Schematic design
- [ ] Component selection and sourcing
- [ ] PCB layout (optimized for grouped signals)
- [ ] Design review
- [ ] Prototype manufacturing
- [ ] Testing and validation

## Design Files

- **Schematic**: `schematic/controller_board.sch`
- **PCB Layout**: `pcb_layout/controller_board.kicad_pcb`
- **3D Models**: `3d_models/` (custom components)
- **Assembly**: `assembly/assembly_drawing.pdf`

## Testing Plan

### Phase 1: Power and Basic Function
1. **Power-on Test**: Verify all voltage rails (3.3V, 5V, 12V, 24V)
2. **Pico2 Connection**: Confirm header connections and USB communication
3. **Regulator Test**: Load testing of buck converters and linear regulators

### Phase 2: Signal Isolation
4. **Digital Isolator Test**: Verify 3.3V → 5V isolation and signal integrity
5. **Opto-isolator Test**: Test 12V/24V input isolation and response
6. **PWM Isolation**: Confirm PWM signal transmission through isolators

### Phase 3: Motor Interface
7. **Step/Direction Signals**: Oscilloscope verification of output timing
8. **PIO Timing Test**: Verify grblHAL PIO generates correct step patterns
9. **Load Testing**: Test outputs with actual stepper drivers connected

### Phase 4: PWM and CNC Features
10. **Servo Control**: Test 50Hz PWM servo positioning
11. **Spindle PWM**: Verify variable frequency spindle control (1-25kHz)
12. **Coolant Control**: Test independent coolant/fan PWM output

### Phase 5: CNC Control and Safety Systems  
13. **Emergency Stop**: Hardware and software emergency stop testing
14. **Endstop Response**: Verify limit switch inputs with actual switches
15. **M-Code Outputs**: Test M7 (coolant mist) and M62 (general output) functions
16. **Relay Control**: Verify output drivers and relay switching

### Phase 6: Integration
17. **Communication**: UART bridge testing with ROS2 integration
18. **Full System**: Complete robot arm control testing
19. **Safety Validation**: Comprehensive safety system verification

## Manufacturing Notes

- **PCB Layers**: 4-layer recommended for proper power distribution
- **Board Thickness**: 1.6mm standard
- **Surface Finish**: ENIG for fine pitch components
- **Assembly**: SMT components on top, through-hole connectors
- **Signal Routing**: Step/direction pairs for minimal via usage and optimal trace matching
- **RP2350B Specific**: Use 4.7kΩ (or smaller) pull-down resistors for E9 erratum compliance
- **Power Design**: Size 3.3V supply for 250mA to handle RP2350B performance requirements
- **PWM Conflicts**: Document slice sharing for M-code outputs vs step signals
- **Trace Matching**: Keep step/dir pairs within 0.1mm length matching

