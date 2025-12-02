/*
  robotarm_6axis_rp2350b.h

  Board mapping for a 6-axis robot arm on an RP2350B-based minimal board.
  This extends the existing 5-axis Pico 2 mapping to add one extra STEP/DIR
  axis while shifting the remaining I/O "one up" to keep STEP pins
  contiguous for the PIO step generator.

  NOTE: This file does not change any core driver logic – it only defines
  GPIO numbers. To use it, select the appropriate BOARD_* define in
  my_machine.h or via your build configuration and include this header
  from driver.h instead of 5axis_robotarm.h.
*/

#pragma once

// Define step pulse output pins.
// STEP pins are a contiguous block starting at GPIO 2 so they can be driven
// by a single PIO state machine. With 6 axes enabled this yields:
//  X_STEP_PIN: GPIO 2
//  Y_STEP_PIN: GPIO 3
//  Z_STEP_PIN: GPIO 4
//  A_STEP_PIN: GPIO 5
//  B_STEP_PIN: GPIO 6
//  C_STEP_PIN (M5): GPIO 7
#define STEP_PORT               GPIO_PIO
#define STEP_PINS_BASE          2

// Define step direction output pins.
// Direction pins are placed directly above the STEP block to simplify routing.
#define DIRECTION_PORT          GPIO_OUTPUT
#define X_DIRECTION_PIN         8
#define Y_DIRECTION_PIN         9
#define Z_DIRECTION_PIN         10
#define DIRECTION_OUTMODE       GPIO_SHIFT8

// Additional axes A, B, C (mapped as M3, M4, M5 motors)
#define M3_AVAILABLE
#define M4_AVAILABLE
#define M5_AVAILABLE

// STEP pins for A, B, C use the extended STEP_PINS_BASE range.
#define M3_STEP_PIN            (STEP_PINS_BASE + 3)  // A-axis STEP, GPIO 5
#define M4_STEP_PIN            (STEP_PINS_BASE + 4)  // B-axis STEP, GPIO 6
#define M5_STEP_PIN            (STEP_PINS_BASE + 5)  // C-axis STEP, GPIO 7

// Explicit direction pins for A, B, C.
#define M3_DIRECTION_PIN       11
#define M4_DIRECTION_PIN       12
#define M5_DIRECTION_PIN       13

// Define ports for the extra axes.
#define M3_STEP_PORT           STEP_PORT
#define M4_STEP_PORT           STEP_PORT
#define M5_STEP_PORT           STEP_PORT
#define M3_DIRECTION_PORT      DIRECTION_PORT
#define M4_DIRECTION_PORT      DIRECTION_PORT
#define M5_DIRECTION_PORT      DIRECTION_PORT

// Per-axis driver fault inputs (active-low, NC to 3V3 with pull-up).
// Placed immediately after STEP/DIR pins for easier PCB routing.
// These are intended to be wired to closed-loop stepper/servo driver
// fault outputs. When a fault is detected the axis fault plugin will
// raise Alarm_MotorFault and report the affected axes to the host.
#define X_FAULT_PIN             14
#define Y_FAULT_PIN             15
#define Z_FAULT_PIN             16
#define M3_FAULT_PIN            17
#define M4_FAULT_PIN            18
#define M5_FAULT_PIN            19

// Stepper driver enable/disable (shared enable for all axes).
#define ENABLE_PORT             GPIO_OUTPUT
#define STEPPERS_ENABLE_PIN     20

// Homing / hard limit switch inputs, one per axis.
#define X_LIMIT_PIN             21
#define Y_LIMIT_PIN             22
#define Z_LIMIT_PIN             23
#define M3_LIMIT_PIN            24
#define M4_LIMIT_PIN            25
#define M5_LIMIT_PIN            26
// Map M4/M5 limit pins to B/C for driver compatibility
#define B_LIMIT_PIN             M4_LIMIT_PIN
#define C_LIMIT_PIN             M5_LIMIT_PIN
#define LIMIT_INMODE            GPIO_MAP

// Generic aux input, can be used as PROBE or other control input.
#define AUXINPUT2_PIN           27

// PWM analog outputs for M280/M68 plugins.
// These pins are chosen to be PWM-capable on RP2350B:
//  GPIO 30 → PWM 3A
//  GPIO 31 → PWM 3B
//  GPIO 32 → PWM 8A
//  GPIO 33 → PWM 8B
//  GPIO 34 → PWM 9A
//  GPIO 35 → PWM 9B
// Enumeration assigns Servo 0 to the last-defined analog PWM, so the
// order of defines is intentionally reversed.
#define AUXOUTPUT0_PWM_PIN      30
#define AUXOUTPUT1_PWM_PIN      31
#define AUXOUTPUT2_PWM_PIN      32
#define AUXOUTPUT3_PWM_PIN      33
#define AUXOUTPUT4_PWM_PIN      34
#define AUXOUTPUT5_PWM_PIN      35

// Digital aux outputs (M281), up to six channels are exposed.
#define AUXOUTPUT1_PORT         GPIO_OUTPUT
#define AUXOUTPUT1_PIN          28
#define AUXOUTPUT2_PORT         GPIO_OUTPUT
#define AUXOUTPUT2_PIN          29
#define AUXOUTPUT3_PORT         GPIO_OUTPUT
#define AUXOUTPUT3_PIN          36
#define AUXOUTPUT4_PORT         GPIO_OUTPUT
#define AUXOUTPUT4_PIN          37
#define AUXOUTPUT5_PORT         GPIO_OUTPUT
#define AUXOUTPUT5_PIN          38
#define AUXOUTPUT6_PORT         GPIO_OUTPUT
#define AUXOUTPUT6_PIN          39

// Spindle is fully disabled for this robot-arm configuration.
#if DRIVER_SPINDLE_ENABLE
#define SPINDLE_PORT            255
#endif

#undef AUX_CONTROL_SPINDLE
#define AUX_CONTROL_SPINDLE     0
#undef AUX_CONTROLS
#define AUX_CONTROLS            0

#undef SPINDLE_ENABLE
#define SPINDLE_ENABLE          0
#undef DRIVER_SPINDLE_ENABLE
#define DRIVER_SPINDLE_ENABLE   0

#undef SPINDLE_PWM_PIN
#undef SPINDLE_DIRECTION_PIN
#undef SPINDLE_ENABLE_PIN
#define SPINDLE_PWM_PIN         255
#define SPINDLE_DIRECTION_PIN   255
#define SPINDLE_ENABLE_PIN      255
#define SPINDLE_PORT            255

// Optional mappings for GRBLHAL input signals.
#if PROBE_ENABLE
#define PROBE_PIN               AUXINPUT2_PIN
#endif


