#!/bin/bash

# Direct RPi Development Setup
# Sets up RPi for direct development via Cursor remote SSH

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Direct RPi Development Setup ===${NC}"
echo "This setup allows you to develop directly on RPi via Cursor remote SSH"
echo ""

# Configuration
WORKSPACE_DIR="$HOME/robot_arm_ws"

echo -e "${BLUE}Setting up RPi for direct development...${NC}"

# Update system
echo -e "${YELLOW}Updating system...${NC}"
sudo apt update && sudo apt upgrade -y

# Install ROS2
echo -e "${YELLOW}Installing ROS2 Humble...${NC}"
sudo apt install -y curl gnupg2 lsb-release
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
sudo apt update
sudo apt install -y ros-humble-desktop

# Install development tools
echo -e "${YELLOW}Installing development tools...${NC}"
sudo apt install -y python3-colcon-common-extensions python3-rosdep
sudo apt install -y build-essential cmake git python3-pip
sudo apt install -y ros-humble-gazebo-ros-pkgs ros-humble-rviz2
sudo apt install -y ros-humble-joint-state-publisher-gui

# Install Python dependencies
pip3 install --user vcstool colcon-common-extensions

# Set up environment
echo -e "${YELLOW}Setting up ROS2 environment...${NC}"
if ! grep -q "source /opt/ros/humble/setup.bash" ~/.bashrc; then
    echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
fi

# Initialize rosdep
echo -e "${YELLOW}Initializing rosdep...${NC}"
if [ ! -f /etc/ros/rosdep/sources.list.d/20-default.list ]; then
    sudo rosdep init
fi
rosdep update

# Enable SSH
echo -e "${YELLOW}Enabling SSH...${NC}"
sudo systemctl enable ssh
sudo systemctl start ssh

# Create workspace
echo -e "${YELLOW}Creating workspace...${NC}"
mkdir -p "$WORKSPACE_DIR/src"
cd "$WORKSPACE_DIR"

# Copy project files if available
if [ -d "ros2_workspace/src" ]; then
    echo -e "${YELLOW}Copying project files...${NC}"
    cp -r ros2_workspace/src/* src/
fi

# Create VS Code configuration for remote development
echo -e "${YELLOW}Creating VS Code configuration...${NC}"
mkdir -p .vscode

cat > .vscode/extensions.json << 'EOF'
{
    "recommendations": [
        "ms-vscode.cpptools",
        "ms-python.python",
        "ms-vscode.cmake-tools",
        "ms-iot-vscode.vscode-ros",
        "ms-vscode.remote-explorer"
    ]
}
EOF

cat > .vscode/settings.json << 'EOF'
{
    "cmake.configureOnOpen": true,
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "python.defaultInterpreterPath": "/usr/bin/python3",
    "files.exclude": {
        "**/build": true,
        "**/install": true,
        "**/log": true
    },
    "search.exclude": {
        "**/build": true,
        "**/install": true,
        "**/log": true
    },
    "terminal.integrated.defaultProfile.linux": "bash",
    "terminal.integrated.profiles.linux": {
        "bash": {
            "path": "/bin/bash",
            "args": ["-l"]
        }
    }
}
EOF

# Build workspace
echo -e "${YELLOW}Building workspace...${NC}"
source /opt/ros/humble/setup.bash
colcon build

# Test the setup
echo -e "${YELLOW}Testing ROS2 installation...${NC}"
source install/setup.bash

# Run basic test
echo -e "${YELLOW}Running basic ROS2 test...${NC}"
timeout 10s ros2 run demo_nodes_cpp talker &
sleep 2
timeout 5s ros2 run demo_nodes_py listener &
wait

# Get network information
RPI_IP=$(hostname -I | awk '{print $1}')

echo ""
echo -e "${GREEN}=== Direct RPi Development Setup Complete ===${NC}"
echo ""
echo "RPi Configuration:"
echo "- IP Address: $RPI_IP"
echo "- SSH Enabled: Yes"
echo "- ROS2 Installed: Yes"
echo "- Workspace: $WORKSPACE_DIR"
echo ""
echo "Next Steps:"
echo "1. Install Cursor on your main computer"
echo "2. Install Remote-SSH extension in Cursor"
echo "3. Connect to RPi: ssh pi@$RPI_IP"
echo "4. Open workspace: $WORKSPACE_DIR"
echo ""
echo "Cursor Setup Instructions:"
echo "1. Open Cursor"
echo "2. Press Ctrl+Shift+P"
echo "3. Type: 'Remote-SSH: Connect to Host'"
echo "4. Add new SSH target: ssh pi@$RPI_IP"
echo "5. Navigate to: $WORKSPACE_DIR"
echo ""
echo "Development Workflow:"
echo "1. Connect to RPi via Cursor remote SSH"
echo "2. Edit code directly on RPi"
echo "3. Build: colcon build"
echo "4. Test: ros2 launch robot_arm_description display.launch.py"
echo "5. Hardware testing: Immediate (no deployment needed)"
echo ""
echo "Benefits:"
echo "- No VM overhead"
echo "- Direct hardware access"
echo "- Real-time testing"
echo "- Single development environment"
echo "- Faster compilation"