# Quick Start Guide

## Prerequisites
- Ubuntu 22.04 LTS (virtual machine or native)
- 4GB RAM minimum
- 50GB free disk space
- Internet connection

## Step 1: Environment Setup (30 minutes)

### Option A: Automated Setup (Recommended)
```bash
# Clone the repository
git clone <your-repo-url>
cd robot_arm_control

# Run the setup script
chmod +x scripts/setup_ros2_environment.sh
./scripts/setup_ros2_environment.sh
```

### Option B: Manual Setup
```bash
# Install ROS2 Humble
sudo apt update
sudo apt install curl gnupg2 lsb-release
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
sudo apt update
sudo apt install ros-humble-desktop

# Install development tools
sudo apt install python3-colcon-common-extensions python3-rosdep
sudo rosdep init
rosdep update

# Install simulation tools
sudo apt install ros-humble-gazebo-ros-pkgs ros-humble-rviz2 ros-humble-joint-state-publisher-gui

# Set up environment
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

## Step 2: Project Setup (15 minutes)

```bash
# Create workspace
mkdir -p ~/robot_arm_ws/src
cd ~/robot_arm_ws

# Copy project files
cp -r ros2_workspace/src/* src/

# Build the workspace
colcon build

# Source the workspace
source install/setup.bash
```

## Step 3: Test the Robot Model (5 minutes)

```bash
# Launch the robot visualization
ros2 launch robot_arm_description display.launch.py
```

You should see:
- RViz window with the robot arm model
- Joint State Publisher GUI with sliders
- Ability to move joints using the GUI

## Step 4: Verify Installation (5 minutes)

```bash
# Test ROS2 communication
ros2 run demo_nodes_cpp talker &
ros2 run demo_nodes_py listener &
```

## Common Issues and Solutions

### Issue: RViz doesn't show robot model
**Solution:**
```bash
# Check if robot description is published
ros2 topic list | grep robot_description
ros2 topic echo /robot_description
```

### Issue: Joint State Publisher GUI not working
**Solution:**
```bash
# Install missing package
sudo apt install ros-humble-joint-state-publisher-gui
```

### Issue: Build errors
**Solution:**
```bash
# Clean and rebuild
cd ~/robot_arm_ws
rm -rf build/ install/
colcon build
```

## Next Steps

After successful setup:

1. **Explore the robot model** in RViz
2. **Study the URDF file** in `ros2_workspace/src/robot_arm_description/urdf/`
3. **Review the timeline** in `docs/PROJECT_TIMELINE.md`
4. **Plan your development** using `docs/TECHNICAL_ARCHITECTURE.md`

## Development Workflow

```bash
# Daily development workflow
cd ~/robot_arm_ws
source install/setup.bash

# Make changes to code
# ...

# Build changes
colcon build

# Test changes
ros2 launch robot_arm_description display.launch.py
```

## Useful Commands

```bash
# List all topics
ros2 topic list

# Monitor joint states
ros2 topic echo /joint_states

# Check robot description
ros2 param get /robot_state_publisher robot_description

# Launch RViz manually
ros2 run rviz2 rviz2

# Launch Gazebo simulation
ros2 launch gazebo_ros gazebo.launch.py
```

## Getting Help

- **ROS2 Documentation**: https://docs.ros.org/en/humble/
- **URDF Documentation**: http://wiki.ros.org/urdf
- **RViz Documentation**: http://wiki.ros.org/rviz
- **Project Issues**: Create an issue in the repository

## Success Criteria

You have successfully set up the environment when:
- [ ] ROS2 Humble is installed and working
- [ ] Robot arm model displays in RViz
- [ ] Joint State Publisher GUI works
- [ ] Workspace builds without errors
- [ ] Basic ROS2 communication works

Once these are complete, you're ready to begin Phase 2: Simulation & Design!