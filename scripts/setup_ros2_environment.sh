#!/bin/bash

# ROS2 Robot Arm Environment Setup Script
# This script sets up a complete ROS2 development environment

set -e  # Exit on any error

echo "=== ROS2 Robot Arm Environment Setup ==="
echo "This script will install ROS2 Humble and development tools"
echo ""

# Check if running on Ubuntu 22.04
if [[ $(lsb_release -rs) != "22.04" ]]; then
    echo "Warning: This script is designed for Ubuntu 22.04"
    echo "Current version: $(lsb_release -rs)"
    read -p "Continue anyway? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Update system
echo "Updating system packages..."
sudo apt update && sudo apt upgrade -y

# Install required packages
echo "Installing required packages..."
sudo apt install -y curl gnupg2 lsb-release software-properties-common

# Add ROS2 repository
echo "Adding ROS2 repository..."
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# Update package list
sudo apt update

# Install ROS2 Humble
echo "Installing ROS2 Humble..."
sudo apt install -y ros-humble-desktop

# Install additional ROS2 tools
echo "Installing ROS2 development tools..."
sudo apt install -y python3-colcon-common-extensions python3-rosdep

# Initialize rosdep
echo "Initializing rosdep..."
if [ ! -f /etc/ros/rosdep/sources.list.d/20-default.list ]; then
    sudo rosdep init
fi
rosdep update

# Install simulation tools
echo "Installing simulation tools..."
sudo apt install -y ros-humble-gazebo-ros-pkgs ros-humble-rviz2 ros-humble-joint-state-publisher-gui

# Install additional development tools
echo "Installing development tools..."
sudo apt install -y build-essential cmake git python3-pip python3-vcstool

# Install Python dependencies
echo "Installing Python dependencies..."
pip3 install --user vcstool colcon-common-extensions

# Set up environment
echo "Setting up ROS2 environment..."
if ! grep -q "source /opt/ros/humble/setup.bash" ~/.bashrc; then
    echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
fi

# Create workspace
echo "Creating ROS2 workspace..."
mkdir -p ~/robot_arm_ws/src
cd ~/robot_arm_ws

# Source ROS2 environment
source /opt/ros/humble/setup.bash

# Build workspace
echo "Building workspace..."
colcon build

# Install VS Code (optional)
read -p "Install VS Code for development? (y/N): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Installing VS Code..."
    sudo snap install code --classic
fi

# Create project structure
echo "Creating project structure..."
cd ~/robot_arm_ws/src

# Create ROS2 packages
echo "Creating ROS2 packages..."
ros2 pkg create --build-type ament_cmake robot_arm_description --dependencies rclcpp urdf xacro
ros2 pkg create --build-type ament_cmake robot_arm_control --dependencies rclcpp control_msgs trajectory_msgs
ros2 pkg create --build-type ament_cmake robot_arm_hardware --dependencies rclcpp hardware_interface
ros2 pkg create --build-type ament_cmake robot_arm_gui --dependencies rclcpp rclpy rviz2
ros2 pkg create --build-type ament_cmake robot_arm_planning --dependencies rclcpp moveit_ros_planning_interface

# Build packages
cd ~/robot_arm_ws
colcon build

# Test installation
echo "Testing ROS2 installation..."
source install/setup.bash

# Run basic test
echo "Running basic ROS2 test..."
timeout 10s ros2 run demo_nodes_cpp talker &
sleep 2
timeout 5s ros2 run demo_nodes_py listener &
wait

echo ""
echo "=== Setup Complete ==="
echo ""
echo "Next steps:"
echo "1. Source the workspace: source ~/robot_arm_ws/install/setup.bash"
echo "2. Test RViz: ros2 run rviz2 rviz2"
echo "3. Test Gazebo: ros2 launch gazebo_ros gazebo.launch.py"
echo ""
echo "Workspace location: ~/robot_arm_ws"
echo "Packages created in: ~/robot_arm_ws/src"
echo ""
echo "To start development:"
echo "cd ~/robot_arm_ws"
echo "source install/setup.bash"
echo "code .  # If VS Code was installed"