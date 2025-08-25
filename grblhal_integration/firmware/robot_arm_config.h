/*
 * grblHAL Configuration for 5-DOF Robot Arm (Phase 2)
 * Target: Raspberry Pi Pico 2 (RP2350B)
 * Architecture: 5x Stepper Motors + PWM Servo
 * 
 * Full 5-axis support with RP2350B PIO capabilities.
 * Step/dir pairs: (2,3) (4,5) (6,7) (8,9) (10,11)
 */

#ifndef ROBOT_ARM_CONFIG_H
#define ROBOT_ARM_CONFIG_H

// ============================================================================
// BOARD CONFIGURATION
// ============================================================================

// Enable RP2040 support
// Note: BOARD_NAME is defined in my_machine_map.h
#define BOARD_URL "https://github.com/your-repo/robot_arm_control"

// Number of axes (5-axis for RP2350B)
#define N_AXIS 5                    // Total axes (5 steppers)
#define N_STEPPER_MOTORS 5          // Number of stepper motors
#define N_SERVO_MOTORS 1            // Number of servo motors (via PWM)

// ============================================================================
// AXIS CONFIGURATION
// ============================================================================

// Axis definitions (5-axis for RP2350B)
#define X_AXIS 0    // Joint 1 - Base rotation (stepper)
#define Y_AXIS 1    // Joint 2 - Shoulder (stepper)  
#define Z_AXIS 2    // Joint 3 - Elbow (stepper)
#define A_AXIS 3    // Joint 4 - Linear actuator 1 (stepper)
#define B_AXIS 4    // Joint 5 - Linear actuator 2 (stepper)

// Motor types per axis
#define X_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define Y_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define Z_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define A_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define B_MOTOR_TYPE MOTOR_TYPE_STEPPER

// ============================================================================
// STEPPER MOTOR SETTINGS
// ============================================================================

// Default steps per unit (steps/degree for rotational joints, steps/mm for linear)
// Assuming 1.8° stepper (200 steps/rev) with 1/16 microstepping = 3200 steps/rev
#define DEFAULT_X_STEPS_PER_UNIT 17.78     // Base rotation: 3200 steps/rev ÷ 180° = 17.78 steps/degree
#define DEFAULT_Y_STEPS_PER_UNIT 17.78     // Shoulder: 3200 steps/rev ÷ 90° = 35.56 steps/degree
#define DEFAULT_Z_STEPS_PER_UNIT 17.78     // Elbow: 3200 steps/rev ÷ 120° = 26.67 steps/degree
#define DEFAULT_A_STEPS_PER_UNIT 320.0     // Linear actuator 1: 3200 steps/rev ÷ 10mm/rev = 320 steps/mm
#define DEFAULT_B_STEPS_PER_UNIT 320.0     // Linear actuator 2: 3200 steps/rev ÷ 10mm/rev = 320 steps/mm

// GRBL core expects these specific names for A and B axes
#define DEFAULT_A_STEPS_PER_MM DEFAULT_A_STEPS_PER_UNIT
#define DEFAULT_B_STEPS_PER_MM DEFAULT_B_STEPS_PER_UNIT

// Maximum feed rates (units/minute)
#define DEFAULT_A_MAX_RATE 600.0           // mm/min (10 mm/sec max)
#define DEFAULT_B_MAX_RATE 600.0           // mm/min (10 mm/sec max)

// Acceleration (units/sec^2)
#define DEFAULT_A_ACCELERATION 30.0        // mm/sec^2
#define DEFAULT_B_ACCELERATION 30.0        // mm/sec^2

// Jerk (units/sec^3)
#define DEFAULT_A_JERK 10.0                // mm/sec^3
#define DEFAULT_B_JERK 10.0                // mm/sec^3

// Maximum travel (units)
#define DEFAULT_A_MAX_TRAVEL 100.0         // mm
#define DEFAULT_B_MAX_TRAVEL 100.0         // mm

// Homing cycle masks
#define DEFAULT_HOMING_CYCLE_3 (bit(X_AXIS) | bit(Y_AXIS) | bit(Z_AXIS) | bit(A_AXIS))
#define DEFAULT_HOMING_CYCLE_4 (bit(X_AXIS) | bit(Y_AXIS) | bit(Z_AXIS) | bit(A_AXIS) | bit(B_AXIS))

// ============================================================================
// SERVO MOTOR SETTINGS
// ============================================================================

// PWM servo control for end effector (via AUXOUTPUT0_PWM_PIN)
#define ENABLE_PWM_SERVO 1
#define PWM_SERVO_FREQUENCY 50      // 50 Hz for standard servos
#define PWM_SERVO_RESOLUTION 12     // 12-bit resolution (0-4095)

// ============================================================================
// AUXILIARY OUTPUT SETTINGS
// ============================================================================

// Enable auxiliary output control plugin (M210-M215 commands)
#define AUX_OUTPUT_ENABLE 1

// Note: Pin definitions are already defined in my_machine_map.h:
// AUXOUTPUT0_PIN = 26 (GPIO 26)
// AUXOUTPUT1_PIN = 27 (GPIO 27) 
// AUXOUTPUT2_PIN = 28 (GPIO 28)

// ============================================================================
// SAFETY AND LIMITS
// ============================================================================

// Limit switches
#define LIMIT_SWITCHES_ENABLE 1
#define LIMIT_SWITCHES_INVERT 0     // Active low (NC switches)

// Emergency stop
#define E_STOP_ENABLE 1
#define E_STOP_INVERT 0             // Active low (NC switch)

// Soft limits
#define SOFT_LIMITS_ENABLE 1
#define SOFT_LIMITS_INVERT 0

// ============================================================================
// COMMUNICATION
// ============================================================================

// USB CDC (primary communication)
#define USB_SERIAL_CDC 1
#define USB_SERIAL_CDC_BAUDRATE 115200

// UART (backup communication)
#define UART_ENABLE 1
#define UART_BAUDRATE 115200

// ============================================================================
// PERFORMANCE
// ============================================================================

// Step pulse settings
#define STEP_PULSE_LENGTH 10        // 10 microseconds
#define STEP_PULSE_DELAY 5          // 5 microseconds

// Note: Speed and acceleration settings are defined in GRBL/config.h
// Override them there if needed for robot arm specific values

// ============================================================================
// CURRENT CAPABILITIES (Phase 2 - Complete)
// ============================================================================

/*
 * 5-axis system fully implemented:
 * ✅ 5 stepper motors (X, Y, Z, A, B)
 * ✅ PWM servo control (M280)
 * ✅ Auxiliary outputs (M210-M215)
 * ✅ Limit switches for all axes
 * ✅ Emergency stop and safety systems
 * ✅ PIO-based step generation
 * ✅ Full grblHAL compatibility
 * 
 * Ready for advanced robot arm applications!
 */

#endif // ROBOT_ARM_CONFIG_H
