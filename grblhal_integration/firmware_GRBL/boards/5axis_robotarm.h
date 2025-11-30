/*
  driver code for RP2040 ARM processors - 5 Axis Version

  Part of grblHAL

  Copyright (c) 2021-2025 Terje Io
  Copyright (c) 2021 Volksolive

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

#if TRINAMIC_ENABLE
#error Trinamic plugin not supported!
#endif

// Allow two ABC axes (A and B), no ganged axes in this map
#if N_ABC_MOTORS > 2
#error "Axis configuration is not supported!"
#endif

// Define step pulse output pins.
#define STEP_PORT               GPIO_PIO  // N_AXIS pin PIO SM
#define STEP_PINS_BASE          2         // N_AXIS number of consecutive pins are used by PIO

// Define step direction output pins.
#define DIRECTION_PORT          GPIO_OUTPUT
#define X_DIRECTION_PIN         7
#define Y_DIRECTION_PIN         8
#define Z_DIRECTION_PIN         9
#define DIRECTION_OUTMODE       GPIO_SHIFT7

// Define availability and pins for A (M3) and B (M4) axes
#define M3_AVAILABLE
#define M4_AVAILABLE

// Define M3/M4 step pins for A and B axes
#define M3_STEP_PIN            (STEP_PINS_BASE + 3)  // A-axis step (PIO5)
#define M4_STEP_PIN            (STEP_PINS_BASE + 4)  // B-axis step (PIO6)

// Define M3/M4 direction pins directly
#define M3_DIRECTION_PIN       10
#define M4_DIRECTION_PIN       11

// Define M3/M4 ports
#define M3_STEP_PORT           STEP_PORT
#define M4_STEP_PORT           STEP_PORT
#define M3_DIRECTION_PORT      DIRECTION_PORT
#define M4_DIRECTION_PORT      DIRECTION_PORT

// Define stepper driver enable/disable output pin.
#define ENABLE_PORT             GPIO_OUTPUT
#define STEPPERS_ENABLE_PIN     12

// Define homing/hard limit switch input pins (per hardware table)
#define X_LIMIT_PIN             15
#define Y_LIMIT_PIN             17
#define Z_LIMIT_PIN             18
#define M3_LIMIT_PIN            19
#define M4_LIMIT_PIN            20
#define LIMIT_INMODE            GPIO_MAP
// generic input
#define AUXINPUT2_PIN           21

// Map three PWM analog outputs for M280 plugin (P=0..2)
// Note: enumeration assigns Servo 0 to the last-defined analog PWM, so order is reversed
#define AUXOUTPUT0_PWM_PIN      13
#define AUXOUTPUT1_PWM_PIN      14
#define AUXOUTPUT2_PWM_PIN      16


#define AUXOUTPUT1_PORT         GPIO_OUTPUT
#define AUXOUTPUT1_PIN          26
#define AUXOUTPUT2_PORT         GPIO_OUTPUT
#define AUXOUTPUT2_PIN          27
#define AUXOUTPUT3_PORT         GPIO_OUTPUT
#define AUXOUTPUT3_PIN          28



// Define driver spindle pins
#if DRIVER_SPINDLE_ENABLE
#define SPINDLE_PORT            255
#endif

// Ensure no spindle functions are mapped to our AUXOUTPUT pins
#undef AUX_CONTROL_SPINDLE
#define AUX_CONTROL_SPINDLE     0
#undef AUX_CONTROLS
#define AUX_CONTROLS            0

// Disable all spindle functionality completely
#undef SPINDLE_ENABLE
#define SPINDLE_ENABLE          0
#undef DRIVER_SPINDLE_ENABLE
#define DRIVER_SPINDLE_ENABLE   0

// Prevent any spindle pin mappings from generic map
#undef SPINDLE_PWM_PIN
#undef SPINDLE_DIRECTION_PIN
#undef SPINDLE_ENABLE_PIN
#define SPINDLE_PWM_PIN         255
#define SPINDLE_DIRECTION_PIN   255
#define SPINDLE_ENABLE_PIN      255
#define SPINDLE_PORT            255


// Define user-control controls (cycle start, reset, feed hold) input pins.

#if PROBE_ENABLE
#define PROBE_PIN               AUXINPUT2_PIN
#endif

#if SAFETY_DOOR_ENABLE
#define SAFETY_DOOR_PIN         AUXINPUT1_PIN
#endif

#if I2C_STROBE_ENABLE
#define I2C_STROBE_PIN          AUXINPUT0_PIN
#elif MOTOR_FAULT_ENABLE
#define MOTOR_FAULT_PIN         AUXINPUT0_PIN
#endif


// CRITICAL: Completely undefine spindle pins to prevent pin labeling system from seeing them
// This prevents pin_bits_masks.h from creating spindle pin entries
#ifdef SPINDLE_PWM_PIN
#undef SPINDLE_PWM_PIN
#endif
#ifdef SPINDLE_DIRECTION_PIN  
#undef SPINDLE_DIRECTION_PIN
#endif
#ifdef SPINDLE_ENABLE_PIN
#undef SPINDLE_ENABLE_PIN
#endif

// Do NOT redefine them - leave them completely undefined
