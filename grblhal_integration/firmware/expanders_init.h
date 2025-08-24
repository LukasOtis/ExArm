/*
 * expanders_init.h - I/O expander initialization for grblHAL
 *
 * This file is required by driver_opts.h but not used for basic 3-axis robot arm.
 * I/O expanders are disabled for this configuration.
 */

#ifndef _EXPANDERS_INIT_H_
#define _EXPANDERS_INIT_H_

// No I/O expanders configured for basic 3-axis robot arm
static inline void io_expanders_init (void) {
    // Empty implementation - no expanders to initialize
}

#endif // _EXPANDERS_INIT_H_
