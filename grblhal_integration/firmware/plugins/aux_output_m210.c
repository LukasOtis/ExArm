/*
  aux_output_m210.c - plugin for M210-M215, auxiliary output control commands

  Part of grblHAL misc. plugins

  Usage:
    M210 - Aux0 ON
    M211 - Aux0 OFF
    M212 - Aux1 ON
    M213 - Aux1 OFF
    M214 - Aux2 ON
    M215 - Aux2 OFF

  These commands control GPIO outputs for relays, lights, or other auxiliary devices.
*/

#include "driver.h"

#if AUX_OUTPUT_ENABLE == 1

#include <string.h>
#include <stdio.h>
#include "grbl/hal.h"
#include "grbl/protocol.h"
#include "hardware/gpio.h"

// Number of auxiliary outputs supported
#define N_AUX_OUTPUTS 3

// Auxiliary output pin definitions (from my_machine_map.h)
static const uint8_t aux_output_pins[N_AUX_OUTPUTS] = {
    AUXOUTPUT0_PIN,  // GPIO 26
    AUXOUTPUT1_PIN,  // GPIO 27
    AUXOUTPUT2_PIN   // GPIO 28
};

// Auxiliary output state tracking
typedef struct {
    uint8_t pin;           // GPIO pin number
    bool state;            // Current state (true = ON, false = OFF)
    bool enabled;          // Whether this output is enabled
} aux_output_t;

static user_mcode_ptrs_t user_mcode;
static on_report_options_ptr on_report_options;
static aux_output_t aux_outputs[N_AUX_OUTPUTS];

/// @brief Set auxiliary output state
/// @param output Output number (0-2)
/// @param state State to set (true = ON, false = OFF)
/// @return true if successful
static bool aux_output_set_state(uint8_t output, bool state)
{
    if(output < N_AUX_OUTPUTS && aux_outputs[output].enabled) {
        aux_outputs[output].state = state;
        gpio_put(aux_outputs[output].pin, state);
        return true;
    }
    return false;
}



/// @brief Initialize auxiliary outputs
static void aux_output_init_pins(void)
{
    for(uint8_t i = 0; i < N_AUX_OUTPUTS; i++) {
        // Configure GPIO as output
        gpio_init(aux_outputs[i].pin);
        gpio_set_dir(aux_outputs[i].pin, GPIO_OUT);
        gpio_put(aux_outputs[i].pin, false); // Start with OFF state
        
        // Initialize tracking structure
        aux_outputs[i].pin = aux_output_pins[i];
        aux_outputs[i].state = false;
        aux_outputs[i].enabled = true;
    }
}

/// @brief M-code validation function
static status_code_t aux_output_mcode_validate(parser_block_t *gc_block)
{
    status_code_t state = Status_OK;
    
    // Check if this is one of our M-codes
    if(gc_block->user_mcode == AuxOutput0_On || gc_block->user_mcode == AuxOutput0_Off ||
       gc_block->user_mcode == AuxOutput1_On || gc_block->user_mcode == AuxOutput1_Off ||
       gc_block->user_mcode == AuxOutput2_On || gc_block->user_mcode == AuxOutput2_Off) {
        
        // No additional validation needed for simple ON/OFF commands
        gc_block->words.p = gc_block->words.s = Off; // Clear any unused words
    } else
        state = Status_Unhandled;

    // Chain with existing validation handler
    return state == Status_Unhandled && user_mcode.validate ? user_mcode.validate(gc_block) : state;
}

/// @brief M-code execution handler for auxiliary outputs
static void aux_output_mcode_execute(uint_fast16_t state, parser_block_t *gc_block)
{
    // Handle our M-codes
    if(gc_block->user_mcode == AuxOutput0_On) {
        aux_output_set_state(0, true);
    } else if(gc_block->user_mcode == AuxOutput0_Off) {
        aux_output_set_state(0, false);
    } else if(gc_block->user_mcode == AuxOutput1_On) {
        aux_output_set_state(1, true);
    } else if(gc_block->user_mcode == AuxOutput1_Off) {
        aux_output_set_state(1, false);
    } else if(gc_block->user_mcode == AuxOutput2_On) {
        aux_output_set_state(2, true);
    } else if(gc_block->user_mcode == AuxOutput2_Off) {
        aux_output_set_state(2, false);
    } else if(user_mcode.execute) {
        // Chain with existing execute handler
        user_mcode.execute(state, gc_block);
    }
}

/// @brief M-code validation function
static user_mcode_type_t aux_output_mcode_check(user_mcode_t mcode)
{
    // Check if this is one of our M-codes
    switch(mcode) {
        case AuxOutput0_On:
        case AuxOutput0_Off:
        case AuxOutput1_On:
        case AuxOutput1_Off:
        case AuxOutput2_On:
        case AuxOutput2_Off:
            return UserMCode_Normal;
        default:
            // Chain with existing check handler
            return user_mcode.check ? user_mcode.check(mcode) : UserMCode_Unsupported;
    }
}

/// @brief Report auxiliary output status
static void aux_output_report_status(bool newopt)
{
    if(!newopt) {
        // Report current state of all auxiliary outputs
        for(uint8_t i = 0; i < N_AUX_OUTPUTS; i++) {
            if(aux_outputs[i].enabled) {
                char buf[32];
                snprintf(buf, sizeof(buf), "Aux%d:%s", i, aux_outputs[i].state ? "ON" : "OFF");
                hal.stream.write(buf);
                hal.stream.write("\n");
            }
        }
    }
    
    // Call the original report function if it exists
    if(on_report_options)
        on_report_options(newopt);
}

/// @brief Plugin initialization
void aux_output_init(void)
{
    // Initialize GPIO pins
    aux_output_init_pins();
    
    // Store existing M-code handlers to chain properly
    memcpy(&user_mcode, &grbl.user_mcode, sizeof(user_mcode_ptrs_t));
    
    // Chain our handlers with existing ones
    grbl.user_mcode.check = aux_output_mcode_check;
    grbl.user_mcode.validate = aux_output_mcode_validate;
    grbl.user_mcode.execute = aux_output_mcode_execute;
    
    // Register status reporting (chain with existing)
    on_report_options = grbl.on_report_options;
    grbl.on_report_options = aux_output_report_status;
    
    // Report initialization
    hal.stream.write("Auxiliary Output Plugin initialized\n");
    aux_output_report_status(false); // Report current status
}

#endif // AUX_OUTPUT_ENABLE
