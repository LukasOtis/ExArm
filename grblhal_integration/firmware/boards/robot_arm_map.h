/*
  robot_arm_map.h - driver code for ExArm Robot Arm on RP2350A

  Part of grblHAL

  Copyright (c) 2021-2024 Terje Io

  grblHAL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  grblHAL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with grblHAL. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>

#if TRINAMIC_ENABLE
#error Trinamic plugin not supported!
#endif

// Robot Arm Configuration (5-axis)
#define N_AXIS 5
#define N_ABC_MOTORS 2  // A and B axes

#define BOARD_NAME "ExArm Robot Arm"
#define BOARD_URL "https://github.com/your-repo"

// ============================================
// Axis Configuration (5-axis)
// ============================================

// Step/Direction pins (Direct GPIO)
#define STEP_PORT           GPIO_OUTPUT
#define DIRECTION_PORT      GPIO_OUTPUT
#define ENABLE_PORT         GPIO_OUTPUT

// X-axis (Base rotation)
#define X_STEP_PIN         2
#define X_DIRECTION_PIN    3
#define X_LIMIT_PIN        15

// Y-axis (Shoulder)
#define Y_STEP_PIN         4
#define Y_DIRECTION_PIN    5
#define Y_LIMIT_PIN        17

// Z-axis (Elbow)
#define Z_STEP_PIN         6
#define Z_DIRECTION_PIN    7
#define Z_LIMIT_PIN        18

// A-axis (Wrist roll) - grblHAL uses M3 for A-axis
#define M3_STEP_PIN        8
#define M3_DIRECTION_PIN   9
#define M3_LIMIT_PIN       19

// B-axis (Wrist pitch) - grblHAL uses M4 for B-axis
#define M4_STEP_PIN        10
#define M4_DIRECTION_PIN   11
#define M4_LIMIT_PIN       20

// Shared enable for all steppers
#define STEPPERS_ENABLE_PIN 12
#define STEPPERS_ENABLE_PORT GPIO_OUTPUT

// ============================================
// Limit Switch Configuration
// ============================================

// All limit switches are normally open (NO) and pulled up
#define LIMIT_INVERT_MASK  (1<<X_LIMIT_PIN|1<<Y_LIMIT_PIN|1<<Z_LIMIT_PIN|1<<A_LIMIT_PIN|1<<B_LIMIT_PIN)

// ============================================
// PWM Outputs (Multi-Spindle Support)
// ============================================

// Spindle / power PWM (1–25 kHz)
#define SPINDLE_PWM_PORT    GPIO_OUTPUT
#define SPINDLE_PWM_PIN    14    // pwm_b (main spindle)

// Servo PWM
#define SERVO_PWM_PORT      GPIO_OUTPUT
#define SERVO_PWM_PIN      13    // pwm_a (servo control)

// Fan/Coolant PWM
#define FAN_PWM_PORT        GPIO_OUTPUT
#define FAN_PWM_PIN        16    // pwm_c (fan/coolant)

// ============================================
// General Purpose Outputs
// ============================================

#define OUTPUT_A_PIN        26    // General purpose output
#define OUTPUT_B_PIN        27    // General purpose output  
#define OUTPUT_C_PIN        28    // General purpose output

// ============================================
// Input Configuration
// ============================================

// E-Stop (normally closed, pulled up)
#define E_STOP_PIN         22
#define E_STOP_INVERT      1     // Invert because normally closed

// General input
#define C_INPUT_PIN        21     // General purpose input

// ============================================
// Communication Interfaces
// ============================================

// I2C Configuration (Port 1 for future expansion)
#if I2C_ENABLE
#define I2C_PORT            1
#define I2C_SDA             30
#define I2C_SCL             31
#endif

// SPI Configuration (for future SD card support)
#define SPI_PORT            1
#define SPI_SCK_PIN         46
#define SPI_MOSI_PIN        43
#define SPI_MISO_PIN        44
#define SPI_CS_PIN          41

// ============================================
// USB and Serial Configuration
// ============================================

// USB Serial is enabled by default
#ifndef USB_SERIAL_CDC
#define USB_SERIAL_CDC      1
#endif

// ============================================
// Future RP2350B Upgrade Path
// ============================================

// TODO: RP2350B upgrade path
// Additional pins: GPIO 40-49 for 5 more spindles
// Total: 8 spindles + 5 axes + extensive I/O
// Flash: 4MB vs 2MB (WebUI, more plugins)

// Future multi-spindle configuration (RP2350B only):
// #define SPINDLE3_PWM_PIN        40    // Additional servo
// #define SPINDLE4_PWM_PIN        41    // Additional servo
// #define SPINDLE5_PWM_PIN        42    // Additional servo
// #define SPINDLE6_PWM_PIN        43    // Additional servo
// #define SPINDLE7_PWM_PIN        44    // Additional servo

// ============================================
// Pin Mode Definitions
// ============================================

// Step pin mode (fast output)
#define STEP_PINMODE        PINMODE_OUTPUT

// Direction pin mode (fast output)
#define DIRECTION_PINMODE   PINMODE_OUTPUT

// Enable pin mode (fast output)
#define ENABLE_PINMODE      PINMODE_OUTPUT

// Limit switch pin mode (input with pull-up)
#define LIMIT_PINMODE       PINMODE_INPUT_PULLUP

// PWM pin mode (PWM output)
#define PWM_PINMODE         PINMODE_PWM

// General output pin mode (fast output)
#define OUTPUT_PINMODE      PINMODE_OUTPUT

// General input pin mode (input with pull-up)
#define INPUT_PINMODE       PINMODE_INPUT_PULLUP
