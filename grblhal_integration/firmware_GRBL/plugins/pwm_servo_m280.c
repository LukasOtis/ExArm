/*

  pwm_servo_m280.c - plugin for M280, Marlin style servo commands

  Part of grblHAL misc. plugins

  Based on code by @wakass. Public domain.
  https://github.com/wakass/grlbhal_servo

  Usage:
    M280[P<id>][S<position>]

  If no words are specified all servo positions are reported.
  If no position is specified the specific servo position is returned.

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
#include "grbl/ioports.h"

#ifndef N_PWM_SERVOS
#define N_PWM_SERVOS 1
#endif

// Allow up to 6 PWM servos for 6-axis robot arm configuration
#if N_PWM_SERVOS > 6
#undef N_PWM_SERVOS
#define N_PWM_SERVOS 6
#endif

#define DEFAULT_MIN_ANGLE 0.0f
#define DEFAULT_MAX_ANGLE 180.0f

//These are the min and max pulse width in microseconds that are expected by servo. These correspond to the minimum and maximum angle.
#define DEFAULT_MIN_PULSE_WIDTH 544e-6
#define DEFAULT_MAX_PULSE_WIDTH 2400e-6
#define DEFAULT_PWM_FREQ        50.0f

typedef struct {
    uint8_t port; //Port number, referring to (analog) HAL port number
    xbar_t xport; //Handle to ioport xbar object, obtained at init
    float min_angle;
    float max_angle;
    float angle; //Current setpoint for the angle. (degrees)
} servo_t;

static user_mcode_ptrs_t user_mcode;
static on_report_options_ptr on_report_options;
static uint8_t n_servos = 0;
static servo_t servos[N_PWM_SERVOS];
static bool attached[N_PWM_SERVOS] = {0};
static const uint8_t servo_gpio[N_PWM_SERVOS] = {
#if N_PWM_SERVOS > 0
    AUXOUTPUT0_PWM_PIN,
#endif
#if N_PWM_SERVOS > 1
    AUXOUTPUT1_PWM_PIN,
#endif
#if N_PWM_SERVOS > 2
    AUXOUTPUT2_PWM_PIN,
#endif
#if N_PWM_SERVOS > 3
    AUXOUTPUT3_PWM_PIN,
#endif
#if N_PWM_SERVOS > 4
    AUXOUTPUT4_PWM_PIN,
#endif
#if N_PWM_SERVOS > 5
    AUXOUTPUT5_PWM_PIN,
#endif
};

// Forward declaration for lazy attach
static bool servo_attach (xbar_t *pwm_pin, uint8_t port, void *data);

/// @brief 
/// @param servo Servo number
/// @param angle Angle (in degrees) to set servo to
/// @return 
static bool pwm_servo_set_angle(uint8_t servo, float angle)
{
    //Set the position/pwm
    //Servo position is defined from 0 to 180 degrees (left, right)
    //90 degree is the half duty cycle position
    if(servo < N_PWM_SERVOS && attached[servo]) {
        servos[servo].angle = angle;
        ioport_analog_out(servos[servo].port, angle);
        return true;
    }

    return false;
}

static float pwm_servo_get_angle(uint8_t servo)
{
    return (servo < N_PWM_SERVOS && attached[servo]) ? (servos[servo].xport.get_value ? servos[servo].xport.get_value(&servos[servo].xport) : servos[servo].angle) : -1.0f;
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
        // Validate without relying on attached servos
        if(gc_block->words.p) {
            if(!isintf(gc_block->values.p))
                state = Status_BadNumberFormat;
            else if((uint8_t)gc_block->values.p >= N_PWM_SERVOS)
                state = Status_GcodeValueOutOfRange;
        }
        if(state == Status_OK && gc_block->words.s) {
            if(gc_block->values.s < DEFAULT_MIN_ANGLE || gc_block->values.s > DEFAULT_MAX_ANGLE)
                state = Status_GcodeValueOutOfRange;
        }
        // Clear word flags to indicate they've been validated
        // The parser will restore them before calling execute
        gc_block->words.s = gc_block->words.p = Off;
    } else
        state = Status_Unhandled;

    return state == Status_Unhandled && user_mcode.validate ? user_mcode.validate(gc_block) : state;
}

static void mcode_execute (uint_fast16_t state, parser_block_t *gc_block)
{
    if(gc_block->user_mcode == PWMServo_SetPosition) {
        uint8_t servo = (uint8_t)gc_block->values.p;

        // Validate servo index
        if(servo >= N_PWM_SERVOS) {
            if(user_mcode.execute)
                user_mcode.execute(state, gc_block);
            return;
        }

        // Attach only the addressed channel, leave others in analog mode
        if(!attached[servo]) {
            uint8_t target = servo;
            ioports_enumerate(Port_Analog, Port_Output, (pin_cap_t){ .pwm = On, .claimable = On }, servo_attach, &target);
        }

        // Check if servo is attached before trying to use it
        if(!attached[servo]) {
            // Servo attachment failed, cannot proceed
            if(user_mcode.execute)
                user_mcode.execute(state, gc_block);
            return;
        }

        // Use the validated values (word flags are cleared by validation, but values remain)
        // Check if S value was provided by checking if it's not the default (or use a different method)
        // Since validation cleared words.s, we need to check the value differently
        // For now, always try to set if we have a valid servo attached
        // The original code checked words.s, but that's cleared by validation
        // We'll use a different approach: check if values.s was set (non-zero or explicitly provided)
        // Actually, let's just always set the angle if servo is attached, or read it if not provided
        // But we need to know if S was provided... Let's check if values.s is valid
        
        // Check if S was provided (parser restores word flags before execute)
        if(gc_block->words.s) {
            // S was provided, set the angle
#ifdef DEBUGOUT
            debug_print("Setting servo position");
#endif
            pwm_servo_set_angle(servo, gc_block->values.s);
        } else {
            // S was not provided, read the current position
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

    return true;
}

static bool servo_attach (xbar_t *pwm_pin, uint8_t port, void *data)
{
    static const char *descr[] = {
        "PWM Servo 0",
        "PWM Servo 1",
        "PWM Servo 2",
        "PWM Servo 3",
        "PWM Servo 4",
        "PWM Servo 5"
    };
    // Expected target servo index passed via data
    uint8_t target = data ? *(uint8_t *)data : 255;
    if(target >= N_PWM_SERVOS)
        return false;

    // Only attach when enumerated pin matches desired GPIO
    if(!attached[target] && !pwm_pin->cap.servo_pwm && pwm_pin->pin == servo_gpio[target]) {

        servos[target].port = port;

        if(init_servo_default(&servos[target])) {
            pwm_config_t config = {
                .freq_hz = DEFAULT_PWM_FREQ,
                .min = DEFAULT_MIN_ANGLE,
                .max = DEFAULT_MAX_ANGLE,
                .off_value = -1.0f, // Never turn off
                .min_value = DEFAULT_MIN_PULSE_WIDTH * DEFAULT_PWM_FREQ * 100.0f,
                .max_value = DEFAULT_MAX_PULSE_WIDTH * DEFAULT_PWM_FREQ * 100.0f,
                .invert = Off,
                .servo_mode = On
            };

            if(pwm_pin->config(pwm_pin, &config, false)) {
                if(pwm_pin->get_value)
                    memcpy(&servos[target].xport, pwm_pin, sizeof(xbar_t));

                // Use description from array, but ensure we don't go out of bounds
                const char *description = target < sizeof(descr) / sizeof(descr[0]) ? descr[target] : "PWM Servo";
                ioport_set_description(Port_Analog, Port_Output, port, description);

                attached[target] = true;
                n_servos++;
                pwm_servo_set_angle(target, 0.0f);
            }
        }
    }

    // Return true only when target got attached to stop enumeration
    return attached[target];
}

void pwm_servo_init (void)
{
    memcpy(&user_mcode, &grbl.user_mcode, sizeof(user_mcode_ptrs_t));

    grbl.user_mcode.check = mcode_check;
    grbl.user_mcode.validate = mcode_validate;
    grbl.user_mcode.execute = mcode_execute;

    // Do not attach at startup; attach lazily on first M280 use

    on_report_options = grbl.on_report_options;
    grbl.on_report_options = onReportOptions;
}

#endif // PWM_SERVO_ENABLE
