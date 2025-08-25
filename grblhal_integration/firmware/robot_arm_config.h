/*
 * grblHAL Configuration for 3-DOF Robot Arm (Phase 1)
 * Target: Raspberry Pi Pico (RP2040)
 * Architecture: 3x Stepper Motors + PWM Servo
 * 
 * NOTE: Limited to 3-axis due to RP2040 PIO constraints.
 * For 5-axis operation, upgrade to RP2350B (Pico 2) in Phase 5.
 */

#ifndef ROBOT_ARM_CONFIG_H
#define ROBOT_ARM_CONFIG_H

// ============================================================================
// BOARD CONFIGURATION
// ============================================================================

// Enable RP2040 support
// Note: BOARD_NAME is defined in my_machine_map.h
#define BOARD_URL "https://github.com/your-repo/robot_arm_control"

// Number of axes (3-axis for RP2040)
#define N_AXIS 3                    // Total axes (3 steppers)
#define N_STEPPER_MOTORS 3          // Number of stepper motors
#define N_SERVO_MOTORS 1            // Number of servo motors (via PWM)

// ============================================================================
// AXIS CONFIGURATION
// ============================================================================

// Axis definitions (3-axis for RP2040)
#define X_AXIS 0    // Joint 1 - Base rotation (stepper)
#define Y_AXIS 1    // Joint 2 - Shoulder (stepper)  
#define Z_AXIS 2    // Joint 3 - Elbow (stepper)

// Motor types per axis
#define X_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define Y_MOTOR_TYPE MOTOR_TYPE_STEPPER
#define Z_MOTOR_TYPE MOTOR_TYPE_STEPPER

// ============================================================================
// STEPPER MOTOR SETTINGS
// ============================================================================

// Default steps per unit (steps/degree for rotational joints)
// Assuming 1.8° stepper (200 steps/rev) with 1/16 microstepping = 3200 steps/rev
#define DEFAULT_X_STEPS_PER_UNIT 17.78     // Base rotation: 3200 steps/rev ÷ 180° = 17.78 steps/degree
#define DEFAULT_Y_STEPS_PER_UNIT 17.78     // Shoulder: 3200 steps/rev ÷ 90° = 35.56 steps/degree
#define DEFAULT_Z_STEPS_PER_UNIT 17.78     // Elbow: 3200 steps/rev ÷ 120° = 26.67 steps/degree

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
// FUTURE UPGRADE PATH (Phase 5)
// ============================================================================

/*
 * To upgrade to 5-axis operation:
 * 1. Replace RP2040 with RP2350B (Pico 2)
 * 2. Update RP_MCU to 2350
 * 3. Increase N_AXIS to 6
 * 4. Add A, B, C axis definitions
 * 5. Update pin assignments for additional axes
 * 6. Enable PIO step generation for 5-axis
 */

#endif // ROBOT_ARM_CONFIG_H
