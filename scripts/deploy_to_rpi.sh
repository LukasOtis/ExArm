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