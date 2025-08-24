### For Host Build (Testing)

## Building the Firmware

### Step 1: Navigate to Firmware Directory
```bash
cd grblhal_integration/firmware
```

### Step 2: Build with CMake
```bash
mkdir -p build
cd build
cmake ..
make -j4
```

### Step 3: Flash to Pico 2
1. Hold BOOTSEL button on Pico 2 while connecting USB
2. Pico 2 will appear as USB mass storage device
3. Copy the generated `.uf2` file to the Pico 2 drive
4. Pico 2 will automatically restart and run the firmware

### Troubleshooting Build Issues
- Ensure PICO_SDK_PATH is set correctly
- Run `make clean` before rebuilding
- Check that all source files are present in the grbl/ directory
