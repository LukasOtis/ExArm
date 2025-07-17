cmake_minimum_required(VERSION 3.13)

# Include Pico SDK and initialize for RP2350
include(pico_sdk_import.cmake)

# Project configuration
project(grblhal_robot_arm C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize Pico SDK
pico_sdk_init()

# ============================================================================
# SOURCE FILES
# ============================================================================

# grblHAL core source files (minimal set for robot arm)
set(GRBL_SOURCES
    grbl/main.c
    grbl/grbllib.c
    grbl/gcode.c
    grbl/motion_control.c
    grbl/planner.c
    grbl/stepper.c
    grbl/protocol.c
    grbl/settings.c
    grbl/system.c
    grbl/report.c
    grbl/nuts_bolts.c
    grbl/machine_limits.c
    grbl/override.c
    grbl/state_machine.c
    grbl/crossbar.c
    grbl/stream.c
    grbl/coolant_control.c
    grbl/spindle_control.c
    grbl/pid.c
    grbl/crc.c
    grbl/nvs_buffer.c
    grbl/ngc_params.c
    grbl/ngc_expr.c
    grbl/ngc_flowctrl.c
    grbl/regex.c
    grbl/strutils.c
    grbl/sleep.c
    grbl/tool_change.c
    grbl/stepper2.c
    grbl/modbus.c
    grbl/modbus_rtu.c
    grbl/stream_file.c
    grbl/stream_passthru.c
    grbl/vfs.c
    grbl/errors.c
    grbl/alarms.c
    grbl/messages.c
)

# RP2350 driver source files  
set(DRIVER_SOURCES
    driver.c
    serial.c
    usb_serial.c
    stdio_usb_descriptors.c
    i2c.c
    spi.c
    ioports.c
)

# PIO sources for hardware timers and step generation
set(PIO_SOURCES
    driverPIO.pio
)

# ============================================================================
# TARGET CONFIGURATION
# ============================================================================

# Create main executable target
add_executable(grblhal_robot_arm
    main.c
    ${GRBL_SOURCES}
    ${DRIVER_SOURCES}
)

# Generate PIO headers
pico_generate_pio_header(grblhal_robot_arm ${CMAKE_CURRENT_LIST_DIR}/driverPIO.pio)

# ============================================================================
# INCLUDES AND DEFINITIONS
# ============================================================================

# Include directories
target_include_directories(grblhal_robot_arm PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/grbl
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/boards
)

# Compiler definitions for robot arm
target_compile_definitions(grblhal_robot_arm PRIVATE
    # grblHAL platform identification
    GRBL_PLATFORM="RobotArm"
    OVERRIDE_MY_MACHINE=1
    
    # RP2350 specific
    PICO_BOARD="pico2"
    
    # Robot arm configuration
    N_AXIS=6
    ROBOT_ARM_BUILD=1
    
    # Hardware configuration
    RP2350_BUILD=1
    USB_SERIAL_CDC=1
    
    # Performance optimizations
    PICO_USE_STACK_GUARDS=1
    PICO_STACK_SIZE=0x2000
)

# ============================================================================
# PICO SDK LIBRARIES  
# ============================================================================

# Link Pico SDK libraries
target_link_libraries(grblhal_robot_arm
    # Core Pico libraries
    pico_stdlib
    pico_platform
    pico_runtime
    
    # Hardware interface libraries
    pico_multicore
    pico_pio
    hardware_pio
    hardware_dma
    hardware_uart
    hardware_i2c
    hardware_spi
    hardware_pwm
    hardware_gpio
    hardware_clocks
    hardware_timer
    hardware_watchdog
    hardware_flash
    hardware_sync
    
    # USB support for communication
    pico_unique_id
    pico_stdio
    pico_stdio_usb
    pico_stdio_uart
    
    # Math libraries for robot kinematics
    pico_float
    pico_double
    hardware_divider
    
    # Memory and storage
    hardware_rtc
    hardware_adc
)

# ============================================================================
# BUILD OUTPUT CONFIGURATION
# ============================================================================

# Enable stdio outputs (USB and UART)
pico_enable_stdio_usb(grblhal_robot_arm 1)
pico_enable_stdio_uart(grblhal_robot_arm 1)

# Generate map file for debugging
pico_set_linker_create_map_file(grblhal_robot_arm 1)

# Create bin/hex/uf2 outputs
pico_add_extra_outputs(grblhal_robot_arm)

# ============================================================================
# COMPILER FLAGS
# ============================================================================

# Optimization and warnings
target_compile_options(grblhal_robot_arm PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter      # grblHAL has many unused parameters due to HAL abstraction
    -Wno-missing-field-initializers  # grblHAL settings structures
    -O2                        # Optimize for performance
    -g                         # Include debug info
    -ffunction-sections        # Enable dead code elimination
    -fdata-sections           # Enable dead data elimination
)

# Linker flags for size optimization
target_link_options(grblhal_robot_arm PRIVATE
    -Wl,--gc-sections         # Remove unused sections
    -Wl,--print-memory-usage  # Show memory usage
)

# ============================================================================
# BUILD INFORMATION
# ============================================================================

# Print build configuration
message(STATUS "=== grblHAL Robot Arm Build Configuration ===")
message(STATUS "Target: Raspberry Pi Pico 2 (RP2350)")
message(STATUS "Axes: 6-DOF (5 steppers + 1 servo)")
message(STATUS "grblHAL core files: ${list_length(GRBL_SOURCES)}")
message(STATUS "Driver files: ${list_length(DRIVER_SOURCES)}")
message(STATUS "Total source files: ${list_length(GRBL_SOURCES + DRIVER_SOURCES)}")
message(STATUS "============================================")

# Custom targets for development
add_custom_target(flash
    DEPENDS grblhal_robot_arm
    COMMAND picotool load grblhal_robot_arm.uf2 -f
    COMMENT "Flashing grblHAL Robot Arm firmware to Pico 2"
)

add_custom_target(monitor
    COMMAND minicom -D /dev/ttyACM0 -b 115200
    COMMENT "Opening serial monitor (adjust device as needed)"
)

add_custom_target(size
    DEPENDS grblhal_robot_arm
    COMMAND ${CMAKE_SIZE} grblhal_robot_arm.elf
    COMMENT "Showing firmware size information"
) 