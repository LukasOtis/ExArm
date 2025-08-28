#pragma once

// Define board identifier for 5-axis configuration
#define BOARD_ROBOTARM_5AXIS

// Set number of axes to 5 (X, Y, Z, A, B)
#define N_AXIS                 5

// Standardized 1:1 axis settings for ROS2 motion planning
// All axes use 1 step = 1mm for simple, predictable behavior
#define DEFAULT_X_STEPS_PER_MM  1.0f
#define DEFAULT_Y_STEPS_PER_MM  1.0f
#define DEFAULT_Z_STEPS_PER_MM  1.0f
#define DEFAULT_A_STEPS_PER_MM  1.0f
#define DEFAULT_B_STEPS_PER_MM  1.0f

// Standardized max travel limits for all axes (in mm)
// Set to reasonable values for a typical robot arm
#define DEFAULT_X_MAX_TRAVEL    500.0f
#define DEFAULT_Y_MAX_TRAVEL    500.0f
#define DEFAULT_Z_MAX_TRAVEL    500.0f
#define DEFAULT_A_MAX_TRAVEL    500.0f  
#define DEFAULT_B_MAX_TRAVEL    500.0f  

// Standardized max feed rates for all axes (in mm/min)
#define DEFAULT_X_MAX_RATE      1000.0f
#define DEFAULT_Y_MAX_RATE      1000.0f
#define DEFAULT_Z_MAX_RATE      1000.0f
#define DEFAULT_A_MAX_RATE      1000.0f
#define DEFAULT_B_MAX_RATE      1000.0f

// Basic configuration
#ifndef USB_SERIAL_CDC
#define USB_SERIAL_CDC          1 // Serial communication via native USB.
#endif

// Disable unnecessary features for minimal build
#define WIFI_ENABLE             0
#define ETHERNET_ENABLE         0
#define BLUETOOTH_ENABLE        0
#define WEBUI_ENABLE            0
#define SDCARD_ENABLE           0
#define DISPLAY_ENABLE          0
#define KEYPAD_ENABLE           0
#define MPG_ENABLE              0
#define EEPROM_ENABLE           0
#define RGB_LED_ENABLE          0
#define PWM_SERVO_ENABLE        1
#define N_PWM_SERVOS            3
// Enable built-in I/O M-codes (M64/M65, M67/M68)
#define IO_ENABLE               1
#define AUX_OUTPUT_GENERIC      1

// Disable legacy coolant/spindle features so pins are not claimed/labeled
#undef COOLANT_ENABLE
#define COOLANT_ENABLE          0
#undef SPINDLE_ENABLE
#define SPINDLE_ENABLE          0

// Force disable driver spindle functionality to prevent compilation errors
#undef DRIVER_SPINDLE_ENABLE
#define DRIVER_SPINDLE_ENABLE   0
#define BLTOUCH_ENABLE          0
#define EVENTOUT_ENABLE         0

// Optional control signals - disable for testing
#define PROBE_ENABLE            0
#define SAFETY_DOOR_ENABLE      0
#define MOTOR_FAULT_ENABLE      0
#define MOTOR_WARNING_ENABLE    0
#define PROBE_DISCONNECT_ENABLE 0
#define STOP_DISABLE_ENABLE     0
#define BLOCK_DELETE_ENABLE     0
#define SINGLE_BLOCK_ENABLE     0
#define LIMITS_OVERRIDE_ENABLE  0

// Disable control inputs to prevent alarms
#define CONTROL_ENABLE          0

// Disable limit switches for testing (they might be floating)
#define LIMITS_ENABLE           0
