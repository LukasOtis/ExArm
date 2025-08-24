/*
  driver_opts2.h - Additional driver options for grblHAL Robot Arm on Pico
  
  This file contains additional driver options that are included after the first
  configuration section in driver.h
*/

#ifndef DRIVER_OPTS2_H
#define DRIVER_OPTS2_H

// ============================================================================
// ADDITIONAL I/O OPTIONS
// ============================================================================

// I2C support
#define I2C_ENABLE 1
#define I2C_PORT 1
#define I2C_SDA 26
#define I2C_SCL 27

// SPI support
#define SPI_ENABLE 1
#define SPI_PORT 1

// SD card support (disabled for robot arm)
#define SDCARD_ENABLE 0

// ============================================================================
// PIN MODE DEFINITIONS
// ============================================================================

#ifndef PINMODE_OUTPUT
#define PINMODE_OUTPUT 0
#endif

#ifndef PINMODE_INPUT
#define PINMODE_INPUT 1
#endif

#ifndef PINMODE_INPUT_PULLUP
#define PINMODE_INPUT_PULLUP 2
#endif

#ifndef PINMODE_INPUT_PULLDOWN
#define PINMODE_INPUT_PULLDOWN 3
#endif

// ============================================================================
// SHIFT REGISTER SUPPORT
// ============================================================================

// Shift register support (disabled for robot arm)
#define OUT_SHIFT_REGISTER 0

// ============================================================================
// EXPANDER SUPPORT
// ============================================================================

// I/O expander support (disabled for robot arm)
#define EXPANDER_ENABLE 0
#define EXPANDER_PORT 0

// ============================================================================
// SPI RESET SUPPORT
// ============================================================================

// SPI reset port (not used)
#define SPI_RST_PORT 0

// ============================================================================
// ADDITIONAL FEATURES
// ============================================================================

// Real-time command support
#define REAL_TIME_COMMANDS_ENABLE 1

// Status query support
#define STATUS_QUERY_ENABLE 1

// Feed hold support
#define FEED_HOLD_ENABLE 1

// Cycle start support
#define CYCLE_START_ENABLE 1

// ============================================================================
// ROBOT ARM SPECIFIC OPTIONS
// ============================================================================

// Joint limit checking
#define JOINT_LIMITS_ENABLE 1

// Collision detection
#define COLLISION_DETECTION_ENABLE 1

// Trajectory planning
#define TRAJECTORY_PLANNING_ENABLE 1

// Kinematics calculation
#define KINEMATICS_CALC_ENABLE 1

// ============================================================================
// DEBUG AND DEVELOPMENT OPTIONS
// ============================================================================

// Error reporting
#define ERROR_REPORTING_ENABLE 1

// Warning reporting
#define WARNING_REPORTING_ENABLE 1

// Info reporting
#define INFO_REPORTING_ENABLE 1

// ============================================================================
// PLATFORM SPECIFIC OPTIONS
// ============================================================================

// Pico-specific GPIO functions
#define PICO_GPIO_ENABLE 1

// PIO state machine support
#define PIO_SM_ENABLE 1

// DMA channel support
#define DMA_CHANNEL_ENABLE 1

// Timer support
#define TIMER_ENABLE 1

// Watchdog support
#define WATCHDOG_ENABLE 1

#endif // DRIVER_OPTS2_H
