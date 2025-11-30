# ROS2 Robot Arm Control System

## Project Overview
A 5-6 DOF excavator-style robot arm with linear acutators, controlled via ROS2 on Raspberry Pi with Pico handling motor control.

## System Architecture
- **Control System**: Raspberry Pi running ROS2 Humble
- **Motor Control**: Raspberry Pi Pico (V2: RP2350B) with custom GRBL firmware
- **Actuators**: Stepper motors with integrated drivers and encoders
- **Workspace**: 0.75-1m reach, 2-3kg payload capacity
- **Communication**: Serial UART between RPi and Pico

## Project Structure
```
robot_arm_control/
├── docs/                          # Documentation
├── grblhal_integration            # GRBL Frimware files (customized)
├── hardware/                      # Hardware design files
├── simulation/                    # Gazebo simulation files
├── ros2_workspace/               # ROS2 workspace
│   └── src/
│       ├── robot_arm_description/ # URDF/XACRO files
│       ├── robot_arm_control/    # Control nodes
│       ├── robot_arm_hardware/   # Hardware interface
│       ├── robot_arm_gui/        # Manual control GUI
│       └── robot_arm_planning/   # Path planning
└── scripts/                      # Setup and utility scripts
```


## Current Status
- Hardware design in CAD (ongoing)
- Starting ROS2 environment setup
- Project structure planning

## Next Steps
1. Set up RPi with ROS2 (run `./scripts/setup_direct_rpi_dev.sh`)
2. Install Cursor IDE and Remote-SSH extension
3. Connect to RPi via SSH with X11 forwarding: `ssh -X pi@raspberry-pi-ip`
4. Test robot model in RViz (appears on your main computer)
5. Begin hardware interface development with full visualization support
