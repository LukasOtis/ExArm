/*
  pin_bits_masks_simple.h - Simplified pin definitions for direct GPIO control
  
  This file removes all I/O port dependencies and provides basic pin definitions
  for the ExArm Robot Arm using direct GPIO control.
  
  Part of grblHAL (simplified for Phase 1)
*/

#pragma once

#include "platform.h"

// ============================================================================
// BASIC PIN DEFINITIONS - NO I/O PORT DEPENDENCIES
// ============================================================================

// Sanity checks for required pins
#if PROBE_ENABLE && !defined(PROBE_PIN)
#error "Probe input is not supported in this configuration!"
#endif

#if SAFETY_DOOR_ENABLE && !defined(SAFETY_DOOR_PIN)
#error "Safety door input is not supported in this configuration!"
#endif

#if MOTOR_FAULT_ENABLE && !defined(MOTOR_FAULT_PIN)
#error "Motor fault input is not supported in this configuration!"
#endif

#if MOTOR_WARNING_ENABLE && !defined(MOTOR_WARNING_PIN)
#error "Motor warning input is not supported in this configuration!"
#endif

#if I2C_STROBE_ENABLE && !defined(I2C_STROBE_PIN)
#error "I2C keypad/strobe is not supported in this configuration!"
#endif

#if MPG_ENABLE == 1 && !defined(MPG_MODE_PIN)
#error "MPG mode input is not supported in this configuration!"
#endif

#if QEI_SELECT_ENABLE && !defined(QEI_SELECT_PIN)
#error "Encoder select input is not supported in this configuration!"
#endif

// ============================================================================
// CONTROL INPUT SIGNALS - DIRECT GPIO CONTROL
// ============================================================================

// Define the CONTROL_PORT symbol as a shorthand in the *_map.h file if all control inputs share the same port.
#ifdef CONTROL_PORT

#ifndef RESET_PORT
#define RESET_PORT CONTROL_PORT
#endif
#ifndef FEED_HOLD_PORT
#define FEED_HOLD_PORT CONTROL_PORT
#endif
#ifndef CYCLE_START_PORT
#define CYCLE_START_PORT CONTROL_PORT
#endif
#ifndef ESTOP_PORT
#define ESTOP_PORT CONTROL_PORT
#endif
#ifndef PROBE_DISCONNECT_PORT
#define PROBE_DISCONNECT_PORT CONTROL_PORT
#endif
#ifndef STOP_DISABLE_PORT
#define STOP_DISABLE_PORT CONTROL_PORT
#endif
#ifndef BLOCK_DELETE_PORT
#define BLOCK_DELETE_PORT CONTROL_PORT
#endif
#ifndef SINGLE_BLOCK_PORT
#define SINGLE_BLOCK_PORT CONTROL_PORT
#endif
#ifndef MOTOR_FAULT_PORT
#define MOTOR_FAULT_PORT CONTROL_PORT
#endif
#ifndef MOTOR_WARNING_PORT
#define MOTOR_WARNING_PORT CONTROL_PORT
#endif
#ifndef LIMITS_OVERRIDE_PORT
#define LIMITS_OVERRIDE_PORT CONTROL_PORT
#endif
#if SAFETY_DOOR_ENABLE && !defined(SAFETY_DOOR_PORT)
#define SAFETY_DOOR_PORT CONTROL_PORT
#endif

#endif // CONTROL_PORT

// ============================================================================
// BASIC PIN STRUCTURES - SIMPLIFIED FOR DIRECT GPIO
// ============================================================================

// Simple pin structure for direct GPIO control
typedef struct {
    uint8_t pin;
    void *port;
    const char *description;
} simple_pin_t;

// ============================================================================
// END OF SIMPLIFIED PIN DEFINITIONS
// ============================================================================
