# Hardware Requirements and Considerations

Based on your specifications: **5V stepper drivers**, **4mm/rev lead screws**, and **mechanical limit switches**.

## ⚠️ **CRITICAL: 3.3V to 5V Logic Level Issue**

**MAJOR CONCERN**: The RP2350 outputs 3.3V logic, but your stepper drivers expect 5V inputs!

### **Solutions (Choose One):**

#### **Option 1: Level Shifter IC (Recommended)**
Use 74HCT245 or similar for clean level conversion:
```
RP2350 (3.3V) → 74HCT245 → Stepper Drivers (5V)
```
- **Pros**: Clean, reliable signals; handles all pins
- **Cons**: Additional component cost (~$2)
- **Connections**: 
  - VCC = 5V
  - All step/direction signals through shifter

#### **Option 2: Pull-up Resistors**
Add 4.7kΩ pull-up resistors from each signal to 5V:
```
RP2350 Pin ──┬── Stepper Driver Input
             │
           4.7kΩ
             │
            5V
```
- **Pros**: Simple, cheap solution
- **Cons**: Slower rise times, may cause issues at high step rates
- **Use for**: Signals up to ~20kHz step rate

#### **Option 3: 3.3V Compatible Drivers**
Choose stepper drivers that accept 3.3V logic inputs:
- **Examples**: DRV8825 (works with 3.3V), TMC2208/2209
- **Check datasheet**: VIH (input high voltage) should be < 3.3V

## **Updated Motor Calculations**

### **Linear Actuators (4mm/rev lead screws)**
With 1/16 microstepping:
- **Steps per mm**: 3200 steps/rev ÷ 4mm/rev = **800 steps/mm**
- **Maximum speed**: ~10mm/sec (conservative for 5V steppers)
- **Resolution**: 0.00125mm/step (excellent precision!)

### **Rotational Joints**
With 1/16 microstepping:
- **Steps per degree**: 3200 steps/rev ÷ 360° = **8.89 steps/degree**
- **Resolution**: 0.1125°/step (very good precision)

## **Hardware Shopping List**

### **Essential Components**

#### **Level Shifting (Critical!)**
- **74HCT245** octal bus transceiver (2x required for 12 signals)
- **Alternative**: 74AHCT1G125 (single gate, need 12x)

#### **Power Supply**
- **5V, 10A minimum** for stepper drivers + servo
- **12V or 24V** for stepper motors (check driver requirements)
- **Separate 5V rail** for logic (don't use Pi's 5V for motors!)

#### **Connectors & Protection**
- **Schottky diodes** (1N5819) for back-EMF protection
- **Capacitors**: 100μF electrolytic + 0.1μF ceramic per driver
- **Emergency stop switch** (hardware power cutoff)

### **Mechanical Limit Switches**

#### **Switch Specifications**
- **Type**: SPDT microswitches (3 terminals)
- **Rating**: 5A/250VAC minimum
- **Actuator**: Roller lever or plunger type
- **Examples**: Omron SS-5GL, Cherry D44X

#### **Wiring for Mechanical Switches**
```
+3.3V ──┬──[4.7kΩ]──┬── RP2350 GPIO (limit input)
        │            │
        │           SW ── Common
        │            │
       NC            NO ── GND
```
- **Normal operation**: Switch open, input reads HIGH (3.3V)
- **Limit triggered**: Switch closes, input reads LOW (0V)
- **Debouncing**: 50ms in software (configured in grblHAL)

## **Pin Usage Analysis**

### **Current Pin Count: 24/26 Used**
```
Step signals:      5 pins (X,Y,Z,A,B)
Direction signals: 5 pins  
Enable (shared):   1 pin
Servo PWM:         1 pin
Limit switches:    6 pins
Safety/Control:    4 pins (E-stop, door, start, hold)
Status LEDs:       2 pins
────────────────────────
Total:            24 pins
Remaining:         2 pins
```

### **⚠️ Pin Shortage Concerns**
You're using **92% of available pins**! Consider:

1. **Shared enable**: Good choice, saves 4 pins
2. **Optional limit switches**: C-axis (servo) limit might not be needed
3. **Status LEDs**: Could use external driver if pins needed
4. **Emergency stop**: Could use interrupt-capable pin

## **Power Architecture**

### **Recommended Power Distribution**
```
Main PSU (24V/10A)
├── Buck converter → 5V/5A (steppers + servo logic)
├── Buck converter → 3.3V/1A (RP2350 + sensors)
└── Direct → 24V (stepper motor power)
```

### **Current Estimates**
- **Stepper drivers**: ~500mA each @ 5V = 2.5A total
- **Servo**: ~1A peak @ 5V
- **RP2350 + logic**: ~200mA @ 3.3V
- **Safety margin**: 2x power requirements

## **Safety Considerations**

### **Hardware Emergency Stop**
Implement **hardware power cutoff** for emergency stop:
```
E-Stop Switch (NC) ──┬── 5V to all motor drivers
                     └── Signal to RP2350 (for software awareness)
```

### **Limit Switch Reliability**
- **Use quality switches**: Industrial microswitches
- **Mounting**: Adjustable position for fine-tuning
- **Protection**: Dust covers in dirty environments
- **Backup**: Consider dual switches on critical axes

## **Oscilloscope Testing Points**

### **Step/Direction Signals**
- **Before level shifter**: 0-3.3V square waves
- **After level shifter**: 0-5V square waves
- **Step timing**: 10μs minimum pulse width
- **Direction setup**: 20μs before step pulse

### **Servo PWM Signal**
- **Frequency**: 50Hz (20ms period)
- **Pulse width**: 1-2ms (1.5ms = center)
- **Voltage**: 3.3V (should work for most servos)

### **Limit Switch Signals**
- **Idle state**: 3.3V (pulled up)
- **Triggered**: 0V (grounded)
- **Debouncing**: Check for clean transitions

## **Critical Questions for You**

### **1. Stepper Driver Selection**
- **What specific drivers** are you considering?
- **Do they accept 3.3V logic** or need 5V?
- **Current rating** requirements for your motors?

### **2. Power Supply**
- **Motor voltage**: 12V or 24V steppers?
- **Expected current** per motor?
- **Do you have** existing power supplies?

### **3. Mechanical Integration**
- **Limit switch mounting**: How will they be positioned?
- **Emergency stop**: Hardware cutoff or software only?
- **Cable management**: Shielded cables for step signals?

### **4. Performance Requirements**
- **Maximum speed**: How fast do linear actuators need to move?
- **Precision**: Is 0.00125mm resolution adequate?
- **Load requirements**: What forces will the actuators handle?

## **Next Steps Priority**

1. **🔴 FIRST**: Resolve the 3.3V/5V logic issue (choose solution)
2. **🟡 THEN**: Select compatible stepper drivers
3. **🟢 FINALLY**: Design PCB/breadboard layout with level shifters

**Would you like me to help you select specific stepper drivers that work with your setup, or design the level shifter circuit?**