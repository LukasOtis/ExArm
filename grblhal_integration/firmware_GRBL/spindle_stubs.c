#include "grbl/hal.h"

// Stub spindle functions to satisfy linker requirements
// These are no-op functions since spindle functionality is not needed

void spindle_off(spindle_t spindle) {
    // No-op: spindle not implemented
    (void)spindle;
}

void spindle_on(spindle_t spindle) {
    // No-op: spindle not implemented
    (void)spindle;
}

void spindle_dir(bool ccw) {
    // No-op: spindle direction not implemented
    (void)ccw;
}
