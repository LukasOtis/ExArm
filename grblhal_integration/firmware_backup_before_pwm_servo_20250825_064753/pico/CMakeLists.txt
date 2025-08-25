cmake_minimum_required(VERSION 3.13)

# Include Pico SDK
include(pico_sdk_import.cmake)

# Project definition
project(grbl_pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize Pico SDK
pico_sdk_init()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../grbl
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# GRBL source files
set(GRBL_SOURCES
    ../grbl/main.c
)

# Pico-specific source files
set(PICO_SOURCES
    pico_main.c
    pico_hal.c
)

# Create executable
add_executable(grbl_pico
    ${GRBL_SOURCES}
    ${PICO_SOURCES}
)

# Link libraries
target_link_libraries(grbl_pico
    pico_stdlib
    pico_unique_id
    hardware_gpio
    hardware_timer
    hardware_pwm
    hardware_uart
    hardware_flash
)

# Enable USB and UART output
pico_enable_stdio_usb(grbl_pico 1)
pico_enable_stdio_uart(grbl_pico 1)

# Create additional output files
pico_add_extra_outputs(grbl_pico)

# Compiler flags
target_compile_options(grbl_pico PRIVATE
    -Wall
    -Wextra
    -O2
    -g
)

# Define macros
target_compile_definitions(grbl_pico PRIVATE
    PICO_PLATFORM=1
    GRBL_PICO=1
    N_AXIS=6
)
