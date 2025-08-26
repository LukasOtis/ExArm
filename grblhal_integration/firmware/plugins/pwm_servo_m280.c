/*

  pwm_servo_m280.c - plugin for M280, Marlin style servo commands

  Part of grblHAL misc. plugins

  Based on code by @wakass. Public domain.
  https://github.com/wakass/grlbhal_servo

  Usage:
    M280[P<id>][S<pwm_level>]

  If no words are specified all servo positions are reported.
  If no position is specified the specific servo position is returned.

  S parameter: Direct PWM level (0-65535), no angle conversion.
  ROS2 handles all calculations and sends appropriate PWM values.

  https://marlinfw.org/docs/gcode/M280.html

*/

#include "driver.h"

#if PWM_SERVO_ENABLE == 1

#include <math.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "grbl/hal.h"
#include "grbl/protocol.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#ifndef N_PWM_SERVOS
#define N_PWM_SERVOS 3  // Support 3 PWM outputs (servo, spindle, fan)
#endif

#if N_PWM_SERVOS > 4
#undef N_PWM_SERVOS
#define N_PWM_SERVOS 4
#endif

#define DEFAULT_MIN_ANGLE 0.0f
#define DEFAULT_MAX_ANGLE 180.0f

//These are the min and max pulse width in microseconds that are expected by servo. These correspond to the minimum and maximum angle.
#define DEFAULT_MIN_PULSE_WIDTH 544e-6
#define DEFAULT_MAX_PULSE_WIDTH 2400e-6
#define DEFAULT_PWM_FREQ        50.0f

// PWM pins for servos (corrected from hardware README)
static const uint8_t servo_pins[N_PWM_SERVOS] = {
    13,  // GPIO 13 - PWM A: Servo PWM (End effector servo, 50Hz)
    14,  // GPIO 14 - PWM B: Spindle/Laser PWM (Variable speed, 1-25kHz)
    16   // GPIO 16 - PWM C: Fan/Coolant PWM (Independent frequency control)
};

typedef struct {
    uint8_t pin;      // GPIO pin number
    float min_angle;  // Minimum angle in degrees
    float max_angle;  // Maximum angle in degrees
    float angle;      // Current angle in degrees
    uint slice_num;   // PWM slice number
    uint chan;        // PWM channel
} servo_t;

static user_mcode_ptrs_t user_mcode;
static on_report_options_ptr on_report_options;
static uint8_t n_servos = 0;
static servo_t servos[N_PWM_SERVOS];

/// @brief Set servo position in degrees
/// @param servo Servo number
/// @param angle Angle (in degrees) to set servo to
/// @return true if successful
static bool pwm_servo_set_angle(uint8_t servo, float angle)
{
    if(servo < n_servos) {
        servos[servo].angle = angle;

        // Direct PWM control - ROS2 handles calculations
        // S parameter is directly used as PWM level (0-65535)
        uint16_t pwm_level = (uint16_t)angle;
        pwm_set_chan_level(servos[servo].slice_num, servos[servo].chan, pwm_level);
        return true;
    }

    return false;
}

static float pwm_servo_get_angle(uint8_t servo)
{
    return servo < n_servos ? servos[servo].angle : -1.0f;
}

static user_mcode_type_t mcode_check (user_mcode_t mcode)
{
    return mcode == PWMServo_SetPosition
                     ? UserMCode_Normal
                     : (user_mcode.check ? user_mcode.check(mcode) : UserMCode_Unsupported);
}

static status_code_t mcode_validate (parser_block_t *gc_block)
{
    status_code_t state = Status_OK;

    if(gc_block->user_mcode == PWMServo_SetPosition) {
        if(gc_block->words.p) {
            if(!isintf(gc_block->values.p))
                state = Status_BadNumberFormat;
            else if(gc_block->words.p && ((uint8_t)gc_block->values.p >= n_servos))
                state = Status_GcodeValueOutOfRange;
        }
        if(gc_block->words.s && (gc_block->values.s < 0 || gc_block->values.s > 65535))
            state = Status_GcodeValueOutOfRange;
        gc_block->words.s = gc_block->words.p = Off;
    } else
        state = Status_Unhandled;

    return state == Status_Unhandled && user_mcode.validate ? user_mcode.validate(gc_block) : state;
}

static void mcode_execute (uint_fast16_t state, parser_block_t *gc_block)
{
    if(gc_block->user_mcode == PWMServo_SetPosition) {

        uint8_t servo = (uint8_t)gc_block->values.p;

        if(gc_block->words.s) {
#ifdef DEBUGOUT
            debug_print("Setting servo position");
#endif
            pwm_servo_set_angle(servo, gc_block->values.s);
        } else {
            //Reads the position/pwm
            float value = pwm_servo_get_angle(servo);
            if (value >= 0.0f) {
                char buf[40];
#ifdef DEBUGOUT
                debug_print("[Servo position: %5.2f degrees]",  value);
#endif
                // TODO: check Marlin format?
                strcpy(buf, "[Servo ");
                strcat(buf, uitoa(servo));
                strcat(buf, " position: ");
                strcat(buf, ftoa(value, 2));
                strcat(buf, " degrees]" ASCII_EOL);
                hal.stream.write(buf);
            }
        }
    } else if(user_mcode.execute)
        user_mcode.execute(state, gc_block);
}

static void onReportOptions (bool newopt)
{
    on_report_options(newopt);

    if(!newopt)
        report_plugin("PWM servo", "0.04");
}

static bool init_servo_default (servo_t* servo)
{
    servo->max_angle = DEFAULT_MAX_ANGLE;
    servo->min_angle = DEFAULT_MIN_ANGLE;
    servo->angle = 0.0f;

    // Initialize PWM for this servo
    gpio_set_function(servo->pin, GPIO_FUNC_PWM);
    servo->slice_num = pwm_gpio_to_slice_num(servo->pin);
    servo->chan = pwm_gpio_to_channel(servo->pin);

    // Set PWM frequency
    pwm_set_wrap(servo->slice_num, 65535);  // 16-bit resolution
    pwm_set_clkdiv(servo->slice_num, clock_get_hz(clk_sys) / (65535.0f * DEFAULT_PWM_FREQ));
    pwm_set_enabled(servo->slice_num, true);

    // Set initial position to 0 degrees
    pwm_servo_set_angle(n_servos, 0.0f);

    return true;
}

void pwm_servo_init (void)
{
    memcpy(&user_mcode, &grbl.user_mcode, sizeof(user_mcode_ptrs_t));

    grbl.user_mcode.check = mcode_check;
    grbl.user_mcode.validate = mcode_validate;
    grbl.user_mcode.execute = mcode_execute;

    // Initialize servos
    for(uint8_t i = 0; i < N_PWM_SERVOS; i++) {
        servos[n_servos].pin = servo_pins[i];
        if(init_servo_default(&servos[n_servos])) {
            n_servos++;
        }
    }

    on_report_options = grbl.on_report_options;
    grbl.on_report_options = onReportOptions;
}

#endif // PWM_SERVO_ENABLE