#include "grbllib.h"

// Main entry point for grblHAL on Raspberry Pi Pico
// This is the robot arm specific implementation

int main(void) {
    // Initialize and enter grblHAL
    grbl_enter();
    
    // grbl_enter() runs the main protocol loop and never returns
    // under normal operation
    return 0;
} 