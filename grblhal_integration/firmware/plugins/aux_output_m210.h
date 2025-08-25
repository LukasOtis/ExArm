/*
  aux_output_m210.h - plugin for M210-M215, auxiliary output control commands

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

#ifndef _AUX_OUTPUT_M210_H_
#define _AUX_OUTPUT_M210_H_

#include "driver.h"

#if AUX_OUTPUT_ENABLE == 1

void aux_output_init (void);

#endif // AUX_OUTPUT_ENABLE

#endif // _AUX_OUTPUT_M210_H_
