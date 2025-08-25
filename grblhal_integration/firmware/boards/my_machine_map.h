/*
  my_machine_map.h - Board mapping for ExArm Pico2Breakout (RP2350B)

  Custom pin map aligned with hardware/pcb/controller_board README pinout.
  
  Full 5-axis support with RP2350B (Pico 2) PIO capabilities.
  Step/dir pairs are adjacent (step,dir) on GPIO: (2,3) (4,5) (6,7) (8,9) (10,11)
  Shared enable on GPIO 12.

  Using GPIO_OUTPUT for steps since PIO requires consecutive pins.
*/

#ifndef MY_MACHINE_MAP_H
#define MY_MACHINE_MAP_H

#include "driver.h"

// Support RP2040 (Pico) and RP2350 (Pico 2) - 3-axis for basic control
#if RP_MCU != 2040 && RP_MCU != 2350
#error "This board map targets RP2040/RP2350 processors (Pico/Pico 2)."
#endif

#define BOARD_NAME "ExArm Pico2Breakout (5-Axis)"

// -----------------------------
// Step/Dir/Enable (5-axis support)
// -----------------------------

#define STEP_PORT               GPIO_OUTPUT
#define DIRECTION_PORT          GPIO_OUTPUT
#define DIRECTION_OUTMODE       0
#define ENABLE_PORT             GPIO_OUTPUT

// Primary axes (X, Y, Z)
#define X_STEP_PIN              2
#define Y_STEP_PIN              4
#define Z_STEP_PIN              6

#define X_DIRECTION_PIN         3
#define Y_DIRECTION_PIN         5
#define Z_DIRECTION_PIN         7

// Secondary axes (A, B) - mapped to M3 and M4 for GRBL compatibility
#define M3_AVAILABLE                    // Enable M3 (A-axis) support
#define M3_STEP_PIN             8      // A-axis step
#define M3_DIRECTION_PIN        9      // A-axis direction
#define M3_STEP_PORT           STEP_PORT
#define M3_DIRECTION_PORT      DIRECTION_PORT

#define M4_AVAILABLE                    // Enable M4 (B-axis) support
#define M4_STEP_PIN             10     // B-axis step
#define M4_DIRECTION_PIN        11     // B-axis direction
#define M4_STEP_PORT           STEP_PORT
#define M4_DIRECTION_PORT      DIRECTION_PORT

// Alias for compatibility
#define A_STEP_PIN              M3_STEP_PIN
#define A_DIRECTION_PIN         M3_DIRECTION_PIN
#define B_STEP_PIN              M4_STEP_PIN
#define B_DIRECTION_PIN         M4_DIRECTION_PIN

#define STEPPERS_ENABLE_PIN     12    // shared enable
#define STEPPERS_ENABLE_PORT    GPIO_OUTPUT

// -----------------------------
// Limits & inputs
// -----------------------------

#define X_LIMIT_PIN             15
#define Y_LIMIT_PIN             17
#define Z_LIMIT_PIN             18
#define A_LIMIT_PIN             19
#define B_LIMIT_PIN             20
#define LIMIT_INMODE            GPIO_MAP

// General input (isolated)
#define AUXINPUT0_PIN           21    // c_input

// Emergency stop (active low NC)
#ifndef CONTROL_ENABLE
#define CONTROL_ENABLE (CONTROL_HALT)
#endif
#define RESET_PIN               22    // estop_all

// -----------------------------
// PWM & auxiliary outputs
// -----------------------------

// Servo-style PWM (50 Hz) for end effector / joint control
#define AUXOUTPUT0_PWM_PIN      13    // pwm_a

// Spindle / power PWM (1–25 kHz)
#define SPINDLE_PORT            GPIO_OUTPUT
#define SPINDLE_PWM_PIN         14    // pwm_b

// Optional third PWM-capable channel (fan/coolant)
#define AUXOUTPUT1_PWM_PIN      16    // pwm_c

// General-purpose outputs
#define AUXOUTPUT0_PORT         GPIO_OUTPUT
#define AUXOUTPUT0_PIN          26    // output_a
#define AUXOUTPUT1_PORT         GPIO_OUTPUT
#define AUXOUTPUT1_PIN          27    // output_b
#define AUXOUTPUT2_PORT         GPIO_OUTPUT
#define AUXOUTPUT2_PIN          28    // output_c

// I2C reserved for V2 expansion (keep default if needed)
#if I2C_ENABLE
#ifndef I2C_PORT
#define I2C_PORT                1
#endif
#ifndef I2C_SDA
#define I2C_SDA                 30
#endif
#ifndef I2C_SCL
#define I2C_SCL                 31
#endif
#endif

// -----------------------------
// SPI Configuration (for future SD card support)
// -----------------------------

#ifndef SPI_PORT
#define SPI_PORT                1
#endif

#ifndef SPI_SCK_PIN
#define SPI_SCK_PIN             46
#endif

#ifndef SPI_MOSI_PIN
#define SPI_MOSI_PIN            43
#endif

#ifndef SPI_MISO_PIN
#define SPI_MISO_PIN            44
#endif

#ifndef SPI_CS_PIN
#define SPI_CS_PIN              41
#endif

#endif // MY_MACHINE_MAP_H



