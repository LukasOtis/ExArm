#ifndef GRBLHAL_CONFIG_H
#define GRBLHAL_CONFIG_H

/*
 * grblHAL Configuration for 5-6 DOF Robot Arm
 * Target: Raspberry Pi Pico 2 (RP2350)
 * Architecture: 5x Stepper Motors + 1x PWM Servo
 */

// ============================================================================
// BOARD CONFIGURATION
// ============================================================================

// Enable RP2350 support
#define BOARD_NAME "Robot Arm Controller"
#define BOARD_URL "https://github.com/your-repo/robot_arm_control"

// Number of axes
#define N_AXIS 6                    // Total axes (5 steppers + 1 servo)
#define N_STEPPER_MOTORS 5          // Number of stepper motors
#define N_SERVO_MOTORS 1            // Number of servo motors

// ============================================================================
// AXIS CONFIGURATION
// ============================================================================

// Axis definitions
#define X_AXIS 0    // Joint 1 - Base rotation (stepper)
#define Y_AXIS 1    // Joint 2 - Shoulder (stepper)  
#define Z_AXIS 2    // Joint 3 - Elbow (stepper)
#define A_AXIS 3    // Joint 4 - Linear actuator 1 (stepper)
#define B_AXIS 4    // Joint 5 - Linear actuator 2 (stepper)
#define C_AXIS 5    // Joint 6 - End effector rotation (servo)

// Motor types per axis
#define X_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define Y_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define Z_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define A_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define B_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define C_MOTOR_TYPE MOTOR_TYPE_SERVO

// ============================================================================
// STEPPER MOTOR SETTINGS
// ============================================================================

// Default steps per unit (steps/degree for rotational, steps/mm for linear)
// Assuming 1.8° stepper (200 steps/rev) with 1/16 microstepping = 3200 steps/rev
#define DEFAULT_X_STEPS_PER_UNIT 17.78     // Base rotation: 3200 steps/rev ÷ 180° = 17.78 steps/degree
#define DEFAULT_Y_STEPS_PER_UNIT 17.78     // Shoulder: 3200 steps/rev ÷ 180° = 17.78 steps/degree
#define DEFAULT_Z_STEPS_PER_UNIT 17.78     // Elbow: 3200 steps/rev ÷ 180° = 17.78 steps/degree  
#define DEFAULT_A_STEPS_PER_UNIT 800.0     // Linear actuator 1: 3200 steps/rev ÷ 4mm/rev = 800 steps/mm
#define DEFAULT_B_STEPS_PER_UNIT 800.0     // Linear actuator 2: 3200 steps/rev ÷ 4mm/rev = 800 steps/mm

// Maximum feed rates (units/minute) - Conservative values for 5V steppers
#define DEFAULT_X_MAX_RATE 1800.0          // degrees/min (30 deg/sec max)
#define DEFAULT_Y_MAX_RATE 1800.0          // degrees/min (30 deg/sec max)
#define DEFAULT_Z_MAX_RATE 1800.0          // degrees/min (30 deg/sec max)
#define DEFAULT_A_MAX_RATE 600.0           // mm/min (10 mm/sec max)
#define DEFAULT_B_MAX_RATE 600.0           // mm/min (10 mm/sec max)

// Acceleration (units/sec^2) - Start conservative, tune later
#define DEFAULT_X_ACCELERATION 60.0        // degrees/sec^2
#define DEFAULT_Y_ACCELERATION 60.0        // degrees/sec^2
#define DEFAULT_Z_ACCELERATION 60.0        // degrees/sec^2
#define DEFAULT_A_ACCELERATION 30.0        // mm/sec^2
#define DEFAULT_B_ACCELERATION 30.0        // mm/sec^2

// Stepper timing for 5V drivers
#define STEP_PULSE_DELAY 10                // Step pulse delay in microseconds (for 5V drivers)
#define STEP_PULSE_TIME 10                 // Step pulse width in microseconds
#define DIRECTION_DELAY 20                 // Direction signal setup time (microseconds)

// ============================================================================
// PIN ASSIGNMENTS (RP2350) - Updated for 5V Logic Level
// ============================================================================

// Stepper motor step pins
#define X_STEP_PIN      2
#define Y_STEP_PIN      4  
#define Z_STEP_PIN      6
#define A_STEP_PIN      8
#define B_STEP_PIN      10

// Stepper motor direction pins
#define X_DIRECTION_PIN 3
#define Y_DIRECTION_PIN 5
#define Z_DIRECTION_PIN 7
#define A_DIRECTION_PIN 9
#define B_DIRECTION_PIN 11

// Stepper motor enable pin (shared) - IMPORTANT: May need level shifter for 5V
#define STEPPERS_ENABLE_PIN 12

// NOTE: 3.3V to 5V Level Shifting Required!
// The RP2350 outputs 3.3V logic, but your stepper drivers expect 5V
// Options:
// 1. Use 74HCT245 or similar level shifter IC
// 2. Use stepper drivers that accept 3.3V input (check datasheet)
// 3. Use pull-up resistors to 5V on driver inputs

// Servo PWM pin (3.3V is usually fine for servo signal)
#define C_SERVO_PIN     13

// Limit switch pins (with pull-up resistors for mechanical switches)
#define X_LIMIT_PIN     14
#define Y_LIMIT_PIN     15
#define Z_LIMIT_PIN     16
#define A_LIMIT_PIN     17
#define B_LIMIT_PIN     18
#define C_LIMIT_PIN     19  // Optional for servo

