/*
 * canbus.c - CAN bus interface for grblHAL
 *
 * This file provides stub implementations for CAN bus functionality
 * which is not used in the basic robot arm configuration.
 */

#include "canbus.h"
#include "system.h"  // For status_code_t and Status_Unhandled
#include "tool_change.h"  // For tool_data_t

// Stub implementation - CAN bus not supported in basic robot arm
bool canbus_enabled (void) {
    return false; // CAN bus disabled
}

// Stub implementation - no CAN bus initialization
void canbus_init (void) {
    // No initialization needed
}

// Stub implementation - no CAN bus processing
void canbus_process (void) {
    // No processing needed
}

// Stub implementation - no CAN bus streaming
bool canbus_stream_rx (char c) {
    return false; // Not handled
}

// Stub implementation - no CAN bus settings
status_code_t canbus_setting (uint_fast16_t setting, float value, char *svalue) {
    return Status_Unhandled;
}

// Stub implementation - no CAN bus info
void canbus_info (tool_data_t *tool) {
    // No info to provide
}
