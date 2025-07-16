# Development Environment Setup Guide

## Overview
This guide covers setting up Cursor IDE for ROS2 robot arm development. **Option 1 (Direct RPi Development) is recommended** as it eliminates VM overhead and provides direct hardware access.

## 🎯 **Option 1: Direct RPi Development (RECOMMENDED)**

### **Why This Approach?**
- ✅ **No VM needed** - develop directly on RPi
- ✅ **Direct hardware access** - immediate testing
- ✅ **Faster development** - no virtualization overhead
- ✅ **Simpler setup** - single environment
- ✅ **Real-time feedback** - see hardware response instantly

### **Step 1: Set up RPi for Direct Development**

```bash
# On Raspberry Pi
chmod +x scripts/setup_direct_rpi_dev.sh
./scripts/setup_direct_rpi_dev.sh
```

This script will:
- Install ROS2 Humble on RPi
- Enable SSH for remote access
- Create development workspace
- Install all necessary tools
- Configure VS Code settings

### **Step 2: Configure Cursor for Remote Development**

1. **Install Cursor on your main computer** (Windows/Mac/Linux)
2. **Install Remote-SSH Extension**
   - Open Cursor
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "Remote - SSH"
   - Install the extension

3. **Connect to RPi**
   ```bash
   # In Cursor, press Ctrl+Shift+P
   # Type: "Remote-SSH: Connect to Host"
   # Add new SSH target:
   ssh pi@raspberry-pi-ip
   ```

4. **Open workspace on RPi**
   ```bash
   # Navigate to workspace
   cd ~/robot_arm_ws
   ```

### **Step 3: Install ROS2 Extensions on RPi**

In Cursor connected to RPi, install these extensions:
- **ROS2** (by Microsoft)
- **Python**
- **C/C++**
- **CMake Tools**

### **Step 4: Test the Setup**

```bash
# On RPi via Cursor remote connection
cd ~/robot_arm_ws
source install/setup.bash
ros2 launch robot_arm_description display.launch.py
```

## 🔄 **Option 2: Hybrid Development (VM + RPi)**

### **When to Use This Approach**
- You need **simulation testing** (Gazebo)
- Your **RPi is underpowered** for development
- You want **offline development** capability
- You need **faster compilation** on VM

### **Setup Process**

```bash
# On VM
./scripts/setup_hybrid_dev.sh

# On RPi
./scripts/setup_rpi.sh

# Deploy from VM to RPi
./scripts/deploy_to_rpi.sh
```

## 🚀 **Quick Start: Direct RPi Development**

### **1. RPi Setup (One-time)**
```bash
# Copy setup script to RPi
scp scripts/setup_direct_rpi_dev.sh pi@raspberry-pi-ip:~/

# On RPi
chmod +x setup_direct_rpi_dev.sh
./setup_direct_rpi_dev.sh
```

### **2. Cursor Setup**
```bash
# Install Cursor on your main computer
# Install Remote-SSH extension
# Connect to RPi: ssh pi@raspberry-pi-ip
# Open workspace: ~/robot_arm_ws
```

### **3. Development Workflow**
```bash
# Edit code directly on RPi via Cursor
# Build: colcon build
# Test: ros2 launch robot_arm_description display.launch.py
# Hardware testing: Immediate
```

## 📊 **Comparison of Approaches**

| Feature | Direct RPi | Hybrid (VM+RPi) |
|---------|------------|------------------|
| **Setup Complexity** | ⭐ Simple | ⭐⭐⭐ Complex |
| **Hardware Access** | ⭐⭐⭐ Direct | ⭐⭐ Deployed |
| **Compilation Speed** | ⭐⭐ Fast | ⭐⭐⭐ Faster |
| **Simulation** | ⭐ Limited | ⭐⭐⭐ Full |
| **Offline Development** | ❌ No | ✅ Yes |
| **Resource Usage** | ⭐ Low | ⭐⭐ Medium |

## 🎯 **Recommended for Your Project**

Given your requirements:
- **5-6 DOF excavator-style arm**
- **Stepper motors with encoders**
- **20 hours/week development**
- **Hardware testing needed**

**Direct RPi Development is ideal** because:
1. **Immediate hardware feedback** - test motor control instantly
2. **No deployment delays** - code changes take effect immediately
3. **Simpler workflow** - single environment to maintain
4. **Real-time debugging** - see hardware issues immediately

## 🔧 **Cursor IDE Configuration**

### **Recommended Extensions**
```json
{
    "recommendations": [
        "ms-vscode.cpptools",
        "ms-python.python",
        "ms-vscode.cmake-tools",
        "ms-iot-vscode.vscode-ros",
        "ms-vscode-remote.remote-ssh"
    ]
}
```

### **Workspace Settings**
```json
{
    "cmake.configureOnOpen": true,
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "python.defaultInterpreterPath": "/usr/bin/python3",
    "files.exclude": {
        "**/build": true,
        "**/install": true,
        "**/log": true
    }
}
```

## 🚀 **Development Workflow**

### **Daily Process (Direct RPi)**
```bash
# 1. Connect to RPi via Cursor remote SSH
# 2. Edit code directly on RPi
# 3. Build: colcon build
# 4. Test: ros2 launch robot_arm_description display.launch.py
# 5. Hardware testing: Immediate
```

### **Testing Strategy**
- **Unit Tests**: Run on RPi
- **Integration Tests**: Run on RPi
- **Hardware Tests**: Run on RPi (immediate)
- **Simulation Tests**: Run on RPi (limited Gazebo)

## 🛠️ **Troubleshooting**

### **Common Issues**

1. **SSH Connection Failed**
   ```bash
   # Check RPi SSH service
   sudo systemctl status ssh
   
   # Check network connectivity
   ping raspberry-pi-ip
   ```

2. **Build Errors on RPi**
   ```bash
   # Check ROS2 installation
   ros2 --version
   
   # Check dependencies
   rosdep install --from-paths src --ignore-src -r -y
   ```

3. **Cursor Remote Issues**
   ```bash
   # Check SSH key setup
   ssh-copy-id pi@raspberry-pi-ip
   
   # Test SSH connection
   ssh pi@raspberry-pi-ip "echo 'Connection successful'"
   ```

## 📋 **Next Steps**

1. **Choose your approach**: Direct RPi (recommended) or Hybrid
2. **Set up RPi**: Run the appropriate setup script
3. **Configure Cursor**: Install Remote-SSH extension
4. **Connect and test**: Verify everything works
5. **Start development**: Begin coding your robot arm!

## 🎯 **Success Criteria**

You have successfully set up the environment when:
- [ ] RPi has ROS2 installed and working
- [ ] Cursor can connect to RPi via SSH
- [ ] Robot arm model displays in RViz
- [ ] Code builds successfully on RPi
- [ ] Hardware testing is possible immediately