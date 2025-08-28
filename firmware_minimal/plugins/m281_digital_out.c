/*
  m281_digital_out.c - simple plugin for digital outputs on Aux out 3/4/5

  Usage:
    M281 P<n> S0|1   -> set Aux out <n> low/high (n = 0..2)
*/

#include "driver.h"

#if 1

#include <string.h>
#include "hardware/gpio.h"

#include "grbl/hal.h"
#include "grbl/protocol.h"
#include "grbl/ioports.h"

// Define the GPIO pins for digital outputs
#define M281_OUT0_PIN AUXOUTPUT1_PIN
#define M281_OUT1_PIN AUXOUTPUT2_PIN
#define M281_OUT2_PIN AUXOUTPUT3_PIN

// Array to hold the GPIO pins
static const uint8_t m281_pins[] = {
    M281_OUT0_PIN,
    M281_OUT1_PIN,
    M281_OUT2_PIN
};
#define N_M281_PINS (sizeof(m281_pins) / sizeof(m281_pins[0]))

static user_mcode_ptrs_t user_mcode;
static on_report_options_ptr on_report_options;
static void report_options_cb (bool newopt)
{
    on_report_options(newopt);
    if(!newopt)
        report_plugin("M281 digital out", "0.01");
}

static user_mcode_type_t mcode_check (user_mcode_t mcode)
{
    return mcode == (user_mcode_t)281 ? UserMCode_Normal : (user_mcode.check ? user_mcode.check(mcode) : UserMCode_Unsupported);
}

static status_code_t mcode_validate (parser_block_t *gc_block)
{
    status_code_t state = Status_OK;

    if(gc_block->user_mcode == (user_mcode_t)281) {
        if(!gc_block->words.p || !gc_block->words.s)
            state = Status_GcodeValueWordMissing;
        else if(!isintf(gc_block->values.p) || !isintf(gc_block->values.s))
            state = Status_BadNumberFormat;
        else if((uint8_t)gc_block->values.p > 2 || ((uint8_t)gc_block->values.s > 1))
            state = Status_GcodeValueOutOfRange;
        gc_block->words.p = gc_block->words.s = Off;
    } else
        state = Status_Unhandled;

    return state == Status_Unhandled && user_mcode.validate ? user_mcode.validate(gc_block) : state;
}

static void mcode_execute (uint_fast16_t state, parser_block_t *gc_block)
{
    if(gc_block->user_mcode == (user_mcode_t)281) {
        uint8_t port = (uint8_t)gc_block->values.p;   // Aux out port index
        uint8_t on = (uint8_t)gc_block->values.s;     // 0/1
        
        if(port < N_M281_PINS) {
            // Directly control the GPIO pin using hardware functions
            gpio_put(m281_pins[port], on);
        }
    } else if(user_mcode.execute)
        user_mcode.execute(state, gc_block);
}

void m281_digital_out_init (void)
{
    memcpy(&user_mcode, &grbl.user_mcode, sizeof(user_mcode_ptrs_t));
    grbl.user_mcode.check = mcode_check;
    grbl.user_mcode.validate = mcode_validate;
    grbl.user_mcode.execute = mcode_execute;

    on_report_options = grbl.on_report_options;
    grbl.on_report_options = report_options_cb;
    
    // Initialize GPIO pins as outputs
    for(uint_fast8_t i = 0; i < N_M281_PINS; i++) {
        gpio_init(m281_pins[i]);
        gpio_set_dir(m281_pins[i], GPIO_OUT);
        gpio_put(m281_pins[i], 0); // Set to off by default
    }
}

#endif


