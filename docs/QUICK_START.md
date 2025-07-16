# Quick Start Guide - Direct RPi Development

## Prerequisites
- Raspberry Pi (4B recommended) with Ubuntu 22.04 or Raspberry Pi OS
- Main computer (Windows/Mac/Linux) for running Cursor IDE
- Both devices on same network
- Internet connection

## Step 1: RPi Setup (30 minutes)

### Option A: Automated Setup (Recommended)
```bash
# Copy setup script to RPi
scp scripts/setup_direct_rpi_dev.sh pi@raspberry-pi-ip:~/

# On RPi
chmod +x setup_direct_rpi_dev.sh
./setup_direct_rpi_dev.sh
```

### Option B: Manual Setup
```bash
# On RPi
sudo apt update && sudo apt upgrade -y

# Install ROS2 Humble
sudo apt install curl gnupg2 lsb-release
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
sudo apt update
sudo apt install ros-humble-desktop

# Install development tools
sudo apt install python3-colcon-common-extensions python3-rosdep
sudo apt install build-essential cmake git python3-pip
sudo apt install ros-humble-gazebo-ros-pkgs ros-humble-rviz2
sudo apt install ros-humble-joint-state-publisher-gui

# Set up environment
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc

# Initialize rosdep
sudo rosdep init
rosdep update

# Enable SSH
sudo systemctl enable ssh
sudo systemctl start ssh

# Create workspace
mkdir -p ~/robot_arm_ws/src
cd ~/robot_arm_ws
colcon build
```

## Step 2: Cursor IDE Setup (10 minutes)

### Install Cursor on Main Computer
1. **Download Cursor** from https://cursor.sh/
2. **Install Cursor** on your main computer (Windows/Mac/Linux)
3. **Install Remote-SSH Extension**
   - Open Cursor
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "Remote - SSH"
   - Install the extension

### Connect to RPi
```bash
# In Cursor, press Ctrl+Shift+P
# Type: "Remote-SSH: Connect to Host"
# Add new SSH target:
ssh pi@raspberry-pi-ip
```

### Open Workspace on RPi
```bash
# Navigate to workspace
cd ~/robot_arm_ws

# Install ROS2 extensions in Cursor
# - ROS2 (by Microsoft)
# - Python
# - C/C++
# - CMake Tools
```

## Step 3: Project Setup (15 minutes)

### Copy Project Files to RPi
```bash
# From your main computer
scp -r ros2_workspace/src/* pi@raspberry-pi-ip:~/robot_arm_ws/src/

# On RPi via Cursor
cd ~/robot_arm_ws
colcon build
source install/setup.bash
```

## Step 4: Test the Robot Model (5 minutes)

```bash
# Launch the robot visualization
ros2 launch robot_arm_description display.launch.py
```

You should see:
- RViz window with the robot arm model
- Joint State Publisher GUI with sliders
- Ability to move joints using the GUI

## Step 5: Verify Installation (5 minutes)

```bash
# Test ROS2 communication
ros2 run demo_nodes_cpp talker &
ros2 run demo_nodes_py listener &
```

## Development Workflow

### Daily Development Process
```bash
# 1. Connect to RPi via Cursor remote SSH
# 2. Edit code directly on RPi
# 3. Build: colcon build
# 4. Test: ros2 launch robot_arm_description display.launch.py
# 5. Hardware testing: Immediate (no deployment needed)
```

### Testing Strategy
- **Unit Tests**: Run on RPi
- **Integration Tests**: Run on RPi
- **Hardware Tests**: Run on RPi (immediate)
- **Simulation Tests**: Run on RPi (limited Gazebo)

## Common Issues and Solutions

### Issue: SSH Connection Failed
**Solution:**
```bash
# Check RPi SSH service
sudo systemctl status ssh

# Check network connectivity
ping raspberry-pi-ip

# Check SSH key setup
ssh-copy-id pi@raspberry-pi-ip
```

### Issue: RViz doesn't show robot model
**Solution:**
```bash
# Check if robot description is published
ros2 topic list | grep robot_description
ros2 topic echo /robot_description
```

### Issue: Build errors on RPi
**Solution:**
```bash
# Check ROS2 installation
ros2 --version

# Check dependencies
rosdep install --from-paths src --ignore-src -r -y

# Clean and rebuild
rm -rf build/ install/
colcon build
```

### Issue: Cursor remote connection issues
**Solution:**
```bash
# Test SSH connection
ssh pi@raspberry-pi-ip "echo 'Connection successful'"

# Check RPi IP address
hostname -I

# Verify SSH is enabled
sudo systemctl enable ssh
sudo systemctl start ssh
```

## Next Steps

After successful setup:

1. **Explore the robot model** in RViz
2. **Study the URDF file** in `ros2_workspace/src/robot_arm_description/urdf/`
3. **Review the timeline** in `docs/PROJECT_TIMELINE.md`
4. **Plan your development** using `docs/TECHNICAL_ARCHITECTURE.md`

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

# Check RPi system info
htop
df -h
free -h
```

## Performance Tips

### RPi Optimization
```bash
# Disable unnecessary services
sudo systemctl disable bluetooth
sudo systemctl disable avahi-daemon

# Increase swap space if needed
sudo dphys-swapfile swapoff
sudo nano /etc/dphys-swapfile
# Set CONF_SWAPSIZE=2048
sudo dphys-swapfile setup
sudo dphys-swapfile swapon
```

### Network Optimization
```bash
# Use wired connection if possible
# Ensure both devices on same network
# Check network speed: speedtest-cli
```

## Getting Help

- **ROS2 Documentation**: https://docs.ros.org/en/humble/
- **URDF Documentation**: http://wiki.ros.org/urdf
- **RViz Documentation**: http://wiki.ros.org/rviz
- **Project Issues**: Create an issue in the repository

## Success Criteria

You have successfully set up the environment when:
- [ ] RPi has ROS2 installed and working
- [ ] Cursor can connect to RPi via SSH
- [ ] Robot arm model displays in RViz
- [ ] Code builds successfully on RPi
- [ ] Hardware testing is possible immediately

Once these are complete, you're ready to begin Phase 2: Simulation & Design!

## Benefits of This Approach

- ✅ **No VM overhead** - faster development
- ✅ **Direct hardware access** - immediate testing
- ✅ **Real-time feedback** - see hardware response instantly
- ✅ **Simpler setup** - single environment
- ✅ **No deployment delays** - code changes take effect immediately
- ✅ **Cost effective** - no additional VM resources needed