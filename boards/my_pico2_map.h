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
