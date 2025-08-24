# Mechanical Design Files

This directory contains mechanical design files for the ExArm robot controller hardware, including enclosures, mounting solutions, and connector specifications.

## Directory Contents

### Enclosures
**Location**: `enclosures/`
**Purpose**: Protective housings for controller boards and electronics

**Design Files**:
- Controller board enclosure
- Interface board housing
- Integrated system enclosure
- Ventilation and cooling solutions

**File Formats**:
- `.step` - CAD interchange format
- `.stl` - 3D printing files
- `.dwg` - 2D technical drawings
- `.pdf` - Assembly instructions

### Mounting Hardware
**Location**: `mounting/`
**Purpose**: Brackets and hardware for system installation

**Components**:
- PCB mounting standoffs and spacers
- System mounting brackets
- Cable management solutions
- Vibration dampening mounts

### Connector Specifications
**Location**: `connectors/`
**Purpose**: Physical connector definitions and specifications

**Documentation**:
- Connector pinout diagrams
- Physical dimensions and tolerances
- Mating connector specifications
- Cable assembly instructions

## Design Requirements

### Environmental Protection
- **IP Rating**: IP20 minimum (finger-safe, dust protection)
- **Operating Temperature**: 0°C to +60°C
- **Storage Temperature**: -20°C to +80°C
- **Humidity**: 5% to 95% non-condensing

### Physical Constraints
- **Size**: Compact design for robotic applications
- **Weight**: Minimize weight for mobile platforms
- **Accessibility**: Easy access to connectors and indicators
- **Serviceability**: Tool-free access for maintenance

### Materials
- **Enclosures**: ABS or PC plastic for impact resistance
- **Mounting**: Aluminum or steel for strength
- **Gaskets**: Silicone or EPDM for environmental sealing
- **Hardware**: Stainless steel fasteners for corrosion resistance

## Design Standards

### Mechanical Design
- **Tolerances**: ±0.1mm for critical dimensions
- **Wall Thickness**: 2mm minimum for structural integrity
- **Corner Radius**: 2mm minimum for manufacturability
- **Draft Angles**: 1° minimum for molded parts

### Fasteners
- **Standard Sizes**: M3, M4, M5 metric fasteners
- **Thread Engagement**: 1.5x diameter minimum
- **Torque Specifications**: Per ISO 4762 standards
- **Anti-seize**: Recommended for dissimilar metals

### Surface Finish
- **Molded Parts**: SPI A2 finish (600 grit equivalent)
- **Machined Parts**: Ra 3.2μm maximum
- **Painted Parts**: Two-coat system with primer
- **Anodized Parts**: Type II anodizing, clear or black

## Manufacturing Methods

### 3D Printing (Prototypes)
- **Technology**: FDM or SLA for different requirements
- **Materials**: PLA for prototypes, PETG for functional parts
- **Layer Height**: 0.2mm for good surface finish
- **Infill**: 20% for prototypes, 50% for functional parts

### Injection Molding (Production)
- **Materials**: ABS, PC, or PC/ABS blend
- **Tooling**: Aluminum for low volume, steel for high volume
- **Cycle Time**: 30-60 seconds depending on part size
- **Quality**: Statistical process control for consistency

### CNC Machining (Metal Parts)
- **Materials**: 6061-T6 aluminum for brackets
- **Tolerances**: ±0.05mm for critical features
- **Surface Treatment**: Anodize or powder coat
- **Fixturing**: Proper work holding for accuracy

## Assembly Considerations

### PCB Mounting
- **Standoffs**: Nylon or metal standoffs for electrical isolation
- **Strain Relief**: Prevent stress on solder joints
- **Thermal Management**: Consider heat dissipation requirements
- **Access**: Maintain access to programming and debug connectors

### Cable Management
- **Entry Points**: Sealed cable glands or grommets
- **Routing**: Avoid sharp edges and pinch points
- **Strain Relief**: Adequate support for all cables
- **Separation**: Keep power and signal cables separated

### Maintenance Access
- **Removable Panels**: Easy access to serviceable components
- **Clear Markings**: Labels for all connectors and functions
- **Tool Requirements**: Standard tools only (Phillips, hex keys)
- **Safety**: Lock-out/tag-out compatibility for maintenance

## Testing and Validation

### Mechanical Testing
- **Drop Test**: 1 meter drop onto concrete
- **Vibration Test**: Per IEC 60068-2-6
- **Temperature Cycling**: -20°C to +80°C, 100 cycles
- **Humidity Test**: 95% RH at 40°C for 48 hours

### Fit and Finish
- **Dimensional Check**: All critical dimensions verified
- **Gap and Flush**: Consistent gaps and alignment
- **Surface Quality**: No visual defects or sharp edges
- **Function Test**: All latches and mechanisms operate smoothly

## Documentation Standards

### Technical Drawings
- **Standards**: ASME Y14.5 or ISO 1101 for GD&T
- **Views**: Sufficient views to fully define the part
- **Dimensions**: All critical dimensions and tolerances
- **Notes**: Material, finish, and special requirements

### Assembly Instructions
- **Step-by-Step**: Clear sequence with illustrations
- **Tools Required**: List of all required tools
- **Torque Values**: Specified for all fasteners
- **Safety Notes**: Warnings and precautions

### Bill of Materials
- **Part Numbers**: Unique identifier for each component
- **Descriptions**: Clear description including material
- **Quantities**: Accurate quantities per assembly
- **Sources**: Preferred suppliers and alternates

## Current Status

- [ ] Requirements definition complete
- [ ] Concept design and layout
- [ ] Detailed design and modeling
- [ ] Prototype manufacturing
- [ ] Testing and validation
- [ ] Production tooling
- [ ] Production qualification

## File Organization

```
mechanical/
├── enclosures/
│   ├── controller_enclosure/
│   ├── interface_enclosure/
│   └── system_enclosure/
├── mounting/
│   ├── pcb_mounts/
│   ├── system_brackets/
│   └── cable_management/
└── connectors/
    ├── pinout_diagrams/
    ├── physical_specs/
    └── cable_assemblies/
```

Each subdirectory contains:
- CAD source files (native format)
- STEP files for interchange
- STL files for 3D printing
- Technical drawings (PDF)
- Assembly instructions
- Bill of materials 