#!/bin/bash

# ROS2 Robot Arm Hybrid Development Setup
# Sets up both VM development and RPi production environments

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== ROS2 Robot Arm Hybrid Development Setup ===${NC}"
echo ""

# Configuration
DEV_DIR="$HOME/robot_arm_dev"
PROD_DIR="$HOME/robot_arm_prod"

echo -e "${BLUE}Setting up development environment on VM...${NC}"

# Create development directory
mkdir -p "$DEV_DIR/src"
cd "$DEV_DIR"

# Install ROS2 and development tools
echo -e "${YELLOW}Installing ROS2 and development tools...${NC}"
sudo apt update
sudo apt install -y ros-humble-desktop
sudo apt install -y python3-colcon-common-extensions
sudo apt install -y ros-humble-gazebo-ros-pkgs
sudo apt install -y ros-humble-rviz2
sudo apt install -y ros-humble-joint-state-publisher-gui

# Install additional development tools
echo -e "${YELLOW}Installing additional development tools...${NC}"
sudo apt install -y build-essential cmake git python3-pip
sudo apt install -y python3-vcstool

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

# Copy project files to development workspace
echo -e "${YELLOW}Setting up project structure...${NC}"
if [ -d "ros2_workspace/src" ]; then
    cp -r ros2_workspace/src/* src/
fi

# Create VS Code configuration
echo -e "${YELLOW}Creating VS Code configuration...${NC}"
mkdir -p .vscode

cat > .vscode/extensions.json << 'EOF'
{
    "recommendations": [
        "ms-vscode.cpptools",
        "ms-python.python",
        "ms-vscode.cmake-tools",
        "ms-iot-vscode.vscode-ros",
        "ms-vscode-remote.remote-ssh",
        "ms-vscode-remote.remote-containers",
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
    }
}
EOF

# Build development workspace
echo -e "${YELLOW}Building development workspace...${NC}"
source /opt/ros/humble/setup.bash
colcon build

# Create deployment script
echo -e "${YELLOW}Creating deployment script...${NC}"
mkdir -p scripts

cat > scripts/deploy_to_rpi.sh << 'EOF'
#!/bin/bash

# ROS2 Robot Arm Deployment Script
# Syncs code from VM development environment to RPi production environment

set -e  # Exit on any error

# Configuration - Update these values
RPI_USER="pi"
RPI_HOST="192.168.1.100"  # Change to your RPi IP address
DEV_DIR="$HOME/robot_arm_dev"
PROD_DIR="$HOME/robot_arm_prod"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== ROS2 Robot Arm Deployment Script ===${NC}"
echo ""

# Check if RPi is reachable
echo -e "${YELLOW}Checking RPi connectivity...${NC}"
if ! ping -c 1 $RPI_HOST > /dev/null 2>&1; then
    echo -e "${RED}Error: Cannot reach RPi at $RPI_HOST${NC}"
    echo "Please check:"
    echo "1. RPi is powered on and connected to network"
    echo "2. IP address is correct: $RPI_HOST"
    echo "3. Network connectivity"
    exit 1
fi

# Check SSH connection
echo -e "${YELLOW}Testing SSH connection...${NC}"
if ! ssh -o ConnectTimeout=5 $RPI_USER@$RPI_HOST "echo 'SSH connection successful'" > /dev/null 2>&1; then
    echo -e "${RED}Error: Cannot connect to RPi via SSH${NC}"
    echo "Please check:"
    echo "1. SSH is enabled on RPi: sudo systemctl enable ssh"
    echo "2. SSH service is running: sudo systemctl status ssh"
    echo "3. Username is correct: $RPI_USER"
    echo "4. SSH key is set up or password authentication is enabled"
    exit 1
fi

# Check if development directory exists
if [ ! -d "$DEV_DIR" ]; then
    echo -e "${RED}Error: Development directory not found: $DEV_DIR${NC}"
    echo "Please run the setup script first: ./scripts/setup_hybrid_dev.sh"
    exit 1
fi

# Build on VM
echo -e "${YELLOW}Building on VM...${NC}"
cd "$DEV_DIR"
if ! colcon build; then
    echo -e "${RED}Error: Build failed on VM${NC}"
    exit 1
fi

# Create production directory on RPi if it doesn't exist
echo -e "${YELLOW}Setting up production directory on RPi...${NC}"
ssh $RPI_USER@$RPI_HOST "mkdir -p $PROD_DIR"

# Sync code to RPi
echo -e "${YELLOW}Syncing code to RPi...${NC}"
rsync -avz --delete \
    --exclude='build/' \
    --exclude='install/' \
    --exclude='log/' \
    --exclude='.git/' \
    --exclude='.vscode/' \
    --exclude='*.swp' \
    --exclude='*~' \
    "$DEV_DIR/" "$RPI_USER@$RPI_HOST:$PROD_DIR/"

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Sync failed${NC}"
    exit 1
fi

# Build on RPi
echo -e "${YELLOW}Building on RPi...${NC}"
ssh $RPI_USER@$RPI_HOST "cd $PROD_DIR && source /opt/ros/humble/setup.bash && colcon build"

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Build failed on RPi${NC}"
    echo "Check RPi logs for details"
    exit 1
fi

# Test deployment
echo -e "${YELLOW}Testing deployment...${NC}"
ssh $RPI_USER@$RPI_HOST "cd $PROD_DIR && source install/setup.bash && ros2 pkg list | grep robot_arm"

if [ $? -eq 0 ]; then
    echo -e "${GREEN}Deployment successful!${NC}"
    echo ""
    echo "Next steps:"
    echo "1. SSH to RPi: ssh $RPI_USER@$RPI_HOST"
    echo "2. Navigate to workspace: cd $PROD_DIR"
    echo "3. Source environment: source install/setup.bash"
    echo "4. Test robot: ros2 launch robot_arm_description display.launch.py"
else
    echo -e "${RED}Error: Deployment test failed${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}=== Deployment Complete ===${NC}"
EOF

chmod +x scripts/deploy_to_rpi.sh

# Create RPi setup script
echo -e "${YELLOW}Creating RPi setup script...${NC}"
cat > scripts/setup_rpi.sh << 'EOF'
#!/bin/bash

# RPi Setup Script
# Run this on the Raspberry Pi to set up the production environment

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${GREEN}=== RPi ROS2 Setup ===${NC}"

# Update system
echo -e "${YELLOW}Updating system...${NC}"
sudo apt update && sudo apt upgrade -y

# Install ROS2
echo -e "${YELLOW}Installing ROS2...${NC}"
sudo apt install -y curl gnupg2 lsb-release
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
sudo apt update
sudo apt install -y ros-humble-ros-base

# Install development tools
echo -e "${YELLOW}Installing development tools...${NC}"
sudo apt install -y python3-colcon-common-extensions python3-rosdep
sudo apt install -y build-essential cmake git

# Set up environment
echo -e "${YELLOW}Setting up environment...${NC}"
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source /opt/ros/humble/setup.bash

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

echo -e "${GREEN}RPi setup complete!${NC}"
echo "RPi IP address: $(hostname -I)"
EOF

chmod +x scripts/setup_rpi.sh

# Test the setup
echo -e "${YELLOW}Testing development environment...${NC}"
source install/setup.bash

# Run basic test
echo -e "${YELLOW}Running basic ROS2 test...${NC}"
timeout 10s ros2 run demo_nodes_cpp talker &
sleep 2
timeout 5s ros2 run demo_nodes_py listener &
wait

echo ""
echo -e "${GREEN}=== Hybrid Development Setup Complete ===${NC}"
echo ""
echo "Development Environment:"
echo "- Location: $DEV_DIR"
echo "- ROS2: Installed and configured"
echo "- Build: Ready"
echo ""
echo "Next Steps:"
echo "1. Update RPi IP in scripts/deploy_to_rpi.sh"
echo "2. Copy scripts/setup_rpi.sh to RPi and run it"
echo "3. Test deployment: ./scripts/deploy_to_rpi.sh"
echo "4. Start development: cd $DEV_DIR && code ."
echo ""
echo "Development Workflow:"
echo "1. Edit code in $DEV_DIR"
echo "2. Build: colcon build"
echo "3. Test in simulation"
echo "4. Deploy: ./scripts/deploy_to_rpi.sh"
echo "5. Test on RPi hardware"