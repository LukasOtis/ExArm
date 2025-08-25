/*
  pwm_servo_m280.h - plugin for M280, Marlin style servo commands

  Part of grblHAL misc. plugins

  Based on code by @wakass. Public domain.
  https://github.com/wakass/grlbhal_servo

  Usage:
    M280[P<id>][S<position>]

  If no words are specified all servo positions are reported.
  If no position is specified the specific servo position is returned.

  https://marlinfw.org/docs/gcode/M280.html
*/

#ifndef _PWM_SERVO_M280_H_
#define _PWM_SERVO_M280_H_

#include "driver.h"

#if PWM_SERVO_ENABLE == 1

void pwm_servo_init (void);

#endif // PWM_SERVO_ENABLE

#endif // _PWM_SERVO_M280_H_
