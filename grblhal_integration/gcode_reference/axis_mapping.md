# Robot Arm Axis Mapping: Joints to G-code

This document describes how the physical robot arm joints map to grblHAL axes and G-code commands.

## Physical Robot Arm Layout

```
                    🏠 Base (J1)
                     │
              ┌──────┴──────┐ Shoulder (J2)
              │             │
              │      ┌──────┴──────┐ Elbow (J3)
              │      │             │
              │      │    ┌────────┴─────┐ Linear Act. 1 (J4)
              │      │    │              │
              │      │    │    ┌─────────┴──┐ Linear Act. 2 (J5)
              │      │    │    │            │
              │      │    │    │   ╔════════╪═══╗ End Effector (J6)
              │      │    │    │   ║        │   ║
              └──────┴────┴────┴───╨────────┴───╜
```

## Joint to G-code Axis Mapping

| Physical Joint | Type | grblHAL Axis | G-code Letter | Units | Range | Description |
|----------------|------|-------------|---------------|--------|-------|-------------|
| **J1** | Stepper | X | `X` | degrees | -360° to +360° | Base rotation (continuous) |
| **J2** | Stepper | Y | `Y` | degrees | 0° to 180° | Shoulder elevation |
| **J3** | Stepper | Z | `Z` | degrees | 0° to 180° | Elbow flexion |
| **J4** | Stepper | A | `A` | mm | 0 to 100mm | Linear actuator 1 extension |
| **J5** | Stepper | B | `B` | mm | 0 to 100mm | Linear actuator 2 extension |
| **J6** | Servo | C | `C` + `S` | degrees | -180° to +180° | End effector rotation |

## G-code Command Structure

### Basic Movement Command
```gcode
G1 X<base> Y<shoulder> Z<elbow> A<linear1> B<linear2> C<end_eff> F<feedrate>
```

### Servo Control (Joint 6)
For servo control, we use both the `C` axis and `S` parameter:
- `C`: Target angle in degrees
- `S`: Servo speed/pulse width (optional)

```gcode
G1 C90 S1500    ; Move end effector to 90 degrees with specific speed
```

## Coordinate Systems

### Machine Coordinates (G53)
- **Origin**: Physical home position of each joint
- **Units**: As defined in axis mapping table
- **Reference**: Absolute encoder/limit switch positions

### Work Coordinates (G54-G59)
- **Origin**: User-defined reference frame
- **Units**: Same as machine coordinates  
- **Reference**: Relative to workpiece or task-specific datum

## Example G-code Commands

### 1. Home All Axes
```gcode
$H                  ; Execute homing cycle for all axes
```

### 2. Move to Ready Position
```gcode
G21                 ; Set units to millimeters (for linear axes)
G90                 ; Absolute positioning mode
G94                 ; Feed rate in units per minute
G1 X0 Y90 Z45 A50 B50 C0 F1000    ; Move to ready position
```

### 3. Pick and Place Sequence
```gcode
; Move to approach position
G1 X45 Y60 Z30 A75 B25 C0 F2000
; Descend to pick position  
G1 A90 B10 F500
; Rotate end effector to grip
G1 C90 F100
; Lift object
G1 A50 B50 F500
; Move to place position
G1 X-45 Y80 Z60 F2000
; Place object
G1 A85 B15 F500
; Release grip
G1 C0 F100
; Retract
G1 A50 B50 F1000
```

### 4. Trajectory with Multiple Points
```gcode
G1 X0 Y90 Z45 A50 B50 C0 F1000      ; Point 1
G1 X30 Y75 Z60 A60 B40 C45 F1500    ; Point 2  
G1 X60 Y60 Z75 A70 B30 C90 F1500    ; Point 3
G1 X30 Y45 Z90 A80 B20 C45 F1000    ; Point 4
G1 X0 Y90 Z45 A50 B50 C0 F1000      ; Return to start
```

## Custom G-codes for Robot Arm

### M10 - Joint Position Move
Direct joint space movement (bypasses kinematics):
```gcode
M10 J1=45 J2=90 J3=60 J4=75 J5=25 J6=0 F1000
```

### M11 - Get Joint Positions  
Query current joint positions:
```gcode
M11                 ; Returns: J1:45.0 J2:90.0 J3:60.0 J4:75.0 J5:25.0 J6:0.0
```

### M12 - Set Servo Position
Direct servo control with fine adjustment:
```gcode
M12 S1500          ; Set servo pulse width directly (1000-2000μs)
```

### M13 - Emergency Stop
Immediate stop of all motion:
```gcode
M13                ; Emergency stop - all axes halt immediately
```

