# Development Environment Setup Guide

## Overview
This guide covers setting up Cursor IDE for both virtual machine development and Raspberry Pi deployment.

## Option 1: Remote Development (Recommended)

### Step 1: Set up RPi for Remote Development

```bash
# On Raspberry Pi
sudo apt update
sudo apt install openssh-server
sudo systemctl enable ssh
sudo systemctl start ssh

# Get RPi IP address
hostname -I

# Install ROS2 on RPi (same as VM setup)
sudo apt install ros-humble-desktop
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
```

### Step 2: Configure Cursor for Remote Development

1. **Install Cursor on your VM**
2. **Install Remote Development Extension**
   - Open Cursor
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "Remote - SSH"
   - Install the extension

3. **Connect to RPi**
   ```bash
   # In Cursor, press Ctrl+Shift+P
   # Type: "Remote-SSH: Connect to Host"
   # Add new SSH target:
   ssh username@raspberry-pi-ip
   ```

4. **Set up workspace on RPi**
   ```bash
   # On RPi via Cursor remote connection
   mkdir -p ~/robot_arm_ws/src
   cd ~/robot_arm_ws
   colcon build
   ```

### Step 3: Install ROS2 Extensions on RPi

```bash
# In Cursor connected to RPi
# Install these extensions:
# - ROS2 (by Microsoft)
# - Python
# - C/C++
# - CMake Tools
```

## Option 2: Cross-Platform Development

### Step 1: VM Development Environment

```bash
# On VM - Development workspace
mkdir -p ~/robot_arm_dev/src
cd ~/robot_arm_dev

# Install ROS2 and development tools
sudo apt install ros-humble-desktop
sudo apt install python3-colcon-common-extensions
```

### Step 2: RPi Production Environment

```bash
# On RPi - Production workspace
mkdir -p ~/robot_arm_prod/src
cd ~/robot_arm_prod

# Install ROS2 (minimal for production)
sudo apt install ros-humble-ros-base
sudo apt install python3-colcon-common-extensions
```

### Step 3: Synchronization Script

Create a deployment script to sync code from VM to RPi:

```bash
#!/bin/bash
# scripts/deploy_to_rpi.sh

RPI_USER="pi"
RPI_HOST="192.168.1.100"  # Change to your RPi IP
DEV_DIR="~/robot_arm_dev"
PROD_DIR="~/robot_arm_prod"

echo "Deploying to Raspberry Pi..."

# Build on VM
cd ~/robot_arm_dev
colcon build

# Sync to RPi
rsync -avz --delete \
    --exclude='build/' \
    --exclude='install/' \
    --exclude='log/' \
    --exclude='.git/' \
    $DEV_DIR/ $RPI_USER@$RPI_HOST:$PROD_DIR/

# Build on RPi
ssh $RPI_USER@$RPI_HOST "cd $PROD_DIR && colcon build"

echo "Deployment complete!"
```

## Option 3: Hybrid Approach (Best of Both)

### Development Workflow

1. **VM for Development**
   - Faster compilation
   - Better debugging tools
   - Simulation testing
   - Code editing and version control

2. **RPi for Testing**
   - Hardware integration testing
   - Real-world performance testing
   - Production deployment

### Setup Scripts

```bash
# scripts/setup_hybrid_dev.sh
#!/bin/bash

echo "Setting up hybrid development environment..."

# VM Development Setup
echo "Setting up VM development environment..."
mkdir -p ~/robot_arm_dev/src
cd ~/robot_arm_dev

# Install development tools
sudo apt install ros-humble-desktop
sudo apt install python3-colcon-common-extensions
sudo apt install ros-humble-gazebo-ros-pkgs
sudo apt install ros-humble-rviz2

# Create deployment script
cat > scripts/deploy_to_rpi.sh << 'EOF'
#!/bin/bash
RPI_USER="pi"
RPI_HOST="192.168.1.100"
DEV_DIR="~/robot_arm_dev"
PROD_DIR="~/robot_arm_prod"

echo "Deploying to Raspberry Pi..."

# Build on VM
cd ~/robot_arm_dev
colcon build

# Sync to RPi
rsync -avz --delete \
    --exclude='build/' \
    --exclude='install/' \
    --exclude='log/' \
    --exclude='.git/' \
    $DEV_DIR/ $RPI_USER@$RPI_HOST:$PROD_DIR/

# Build on RPi
ssh $RPI_USER@$RPI_HOST "cd $PROD_DIR && colcon build"

echo "Deployment complete!"
EOF

chmod +x scripts/deploy_to_rpi.sh

echo "Hybrid development environment setup complete!"
```

## Cursor IDE Configuration

### Recommended Extensions

```json
// .vscode/extensions.json
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
```

### Workspace Settings

```json
// .vscode/settings.json
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
```

## Development Workflow

### Daily Development Process

1. **VM Development**
   ```bash
   # On VM
   cd ~/robot_arm_dev
   source install/setup.bash
   
   # Edit code in Cursor
   # Test in simulation
   colcon build
   ```

2. **Deploy to RPi**
   ```bash
   # Deploy changes
   ./scripts/deploy_to_rpi.sh
   ```

3. **Test on RPi**
   ```bash
   # SSH to RPi or use Cursor remote
   cd ~/robot_arm_prod
   source install/setup.bash
   ros2 launch robot_arm_description display.launch.py
   ```

### Testing Strategy

- **Unit Tests**: Run on VM
- **Integration Tests**: Run on RPi
- **Simulation Tests**: Run on VM
- **Hardware Tests**: Run on RPi

## Performance Considerations

### VM Development
- **Pros**: Faster compilation, better debugging
- **Cons**: No hardware access

### RPi Development
- **Pros**: Direct hardware access, real-world testing
- **Cons**: Slower compilation, limited resources

### Hybrid Approach
- **Pros**: Best of both worlds
- **Cons**: More complex setup, synchronization overhead

## Troubleshooting

### Common Issues

1. **SSH Connection Failed**
   ```bash
   # Check RPi SSH service
   sudo systemctl status ssh
   
   # Check firewall
   sudo ufw status
   ```

2. **Build Errors on RPi**
   ```bash
   # Check ROS2 installation
   ros2 --version
   
   # Check dependencies
   rosdep install --from-paths src --ignore-src -r -y
   ```

3. **Sync Issues**
   ```bash
   # Check rsync installation
   sudo apt install rsync
   
   # Test connection
   ssh pi@raspberry-pi-ip "echo 'Connection successful'"
   ```

## Recommended Setup for Your Project

Given your requirements (VM development, RPi deployment), I recommend:

1. **Start with Option 1 (Remote Development)** for initial setup
2. **Move to Option 3 (Hybrid)** as project grows
3. **Use VM for simulation and development**
4. **Use RPi for hardware testing and deployment**

This gives you the best balance of development speed and hardware access.