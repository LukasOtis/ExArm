#pragma once

// Define board identifier for 4-axis configuration
#define BOARD_GENERIC_4AXIS

// Set number of axes to 4 (X, Y, Z, A)
#define N_AXIS                 4

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
#define PWM_SERVO_ENABLE        0
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
