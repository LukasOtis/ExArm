/*
 * Robot Arm Configuration Overlay
 * Disables CNC-specific features and optimizes for robot arm control
 * Include this after config.h to override CNC-specific defaults
 */

#ifndef _ROBOT_ARM_CONFIG_H_
#define _ROBOT_ARM_CONFIG_H_

// ============================================================================
// DISABLE CNC-SPECIFIC FEATURES
// ============================================================================

// Disable laser mode completely
#undef DEFAULT_LASER_MODE
#define DEFAULT_LASER_MODE Off

#undef DEFAULT_DISABLE_LASER_DURING_HOLD
#define DEFAULT_DISABLE_LASER_DURING_HOLD Off

#undef DEFAULT_PWM_SPINDLE_DISABLE_LASER_MODE  
#define DEFAULT_PWM_SPINDLE_DISABLE_LASER_MODE Off

// Disable automatic tool change features (not needed for robot arm)
#undef DEFAULT_TOOLCHANGE_MODE
#define DEFAULT_TOOLCHANGE_MODE 0  // Manual mode only

#undef DEFAULT_TOOLCHANGE_PROBING_DISTANCE
#define DEFAULT_TOOLCHANGE_PROBING_DISTANCE 0.0

#undef DEFAULT_TOOLCHANGE_NO_RESTORE_POSITION
#define DEFAULT_TOOLCHANGE_NO_RESTORE_POSITION On  // Don't auto-restore position

#undef DEFAULT_TOOLCHANGE_AT_G30
#define DEFAULT_TOOLCHANGE_AT_G30 Off

// ============================================================================
// OPTIMIZE FOR ROBOT ARM CONTROL
// ============================================================================

// Set appropriate axis count for 6-DOF robot arm
#undef N_AXIS
#define N_AXIS 6

// Enable arc tolerance for smooth robot trajectories
#undef DEFAULT_ARC_TOLERANCE  
#define DEFAULT_ARC_TOLERANCE 0.02f  // 0.02mm for smooth motion

// Junction deviation for smooth multi-axis moves
#undef DEFAULT_JUNCTION_DEVIATION
#define DEFAULT_JUNCTION_DEVIATION 0.02f  // Smooth joint coordination

// Optimize step pulse timing for robot arm steppers
#undef DEFAULT_STEP_PULSE_MICROSECONDS
#define DEFAULT_STEP_PULSE_MICROSECONDS 2.0f  // Minimum for reliable stepper control

// Homing configuration for robot arm safety
#undef DEFAULT_HOMING_ENABLE 
#define DEFAULT_HOMING_ENABLE (bit(X_AXIS)|bit(Y_AXIS)|bit(Z_AXIS)|bit(A_AXIS)|bit(B_AXIS))  // Enable for all steppers

#undef DEFAULT_HOMING_DIR_MASK
#define DEFAULT_HOMING_DIR_MASK 0  // All axes home in positive direction (customize as needed)

// Soft limits for robot arm workspace safety
#undef DEFAULT_SOFT_LIMIT_ENABLE
#define DEFAULT_SOFT_LIMIT_ENABLE On  // Essential for robot arm safety

// Hard limits for emergency stops
#undef DEFAULT_HARD_LIMIT_ENABLE
#define DEFAULT_HARD_LIMIT_ENABLE On  // Hardware emergency stops

// Disable sleep mode for robot arm (should always be ready)
#undef DEFAULT_SLEEP_ENABLE
#define DEFAULT_SLEEP_ENABLE Off

// Status reporting optimized for robot arm control
#undef DEFAULT_STATUS_REPORT_MASK
#define DEFAULT_STATUS_REPORT_MASK (REPORT_MACHINE_POSITION|REPORT_WORK_POSITION|REPORT_PLANNER_BUFFER|REPORT_RUN_STATE)

// ============================================================================
// AXIS CONFIGURATION FOR 6-DOF ROBOT ARM 
// ============================================================================

// Define axis letters for robot arm joints
#define AXIS_SETTINGS_BASE     0  // X_AXIS - Base rotation
#define AXIS_SETTINGS_SHOULDER 1  // Y_AXIS - Shoulder 
#define AXIS_SETTINGS_ELBOW    2  // Z_AXIS - Elbow
#define AXIS_SETTINGS_LINEAR1  3  // A_AXIS - Linear actuator 1
#define AXIS_SETTINGS_LINEAR2  4  // B_AXIS - Linear actuator 2  
#define AXIS_SETTINGS_SERVO    5  // C_AXIS - End effector servo

// Robot arm specific motion parameters
#undef DEFAULT_HOMING_FEED_RATE
#define DEFAULT_HOMING_FEED_RATE 600.0f  // deg/min for rotational joints

#undef DEFAULT_HOMING_SEEK_RATE  
#define DEFAULT_HOMING_SEEK_RATE 1200.0f  // Fast homing for robot arm

#undef DEFAULT_HOMING_DEBOUNCE_DELAY
#define DEFAULT_HOMING_DEBOUNCE_DELAY 250  // ms for mechanical limit switches

#undef DEFAULT_HOMING_PULLOFF
#define DEFAULT_HOMING_PULLOFF 2.0f  // degrees or mm pulloff distance

// ============================================================================
// ROBOT ARM SAFETY FEATURES
// ============================================================================

// Force initialization alarm for safety
#undef DEFAULT_FORCE_INITIALIZATION_ALARM
#define DEFAULT_FORCE_INITIALIZATION_ALARM On  // Must home before use

// Unlock after E-stop for safety protocol
#undef DEFAULT_UNLOCK_AFTER_ESTOP
#define DEFAULT_UNLOCK_AFTER_ESTOP On

// Reset actions for robot arm
#undef DEFAULT_RESET_ACTIONS_CLEAR_HOMED
#define DEFAULT_RESET_ACTIONS_CLEAR_HOMED On  // Clear homed status on reset

// Enable parking override for robot arm positioning
#undef DEFAULT_PARKING_ENABLE
#define DEFAULT_PARKING_ENABLE Off  // Disable parking for robot arm

// ============================================================================
// COMMUNICATION SETTINGS
// ============================================================================

// Status report interval for real-time robot control
#undef DEFAULT_REPORT_INTERVAL  
#define DEFAULT_REPORT_INTERVAL 250  // ms - 4Hz for smooth ROS2 integration

// Enable protocol extensions for robot arm control
#undef DEFAULT_LEGACY_RT_COMMANDS
#define DEFAULT_LEGACY_RT_COMMANDS Off  // Use modern command set

#endif // _ROBOT_ARM_CONFIG_H_ 