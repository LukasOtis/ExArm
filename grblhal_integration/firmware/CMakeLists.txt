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
    GRBL/grbllib.c
    GRBL/gcode.c
    GRBL/motion_control.c
    GRBL/planner.c
    GRBL/stepper.c
    GRBL/protocol.c
    GRBL/settings.c
    GRBL/system.c
    GRBL/report.c
    GRBL/nuts_bolts.c
    GRBL/machine_limits.c
    GRBL/override.c
    GRBL/state_machine.c
    GRBL/crossbar.c
    GRBL/stream.c
    GRBL/coolant_control.c
    GRBL/spindle_control.c
    GRBL/pid.c
    GRBL/crc.c
    GRBL/nvs_buffer.c
    GRBL/ngc_params.c
    GRBL/ngc_expr.c
    GRBL/ngc_flowctrl.c
    GRBL/regex.c
    GRBL/strutils.c
    GRBL/sleep.c
    GRBL/tool_change.c
    GRBL/stepper2.c
    # GRBL/modbus.c                    # Disabled for basic 3-axis robot arm
    # GRBL/modbus_rtu.c               # Disabled for basic 3-axis robot arm
    GRBL/stream_file.c
    # GRBL/stream_passthru.c          # Disabled for basic 3-axis robot arm
    GRBL/vfs.c
    GRBL/errors.c
    GRBL/alarms.c
    GRBL/messages.c
    GRBL/flash.c
    GRBL/canbus.c
)

# RP2350 driver source files  
set(DRIVER_SOURCES
    driver.c
    serial.c
    usb_serial.c
    i2c.c
    spi.c
    plugins/pwm_servo_m280.c  # PWM Servo plugin for M280 command
    plugins/aux_output_m210.c  # Auxiliary Output plugin for M210-M215 commands
)

# PIO sources for hardware timers and step generation
set(PIO_SOURCES
    driverPIO.pio
    ${CMAKE_CURRENT_LIST_DIR}/pico-sdk/src/rp2_common/pico_status_led/ws2812.pio
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
pico_generate_pio_header(grblhal_robot_arm ${CMAKE_CURRENT_LIST_DIR}/pico-sdk/src/rp2_common/pico_status_led/ws2812.pio)

# ============================================================================
# INCLUDES AND DEFINITIONS
# ============================================================================

# Include directories
target_include_directories(grblhal_robot_arm PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/GRBL
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/boards
    # Pico SDK headers
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/common/pico_base_headers/include
    # ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/common/pico_time/include  # Removed to avoid hardware_rtc dependency
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_base/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_gpio/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/common/pico_stdlib/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/common/pico_sync/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/pico_platform/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_sync/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_irq/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_timer/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_clocks/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_resets/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_pll/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_vreg/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_watchdog/include
    ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_xosc/include
    # ${CMAKE_CURRENT_LIST_DIR}/build/pico-sdk/src/rp2_common/hardware_rtc/include  # Not available for RP2350
)

# Compiler definitions for robot arm
target_compile_definitions(grblhal_robot_arm PRIVATE
    # grblHAL platform identification
    GRBL_PLATFORM="RobotArm"
    BOARD_MY_MACHINE=1
    
    # RP2350 specific (Pico 2)
    PICO_BOARD="pico2"
    RP_MCU=2350
    
    # Robot arm configuration - 3-axis for RP2040
    N_AXIS=3
    N_ABC_MOTORS=0
    ROBOT_ARM_BUILD=1
    
    # Hardware configuration
    RP2040_BUILD=1
    USB_SERIAL_CDC=1
    
    # Performance optimizations
    PICO_USE_STACK_GUARDS=0
    PICO_STACK_SIZE=0x1000
)

# ============================================================================
# PICO SDK LIBRARIES  
# ============================================================================

# Link Pico SDK libraries
target_link_libraries(grblhal_robot_arm
    # Core Pico libraries (without time dependency)
    pico_platform
    pico_runtime
    
    # Hardware interface libraries
    pico_multicore
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
    # hardware_rtc  # Not available for RP2350
    hardware_flash
    hardware_sync
    
    # USB support for communication (without WiFi dependencies)
    pico_unique_id
    pico_stdio
    # pico_stdio_usb  # Disabled due to cyw43 dependency issues
    pico_stdio_uart
    
    # Math libraries for robot kinematics
    pico_float
    pico_double
    hardware_divider
    
    # Memory and storage
    # hardware_rtc  # Not available for RP2350
    hardware_adc
)

# ============================================================================
# BUILD OUTPUT CONFIGURATION
# ============================================================================

# Enable stdio outputs (USB and UART)
pico_enable_stdio_usb(grblhal_robot_arm 1)
pico_enable_stdio_uart(grblhal_robot_arm 1)

# Generate map file for debugging (commented out for pico-sdk 2.2.0 compatibility)
# pico_set_linker_create_map_file(grblhal_robot_arm 1)

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
message(STATUS "grblHAL core files: ${GRBL_SOURCES}")
message(STATUS "Driver files: ${DRIVER_SOURCES}")
message(STATUS "Total source files: ${GRBL_SOURCES} ${DRIVER_SOURCES}")
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