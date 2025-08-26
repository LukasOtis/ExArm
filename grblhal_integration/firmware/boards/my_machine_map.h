/*
  my_machine_map.h - Board mapping for ExArm Pico2Breakout (RP2350B)

  Custom pin map aligned with hardware/pcb/controller_board README pinout.
  
  Full 5-axis support with RP2350B (Pico 2) PIO capabilities.
  Step/dir pairs are adjacent (step,dir) on GPIO: (2,3) (4,5) (6,7) (8,9) (10,11)
  Shared enable on GPIO 12.

  Using GPIO_PIO for steps for optimal performance and precise timing.
*/

#ifndef MY_MACHINE_MAP_H
#define MY_MACHINE_MAP_H

#include "driver.h"


#define BOARD_NAME "ExArm Pico2Breakout (5-Axis PIO)"

// -----------------------------
// Step/Dir/Enable (5-axis support) - Using individual PIO state machines for optimal performance
// -----------------------------

#define STEP_PORT               GPIO_PIO      // Use single PIO state machine like reference
#define STEP_PINS_BASE          2         // N_AXIS number of consecutive pins starting from GPIO 2
#define DIRECTION_PORT          GPIO_OUTPUT
#define DIRECTION_OUTMODE       GPIO_SHIFT7   // Bit shift mode starting from X_DIRECTION_PIN (7)
#define ENABLE_PORT             GPIO_OUTPUT

// Step pins are automatically assigned by PIO using STEP_PINS_BASE:
// Pin 2 = X-axis step (Base rotation stepper)
// Pin 3 = Y-axis step (Shoulder stepper)  
// Pin 4 = Z-axis step (Elbow stepper)
// Pin 5 = A-axis step (Linear actuator 1)
// Pin 6 = B-axis step (Linear actuator 2)

// Direction pins - Individual definitions (like reference)
#define X_DIRECTION_PIN         7      // GPIO 7 - Base rotation direction
#define Y_DIRECTION_PIN         8      // GPIO 8 - Shoulder direction
#define Z_DIRECTION_PIN         9      // GPIO 9 - Elbow direction

// M3/M4 pin definitions (required by GrblHAL core for A/B axes)
#define M3_AVAILABLE                    // Enable M3 (A-axis) support
#define M3_STEP_PIN             5      // A-axis step (GPIO 5 - Linear actuator 1)
#define M3_DIRECTION_PIN        10     // A-axis direction (GPIO 10 - Linear actuator 1 direction)
#define M3_STEP_PORT           GPIO_PIO
#define M3_DIRECTION_PORT       GPIO_OUTPUT

#define M4_AVAILABLE                    // Enable M4 (B-axis) support
#define M4_STEP_PIN             6      // B-axis step (GPIO 6 - Linear actuator 2)
#define M4_DIRECTION_PIN        11     // B-axis direction (GPIO 11 - Linear actuator 2 direction)
#define M4_STEP_PORT           GPIO_PIO
#define M4_DIRECTION_PORT       GPIO_OUTPUT

// Note: A/B axis pins are now defined via M3/M4 above for GrblHAL compatibility

// Enable pin
#define STEPPERS_ENABLE_PIN     12    // shared enable for all steppers
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