### M14 - Home Individual Axis
Home specific axis:
```gcode
M14 X              ; Home X axis only
M14 Y Z            ; Home Y and Z axes
```

## Coordinate Transformation

### ROS2 Joint Angles → G-code
```python
def joints_to_gcode(joint_angles, feedrate=1000):
    """
    Convert ROS2 joint angles to G-code command
    
    Args:
        joint_angles: [j1, j2, j3, j4, j5, j6] in degrees/mm
        feedrate: Movement speed in units/minute
    
    Returns:
        G-code string
    """
    return f"G1 X{joint_angles[0]:.3f} Y{joint_angles[1]:.3f} " \
           f"Z{joint_angles[2]:.3f} A{joint_angles[3]:.3f} " \
           f"B{joint_angles[4]:.3f} C{joint_angles[5]:.3f} F{feedrate}"

# Example usage:
joint_positions = [45.0, 90.0, 60.0, 75.5, 25.0, 0.0]
gcode_cmd = joints_to_gcode(joint_positions, 1500)
# Result: "G1 X45.000 Y90.000 Z60.000 A75.500 B25.000 C0.000 F1500"
```

### G-code Response → ROS2 Joint States
```python
def parse_grbl_status(status_line):
    """
    Parse grblHAL status report to extract joint positions
    
    Args:
        status_line: grblHAL status string like "<Idle|MPos:1.000,2.000,3.000,4.000,5.000,6.000|FS:0,0>"
    
    Returns:
        Joint positions array
    """
    if 'MPos:' in status_line:
        # Extract position values
        start = status_line.find('MPos:') + 5
        end = status_line.find('|', start)
        pos_str = status_line[start:end]
        positions = [float(x) for x in pos_str.split(',')]
        return positions
    return None
```

## Safety Considerations

### Soft Limits
Each axis has software-enforced limits defined in `grblhal_config.h`:
```c
#define X_MAX_TRAVEL 360.0      // Base: ±360 degrees
#define Y_MIN_TRAVEL 0.0        // Shoulder: 0-180 degrees  
#define Z_MIN_TRAVEL 0.0        // Elbow: 0-180 degrees
// ... etc
```

### Feed Rate Limits
Maximum safe speeds for each axis:
- **Rotational joints**: 3000 deg/min (50 deg/sec)
- **Linear actuators**: 1000 mm/min (16.7 mm/sec)
- **Servo**: Controlled by pulse width timing

### Emergency Stops
Multiple emergency stop methods available:
1. **Hardware E-stop**: Physical button cuts motor power
2. **Software E-stop**: `M13` G-code command  
3. **Feed hold**: `!` real-time command (pause motion)
4. **Soft reset**: `Ctrl+X` (reset controller)

## Units and Scaling

### Linear Units (Linear Actuators)  
- **Native units**: Millimeters
- **Lead screw pitch**: 4mm/revolution
- **Microstepping**: 1/16 (typical)
- **Steps per mm**: 800 steps/mm (3200 steps/rev ÷ 4mm/rev)
- **Resolution**: 0.00125mm/step (excellent precision!)
- **Example**: 2mm move = 1600 steps

### Angular Units (Rotational Joints)
- **Native units**: Degrees
- **Motor**: 200 steps/rev (1.8° per step)
- **Microstepping**: 1/16 
- **Steps per degree**: 8.89 steps/degree (3200 steps/rev ÷ 360°)
- **Resolution**: 0.1125°/step (very good precision)
- **Example**: 45° rotation = 400 steps

### Servo Units (End Effector)
- **Position units**: Degrees (-180° to +180°)
- **Pulse width**: 1000-2000μs (1-2ms)
- **Resolution**: ~0.18°/μs for 180° range

## Troubleshooting Common Issues

### 1. Servo Not Responding
Check PWM signal with oscilloscope:
- **Frequency**: Should be 50Hz (20ms period)
- **Pulse width**: 1-2ms range
- **Voltage**: 3.3V or 5V depending on servo

### 2. Stepper Missing Steps
Monitor step/direction signals:
- **Step pulse width**: Minimum 10μs
- **Direction setup time**: Minimum 5μs before step
- **Maximum step rate**: Verify below motor specifications

### 3. Limit Switch False Triggers
Check electrical noise on limit switch inputs:
- **Use shielded cables** for limit switches  
- **Add debouncing** in software (default 25ms)
- **Check voltage levels** (3.3V logic on RP2350)

This mapping provides the foundation for translating between ROS2 joint space control and grblHAL G-code execution.