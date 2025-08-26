#pragma once

// ----- Axes & pins (contiguous STEP pins for PIO range) -----
#define N_AXIS             3

#define STEP_PORT          GPIO_PIO        // <-- use PIO (single SM, multi-pin)
#define STEP_PINS_BASE     2               // STEP on 2,3,4
#define X_STEP_PIN         2
#define Y_STEP_PIN         3
#define Z_STEP_PIN         4

#define X_DIRECTION_PIN    7
#define Y_DIRECTION_PIN    8
#define Z_DIRECTION_PIN    9

#define STEPPERS_ENABLE_PIN 12

// Optional: limits (pick anything unused)
#define X_LIMIT_PIN        15
#define Y_LIMIT_PIN        17
#define Z_LIMIT_PIN        18

// Spindle PWM (optional, not on STEP range):
#define SPINDLE_PWM_PIN    14

// ----- Misc -----
#define BOARD_NAME         "Pico2-PIO-Contig"

// Add missing port definitions
#define X_DIRECTION_PORT   GPIO_OUTPUT
#define Y_DIRECTION_PORT   GPIO_OUTPUT
#define Z_DIRECTION_PORT   GPIO_OUTPUT
#define STEPPERS_ENABLE_PORT GPIO_OUTPUT

// Add required safety inputs
#define PROBE_PIN         19
#define SAFETY_DOOR_PIN   20
#define MOTOR_FAULT_PIN   21
#define MOTOR_WARNING_PIN 22

// Add spindle control
#define SPINDLE_ENABLE_PIN 23
#define SPINDLE_DIR_PIN    24

// Add coolant control
#define COOLANT_FLOOD_PIN  25
#define COOLANT_MIST_PIN   26

// Add control inputs
#define CONTROL_RESET_PIN  27
#define CONTROL_FEED_HOLD_PIN 28
#define CONTROL_CYCLE_START_PIN 29

// Add status LED
#define STATUS_LED_PIN     30
