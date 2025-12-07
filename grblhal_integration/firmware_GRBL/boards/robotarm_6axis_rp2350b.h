/*
  robotarm_6axis_rp2350b.h

  Board mapping for a 6-axis robot arm on an RP2350B-based minimal board.
  
  Pin layout (reorganized for better PCB routing):
  - GPIO 0-1:   UART (TX/RX)
  - GPIO 2-9:   Limits (6) + Aux input + E-stop (8 pins)
  - GPIO 10-15: Axis fault inputs (6 pins)
  - GPIO 16-21: STEP pins (6 contiguous for PIO)
  - GPIO 22-27: DIR pins (6 pins)
  - GPIO 28:    Enable (all steppers)
  - GPIO 29-34: PWM outputs (6 pins for M280/M68)
  - GPIO 35-40: Digital aux outputs (6 pins for M281)

  NOTE: This file does not change any core driver logic – it only defines
  GPIO numbers. To use it, select the appropriate BOARD_* define in
  my_machine.h or via your build configuration and include this header
  from driver.h instead of 5axis_robotarm.h.
*/

#pragma once

// UART pins (GPIO 0-1) - defined by driver, not here

// Homing / hard limit switch inputs, one per axis (GPIO 2-7)
#define X_LIMIT_PIN             2
#define Y_LIMIT_PIN             3
#define Z_LIMIT_PIN             4
#define M3_LIMIT_PIN            5
#define M4_LIMIT_PIN            6
#define M5_LIMIT_PIN            7
// Map M4/M5 limit pins to B/C for driver compatibility
#define B_LIMIT_PIN             M4_LIMIT_PIN
#define C_LIMIT_PIN             M5_LIMIT_PIN
#define LIMIT_INMODE            GPIO_MAP

// Generic aux input (GPIO 8), can be used as PROBE or other control input.
#define AUXINPUT2_PIN           8

// E-stop input (GPIO 9)
// Define RESET_PIN for E-stop functionality (requires CONTROL_ENABLE & CONTROL_ESTOP)
#define RESET_PIN               9

// Per-axis driver fault inputs (GPIO 10-15, active-low, NC to 3V3 with pull-up).
// These are intended to be wired to closed-loop stepper/servo driver
// fault outputs. When a fault is detected the axis fault plugin will
// raise Alarm_MotorFault and report the affected axes to the host.
#define X_FAULT_PIN             10
#define Y_FAULT_PIN             11
#define Z_FAULT_PIN             12
#define M3_FAULT_PIN            13
#define M4_FAULT_PIN            14
#define M5_FAULT_PIN            15

// Define step pulse output pins (GPIO 16-21).
// STEP pins are a contiguous block starting at GPIO 16 so they can be driven
// by a single PIO state machine. With 6 axes enabled this yields:
//  X_STEP_PIN: GPIO 16
//  Y_STEP_PIN: GPIO 17
//  Z_STEP_PIN: GPIO 18
//  A_STEP_PIN: GPIO 19
//  B_STEP_PIN: GPIO 20
//  C_STEP_PIN (M5): GPIO 21
#define STEP_PORT               GPIO_PIO
#define STEP_PINS_BASE          16

// Define step direction output pins (GPIO 22-27).
#define DIRECTION_PORT          GPIO_OUTPUT
#define X_DIRECTION_PIN         22
#define Y_DIRECTION_PIN         23
#define Z_DIRECTION_PIN         24
#define DIRECTION_OUTMODE       GPIO_SHIFT8

// Additional axes A, B, C (mapped as M3, M4, M5 motors)
#define M3_AVAILABLE
#define M4_AVAILABLE
#define M5_AVAILABLE

// STEP pins for A, B, C use the extended STEP_PINS_BASE range.
#define M3_STEP_PIN            (STEP_PINS_BASE + 3)  // A-axis STEP, GPIO 19
#define M4_STEP_PIN            (STEP_PINS_BASE + 4)  // B-axis STEP, GPIO 20
#define M5_STEP_PIN            (STEP_PINS_BASE + 5)  // C-axis STEP, GPIO 21

// Explicit direction pins for A, B, C.
#define M3_DIRECTION_PIN       25
#define M4_DIRECTION_PIN       26
#define M5_DIRECTION_PIN       27

// Define ports for the extra axes.
#define M3_STEP_PORT           STEP_PORT
#define M4_STEP_PORT           STEP_PORT
#define M5_STEP_PORT           STEP_PORT
#define M3_DIRECTION_PORT      DIRECTION_PORT
#define M4_DIRECTION_PORT      DIRECTION_PORT
#define M5_DIRECTION_PORT      DIRECTION_PORT

// Stepper driver enable/disable (shared enable for all axes, GPIO 28).
#define ENABLE_PORT             GPIO_OUTPUT
#define STEPPERS_ENABLE_PIN     28

// PWM analog outputs for M280/M68 plugins (GPIO 29-34).
// These pins are chosen to be PWM-capable on RP2350B:
//  GPIO 29 → PWM 6B (from 16-31 range)
//  GPIO 30 → PWM 7A
//  GPIO 31 → PWM 7B
//  GPIO 32 → PWM 8A (from 32-47 range)
//  GPIO 33 → PWM 8B
//  GPIO 34 → PWM 9A
// Enumeration assigns Servo 0 to the last-defined analog PWM, so the
// order of defines is intentionally reversed.
#define AUXOUTPUT0_PWM_PIN      29
#define AUXOUTPUT1_PWM_PIN      30
#define AUXOUTPUT2_PWM_PIN      31
#define AUXOUTPUT3_PWM_PIN      32
#define AUXOUTPUT4_PWM_PIN      33
#define AUXOUTPUT5_PWM_PIN      34

// Digital aux outputs (M281), inverted order for PCB routing (GPIO 35-40).
// Pin order inverted: P0=GPIO40, P1=GPIO39, P2=GPIO38, P3=GPIO37, P4=GPIO36, P5=GPIO35
#define AUXOUTPUT1_PORT         GPIO_OUTPUT
#define AUXOUTPUT1_PIN          40  // P0 (was 35)
#define AUXOUTPUT2_PORT         GPIO_OUTPUT
#define AUXOUTPUT2_PIN          39  // P1 (was 36)
#define AUXOUTPUT3_PORT         GPIO_OUTPUT
#define AUXOUTPUT3_PIN          38  // P2 (was 37)
#define AUXOUTPUT4_PORT         GPIO_OUTPUT
#define AUXOUTPUT4_PIN          37  // P3 (was 38)
#define AUXOUTPUT5_PORT         GPIO_OUTPUT
#define AUXOUTPUT5_PIN          36  // P4 (was 39)
#define AUXOUTPUT6_PORT         GPIO_OUTPUT
#define AUXOUTPUT6_PIN          35  // P5 (was 40)

// LED indicator output (GPIO 47) - can be toggled by ROS to indicate connection status
#define AUXOUTPUT7_PORT         GPIO_OUTPUT
#define AUXOUTPUT7_PIN          47  // P6 - LED indicator

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
