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
// PIN ASSIGNMENTS (RP2350)
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

// Stepper motor enable pin (shared)
#define STEPPERS_ENABLE_PIN 12

// Servo PWM pin
#define C_SERVO_PIN     13

// Limit switch pins
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

// Serial communication (USB)
// GPIO 0 and 1 are used for UART by default

// ============================================================================
// STEPPER MOTOR SETTINGS
// ============================================================================

// Default steps per unit (steps/degree for rotational, steps/mm for linear)
#define DEFAULT_X_STEPS_PER_UNIT 200.0    // Base rotation (steps/degree)
#define DEFAULT_Y_STEPS_PER_UNIT 200.0    // Shoulder (steps/degree)
#define DEFAULT_Z_STEPS_PER_UNIT 200.0    // Elbow (steps/degree)  
#define DEFAULT_A_STEPS_PER_UNIT 80.0     // Linear actuator 1 (steps/mm)
#define DEFAULT_B_STEPS_PER_UNIT 80.0     // Linear actuator 2 (steps/mm)

// Maximum feed rates (units/minute)
#define DEFAULT_X_MAX_RATE 3000.0         // degrees/min
#define DEFAULT_Y_MAX_RATE 3000.0         // degrees/min
#define DEFAULT_Z_MAX_RATE 3000.0         // degrees/min
#define DEFAULT_A_MAX_RATE 1000.0         // mm/min
#define DEFAULT_B_MAX_RATE 1000.0         // mm/min

// Acceleration (units/sec^2)
#define DEFAULT_X_ACCELERATION 100.0      // degrees/sec^2
#define DEFAULT_Y_ACCELERATION 100.0      // degrees/sec^2
#define DEFAULT_Z_ACCELERATION 100.0      // degrees/sec^2
#define DEFAULT_A_ACCELERATION 50.0       // mm/sec^2
#define DEFAULT_B_ACCELERATION 50.0       // mm/sec^2

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
// SAFETY AND LIMITS
// ============================================================================

// Soft limits (in machine coordinates)
#define X_MAX_TRAVEL 360.0      // Base: full rotation
#define X_MIN_TRAVEL -360.0
#define Y_MAX_TRAVEL 180.0      // Shoulder: 0-180 degrees
#define Y_MIN_TRAVEL 0.0
#define Z_MAX_TRAVEL 180.0      // Elbow: 0-180 degrees  
#define Z_MIN_TRAVEL 0.0
#define A_MAX_TRAVEL 100.0      // Linear actuator 1: 0-100mm
#define A_MIN_TRAVEL 0.0
#define B_MAX_TRAVEL 100.0      // Linear actuator 2: 0-100mm
#define B_MIN_TRAVEL 0.0
#define C_MAX_TRAVEL 180.0      // End effector: ±90 degrees
#define C_MIN_TRAVEL -180.0

// Homing settings
#define HOMING_CYCLE_0 (1<<Z_AXIS)                    // Home Z first (elbow)
#define HOMING_CYCLE_1 (1<<Y_AXIS)                    // Then Y (shoulder)
#define HOMING_CYCLE_2 (1<<X_AXIS)                    // Then X (base)
#define HOMING_CYCLE_3 ((1<<A_AXIS)|(1<<B_AXIS))     // Linear actuators together
#define HOMING_CYCLE_4 (1<<C_AXIS)                    // Servo last (if limit switch)

// Safety features
#define ENABLE_SAFETY_DOOR_INPUT
#define ENABLE_LIMITS_INPUT
#define ENABLE_CONTROL_INPUT

// ============================================================================
// COMMUNICATION SETTINGS
// ============================================================================

// Serial settings
#define BAUD_RATE 115200

// Status report mask (what to include in status reports)
#define STATUS_REPORT_MASK (BITFLAG_RT_STATUS_POSITION_TYPE | \
                           BITFLAG_RT_STATUS_BUFFER_STATE | \
                           BITFLAG_RT_STATUS_LINE_NUMBERS | \
                           BITFLAG_RT_STATUS_FEED_SPEED)

// Real-time report interval
#define STATUS_REPORT_INTERVAL_MS 100

// ============================================================================
// PERFORMANCE SETTINGS
// ============================================================================

// Stepper interrupt frequency
#define STEPPER_TIMER_RATE 30000    // 30kHz stepper interrupt rate

// Motion control settings
#define SEGMENT_BUFFER_SIZE 32      // Number of segments in buffer
#define ARC_ANGULAR_TRAVEL_EPSILON 5E-7
#define N_ARC_CORRECTION 12

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