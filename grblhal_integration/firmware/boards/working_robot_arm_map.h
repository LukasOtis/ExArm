/*
  generic_map.h - driver code for RP2040 ARM processors

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

// Support for A, B, C axes (M3, M4, M5)
#define N_ABC_MOTORS 2  // A and B axes

#if I2C_STROBE_ENABLE && MOTOR_FAULT_ENABLE
#error "Motor fault input and I2C strobe input (keypad plugin) cannot be enabled at the same time."
#endif

// Define step pulse output pins.
#define STEP_PORT               GPIO_PIO  // N_AXIS pin PIO SM
#define STEP_PINS_BASE          2         // N_AXIS number of consecutive pins are used by PIO

// Define step direction output pins.
#define DIRECTION_PORT          GPIO_OUTPUT
#define X_DIRECTION_PIN         5
#define Y_DIRECTION_PIN         6
#define Z_DIRECTION_PIN         7
#define DIRECTION_OUTMODE       GPIO_SHIFT5

// Define stepper driver enable/disable output pin.
#define ENABLE_PORT             GPIO_OUTPUT
#define STEPPERS_ENABLE_PIN     8

// Define homing/hard limit switch input pins.
#define X_LIMIT_PIN             9
#define Y_LIMIT_PIN             10
#define Z_LIMIT_PIN             11

// A-axis (M3) pins
#define M3_STEP_PIN             8      // A-axis step
#define M3_DIRECTION_PIN        9      // A-axis direction
#define M3_LIMIT_PIN            19     // A-axis limit

// B-axis (M4) pins
#define M4_STEP_PIN             10     // B-axis step
#define M4_DIRECTION_PIN        11     // B-axis direction
#define M4_LIMIT_PIN            20     // B-axis limit

// C-axis (M5) pins - not used but required by grblHAL
#define M5_STEP_PIN             12     // C-axis step (unused)
#define M5_DIRECTION_PIN        13     // C-axis direction (unused)
#define M5_LIMIT_PIN            21     // C-axis limit (unused)
#define LIMIT_INMODE            GPIO_MAP

#define AUXOUTPUT0_PORT         GPIO_OUTPUT
#define AUXOUTPUT0_PIN          12
#if I2C_ENABLE
#define I2C_PORT                1
#define I2C_SDA                 26
#define I2C_SCL                 27
#else
#define AUXOUTPUT1_PORT         GPIO_OUTPUT
#define AUXOUTPUT1_PIN          26
#define AUXOUTPUT2_PORT         GPIO_OUTPUT
#define AUXOUTPUT2_PIN          27
#endif
#define AUXOUTPUT3_PORT         GPIO_OUTPUT // Spindle PWM
#define AUXOUTPUT3_PIN          15
#define AUXOUTPUT4_PORT         GPIO_OUTPUT // Spindle direction
#define AUXOUTPUT4_PIN          14
#define AUXOUTPUT5_PORT         GPIO_OUTPUT // Spindle enable
#define AUXOUTPUT5_PIN          13   
#define AUXOUTPUT6_PORT         GPIO_OUTPUT // Coolant flood
#define AUXOUTPUT6_PIN          16   
#define AUXOUTPUT7_PORT         GPIO_OUTPUT // Coolant mist
#define AUXOUTPUT7_PIN          17

// SPI Configuration (for future SD card support)
#define SPI_PORT                1
#define SPI_SCK_PIN             46
#define SPI_MOSI_PIN            43
#define SPI_MISO_PIN            44
#define SPI_CS_PIN              41   

//#define NEOPIXELS_PIN 27
//#define NEOPIXELS_NUM 5
//Define user-control controls (cycle start, reset, feed hold) input pins.

// Define driver spindle pins
#if DRIVER_SPINDLE_ENABLE
#define SPINDLE_PORT            GPIO_OUTPUT
#endif
#if DRIVER_SPINDLE_ENABLE & SPINDLE_PWM
#define SPINDLE_PWM_PIN         AUXOUTPUT3_PIN
#endif
#if DRIVER_SPINDLE_ENABLE & SPINDLE_DIR
#define SPINDLE_DIRECTION_PIN   AUXOUTPUT4_PIN
#endif
#if DRIVER_SPINDLE_ENABLE & SPINDLE_ENA   
#define SPINDLE_ENABLE_PIN      AUXOUTPUT5_PIN
#endif

// Define flood and mist coolant enable output pins.
#if COOLANT_ENABLE
#define COOLANT_PORT            GPIO_OUTPUT
#endif
#if COOLANT_ENABLE & COOLANT_FLOOD
#define COOLANT_FLOOD_PIN       AUXOUTPUT6_PIN
#endif
#if COOLANT_ENABLE & COOLANT_MIST
#define COOLANT_MIST_PIN        AUXOUTPUT7_PIN
#endif

// Define auxiliary I/O
#define AUXINPUT0_PIN           22
#define AUXINPUT1_PIN           21
#define AUXINPUT2_PIN           28 // Probe
#define AUXINPUT3_PIN           18 // Reset/EStop
#define AUXINPUT4_PIN           19 // Feed hold
#define AUXINPUT5_PIN           20 // Cycle start

// Define user-control controls (cycle start, reset, feed hold) input pins.
#if CONTROL_ENABLE & CONTROL_HALT
#define RESET_PIN               AUXINPUT3_PIN
#endif
#if CONTROL_ENABLE & CONTROL_FEED_HOLD
#define FEED_HOLD_PIN           AUXINPUT4_PIN
#endif
#if CONTROL_ENABLE & CONTROL_CYCLE_START
#define CYCLE_START_PIN         AUXINPUT5_PIN
#endif

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
