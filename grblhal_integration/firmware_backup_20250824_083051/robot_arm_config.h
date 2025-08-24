/*
  robot_arm_config.h - Robot arm specific configuration for grblHAL
  
  This file overrides CNC-specific features and enables robot arm functionality.
  It should be included after config.h in grbl.h
*/

#ifndef ROBOT_ARM_CONFIG_H
#define ROBOT_ARM_CONFIG_H

// ============================================================================
// ROBOT ARM CONFIGURATION OVERRIDES
// ============================================================================

// Disable CNC-specific features that aren't needed for robot arm
#undef ENABLE_SPINDLE_CONTROL
#undef ENABLE_COOLANT_CONTROL
#undef ENABLE_TOOL_CHANGE
#undef ENABLE_PROBE
#undef ENABLE_SAFETY_DOOR
#undef ENABLE_MPG_MODE
#undef ENABLE_OVERRIDE_CONTROLS

// Enable robot arm specific features
#define ENABLE_ROBOT_ARM_MODE 1
#define ENABLE_JOINT_MODE 1
#define ENABLE_CARTESIAN_MODE 1

// Robot arm specific constants
#define ROBOT_ARM_JOINTS 6
#define ROBOT_ARM_MAX_VELOCITY 1000.0f  // mm/s or deg/s
#define ROBOT_ARM_MAX_ACCELERATION 500.0f  // mm/s² or deg/s²

// Joint limits (in degrees for rotational, mm for linear)
#define JOINT_1_MIN -180.0f  // Base rotation
#define JOINT_1_MAX 180.0f
#define JOINT_2_MIN 0.0f     // Shoulder rotation
#define JOINT_2_MAX 180.0f
#define JOINT_3_MIN 0.0f     // Elbow rotation
#define JOINT_3_MAX 180.0f
#define JOINT_4_MIN 0.0f     // Linear actuator 1
#define JOINT_4_MAX 100.0f
#define JOINT_5_MIN 0.0f     // Linear actuator 2
#define JOINT_5_MAX 100.0f
#define JOINT_6_MIN -180.0f  // End effector rotation
#define JOINT_6_MAX 180.0f

// Pin definitions for robot arm (these should match my_machine_map.h)
#define ROBOT_ARM_STEP_PINS {2, 4, 6, 8, 10}
#define ROBOT_ARM_DIR_PINS {3, 5, 7, 9, 11}
#define ROBOT_ARM_ENABLE_PIN 12
#define ROBOT_ARM_LIMIT_PINS {15, 17, 18, 19, 20}
#define ROBOT_ARM_PWM_PINS {13, 14, 16}

// Robot arm mode definitions
#define MODE_JOINT 0
#define MODE_CARTESIAN 1
#define MODE_TOOL 2

// Safety features
#define ENABLE_JOINT_LIMITS 1
#define ENABLE_COLLISION_DETECTION 1
#define ENABLE_EMERGENCY_STOP 1

// Communication settings
#define ROBOT_ARM_BAUD_RATE 115200
#define ENABLE_ROS2_BRIDGE 1

#endif // ROBOT_ARM_CONFIG_H
