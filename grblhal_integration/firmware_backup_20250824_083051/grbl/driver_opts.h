/*
  driver_opts.h - Driver options for grblHAL Robot Arm on Pico
  
  This file defines which features are enabled/disabled for the robot arm build.
  It should be included before driver.h
*/

#ifndef DRIVER_OPTS_H
#define DRIVER_OPTS_H

// ============================================================================
// COMMUNICATION OPTIONS
// ============================================================================

// Serial communication
#define SERIAL_ENABLE 1
#define USB_SERIAL_ENABLE 1
#define UART_ENABLE 1

// Network options (disabled for robot arm)
#define ETHERNET_ENABLE 0
#define WIFI_ENABLE 0
#define BLUETOOTH_ENABLE 0

// ============================================================================
// MOTION CONTROL OPTIONS
// ============================================================================

// Stepper motor control
#define STEPPERS_ENABLE 1
#define STEPPERS_ENABLE_PINMODE 0  // Direct GPIO control

// Servo motor control
#define SERVO_ENABLE 1
#define PWM_ENABLE 1

// Encoder support
#define ENCODER_ENABLE 1
#define ENCODER_AB_ENABLE 1

// ============================================================================
// I/O OPTIONS
// ============================================================================

// Digital I/O
#define DIGITAL_IO_ENABLE 1
#define ANALOG_IO_ENABLE 1

// Limit switches
#define LIMITS_ENABLE 1
#define LIMITS_2_ENABLE 1

// Safety features
#define SAFETY_DOOR_ENABLE 1
#define EMERGENCY_STOP_ENABLE 1

// ============================================================================
// CNC FEATURES (DISABLED FOR ROBOT ARM)
// ============================================================================

// Spindle control (not needed for robot arm)
#define SPINDLE_ENABLE 0
#define SPINDLE_TYPE 0

// Coolant control (not needed for robot arm)
#define COOLANT_ENABLE 0

// Tool change (not needed for robot arm)
#define TOOL_CHANGE_ENABLE 0

// Probe (not needed for robot arm)
#define PROBE_ENABLE 0

// ============================================================================
// ADVANCED FEATURES
// ============================================================================

// Real-time reporting
#define REAL_TIME_REPORTING_ENABLE 1

// Status reporting
#define STATUS_REPORT_ENABLE 1

// G-code parsing
#define GCODE_ENABLE 1

// Settings storage
#define SETTINGS_ENABLE 1
#define NVS_ENABLE 1

// ============================================================================
// ROBOT ARM SPECIFIC
// ============================================================================

// Robot arm mode
#define ROBOT_ARM_MODE 1

// Joint mode support
#define JOINT_MODE_ENABLE 1

// Cartesian mode support
#define CARTESIAN_MODE_ENABLE 1

// Kinematics support
#define KINEMATICS_ENABLE 1

// ============================================================================
// DEBUG AND DEVELOPMENT
// ============================================================================

#ifdef DEBUG
#define DEBUG_ENABLE 1
#define DEBUG_REPORT_ENABLE 1
#else
#define DEBUG_ENABLE 0
#define DEBUG_REPORT_ENABLE 0
#endif

// ============================================================================
// PLATFORM SPECIFIC
// ============================================================================

// Pico-specific features
#define PICO_ENABLE 1
#define RP2040_ENABLE 1

// PIO support
#define PIO_ENABLE 1

// DMA support
#define DMA_ENABLE 1

// Multicore support
#define MULTICORE_ENABLE 1

#endif // DRIVER_OPTS_H