// Safety pins
#define SAFETY_DOOR_PIN     20
#define EMERGENCY_STOP_PIN  21
#define CYCLE_START_PIN     22
#define FEED_HOLD_PIN       23

// Status pins
#define STATUS_LED_PIN      25  // Built-in LED
#define ALARM_LED_PIN       24

// Power control pins (NEW - for 5V power management)
#define STEPPER_POWER_PIN   26  // Control 5V power to steppers
#define SERVO_POWER_PIN     27  // Control 5V power to servo (if needed)

// ============================================================================
// LIMIT SWITCH CONFIGURATION
// ============================================================================

// Mechanical limit switches configuration
#define LIMITS_INVERT_MASK          0       // Normal high (pulled up)
#define LIMIT_SWITCH_DEBOUNCE_MS    50      // Debounce time for mechanical switches
#define ENABLE_LIMITS_INPUT                 // Enable limit switch inputs

// Pull-up configuration for mechanical switches
#define LIMITS_PULLUP_DISABLE_MASK  0       // Enable pull-ups on all limit pins

// Homing configuration for mechanical switches
#define HOMING_INIT_LOCK                    // Require unlocking before homing
#define HOMING_FORCE_SET_ORIGIN             // Set origin after successful homing

// ============================================================================
// SAFETY AND LIMITS - Updated for 4mm/rev lead screws
// ============================================================================

// Soft limits (in machine coordinates)
#define X_MAX_TRAVEL 360.0      // Base: full rotation capability
#define X_MIN_TRAVEL -360.0
#define Y_MAX_TRAVEL 180.0      // Shoulder: 0-180 degrees
#define Y_MIN_TRAVEL 0.0
#define Z_MAX_TRAVEL 180.0      // Elbow: 0-180 degrees  
#define Z_MIN_TRAVEL 0.0
#define A_MAX_TRAVEL 120.0      // Linear actuator 1: 0-120mm (30 revolutions × 4mm)
#define A_MIN_TRAVEL 0.0
#define B_MAX_TRAVEL 120.0      // Linear actuator 2: 0-120mm (30 revolutions × 4mm)
#define B_MIN_TRAVEL 0.0
#define C_MAX_TRAVEL 180.0      // End effector: ±90 degrees
#define C_MIN_TRAVEL -180.0

// Updated homing settings for your configuration
#define HOMING_CYCLE_0 (1<<A_AXIS)                    // Home linear actuators first (safest)
#define HOMING_CYCLE_1 (1<<B_AXIS)                    // Then second linear actuator  
#define HOMING_CYCLE_2 (1<<Z_AXIS)                    // Then elbow
#define HOMING_CYCLE_3 (1<<Y_AXIS)                    // Then shoulder
#define HOMING_CYCLE_4 (1<<X_AXIS)                    // Base last (heaviest load)
#define HOMING_CYCLE_5 (1<<C_AXIS)                    // Servo last (if limit switch present)

// Homing speeds and distances
#define DEFAULT_HOMING_SEEK_RATE    600.0   // Fast homing speed (mm/min or deg/min)
#define DEFAULT_HOMING_FEED_RATE    60.0    // Slow homing speed for precision
#define DEFAULT_HOMING_DEBOUNCE     250     // Homing switch debounce time (ms)
#define DEFAULT_HOMING_PULLOFF      2.0     // Distance to back off after homing (mm or deg)

// ============================================================================
// SERVO MOTOR SETTINGS
// ============================================================================

// Servo PWM settings
#define SERVO_PWM_FREQUENCY 50             // 50Hz standard servo frequency
#define SERVO_MIN_PULSE_US  1000           // 1ms minimum pulse width
#define SERVO_MAX_PULSE_US  2000           // 2ms maximum pulse width
#define SERVO_PERIOD_US     20000          // 20ms period (50Hz)

// Servo range mapping
#define SERVO_MIN_ANGLE     -90.0          // Minimum servo angle (degrees)
#define SERVO_MAX_ANGLE     90.0           // Maximum servo angle (degrees)

// ============================================================================
// FEATURES ENABLE/DISABLE
// ============================================================================

// Core features
#define ENABLE_PARKING_OVERRIDE_CONTROL
#define ENABLE_TOOL_LENGTH_OFFSET
#define ENABLE_VARIABLE_SPINDLE
#define ENABLE_COOLANT_CONTROL

// Advanced features  
#define ENABLE_BACKLASH_COMPENSATION
#define ENABLE_GANGED_AXES
#define ENABLE_DUAL_AXIS

// Debugging and development
#define ENABLE_DEBUG_OUTPUT
#define ENABLE_STEP_PULSE_DELAY

// ============================================================================
// CUSTOM ROBOT ARM FEATURES
// ============================================================================

// Enable custom robot arm specific functions
#define ENABLE_ROBOT_ARM_MODE
#define ENABLE_JOINT_INTERPOLATION
#define ENABLE_SERVO_CONTROL
#define ENABLE_TRAJECTORY_SMOOTHING

// Custom G-codes for robot arm control
#define ENABLE_CUSTOM_GCODE_M10     // Joint position move
#define ENABLE_CUSTOM_GCODE_M11     // Get joint positions
#define ENABLE_CUSTOM_GCODE_M12     // Set servo position
#define ENABLE_CUSTOM_GCODE_M13     // Emergency stop
#define ENABLE_CUSTOM_GCODE_M14     // Home all axes

#endif // GRBLHAL_CONFIG_H