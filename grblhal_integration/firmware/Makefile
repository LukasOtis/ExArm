# Minimal grblHAL Makefile for Robot Arm Project
# Target: Host system for testing (can be adapted for Pico later)

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g -DGRBL_PLATFORM=\"RobotArm\" -DOVERRIDE_MY_MACHINE=1
INCLUDES = -I./grbl -I. \
	-I./build/generated/pico_base \
	-I./pico-sdk/src/common/pico_base_headers/include \
	-I./pico-sdk/src/common/pico_time/include \
	-I./pico-sdk/src/rp2_common/hardware_base/include \
	-I./pico-sdk/src/rp2_common/hardware_gpio/include \
	-I./pico-sdk/src/common/pico_stdlib/include \
	-I./pico-sdk/src/common/pico_sync/include \
	-I./pico-sdk/src/rp2350/pico_platform/include \
	-I./pico-sdk/src/rp2_common/pico_platform_compiler/include \
	-I./pico-sdk/src/rp2350/hardware_regs/include \
	-I./pico-sdk/src/rp2_common/pico_platform_sections/include \
	-I./pico-sdk/src/rp2_common/pico_platform_panic/include \
	-I./pico-sdk/src/rp2_common/pico_platform_common/include \
	-I./pico-sdk/src/rp2_common/hardware_sync/include \
	-I./pico-sdk/src/rp2_common/hardware_irq/include \
	-I./pico-sdk/src/rp2_common/hardware_timer/include \
	-I./pico-sdk/src/rp2_common/hardware_clocks/include \
	-I./pico-sdk/src/rp2_common/hardware_resets/include \
	-I./pico-sdk/src/rp2_common/hardware_pll/include \
	-I./pico-sdk/src/rp2_common/hardware_vreg/include \
	-I./pico-sdk/src/rp2_common/hardware_watchdog/include \
	-I./pico-sdk/src/rp2_common/hardware_xosc/include \
	-I./pico-sdk/src/rp2_common/hardware_rtc/include
LIBS = -lm

# Source files - Core grblHAL files
GRBL_DIR = grbl
GRBL_SOURCES = main.c \
               $(GRBL_DIR)/grbllib.c \
               $(GRBL_DIR)/gcode.c \
               $(GRBL_DIR)/motion_control.c \
               $(GRBL_DIR)/planner.c \
               $(GRBL_DIR)/stepper.c \
               $(GRBL_DIR)/protocol.c \
               $(GRBL_DIR)/settings.c \
               $(GRBL_DIR)/system.c \
               $(GRBL_DIR)/report.c \
               $(GRBL_DIR)/nuts_bolts.c \
               $(GRBL_DIR)/machine_limits.c \
               $(GRBL_DIR)/override.c \
               $(GRBL_DIR)/state_machine.c \
               $(GRBL_DIR)/crossbar.c \
               $(GRBL_DIR)/stream.c \
               $(GRBL_DIR)/coolant_control.c \
               $(GRBL_DIR)/spindle_control.c \
               $(GRBL_DIR)/pid.c \
               $(GRBL_DIR)/crc.c \
               $(GRBL_DIR)/nvs_buffer.c \
               $(GRBL_DIR)/ngc_params.c \
               $(GRBL_DIR)/ngc_expr.c \
               $(GRBL_DIR)/ngc_flowctrl.c \
               $(GRBL_DIR)/regex.c \
               $(GRBL_DIR)/strutils.c \
               $(GRBL_DIR)/sleep.c \
               $(GRBL_DIR)/tool_change.c \
               $(GRBL_DIR)/stepper2.c \
               $(GRBL_DIR)/modbus.c \
               $(GRBL_DIR)/modbus_rtu.c \
               $(GRBL_DIR)/stream_file.c \
               $(GRBL_DIR)/stream_passthru.c \
               $(GRBL_DIR)/vfs.c \
               $(GRBL_DIR)/errors.c \
               $(GRBL_DIR)/alarms.c \
               $(GRBL_DIR)/messages.c

# Driver files (Pico-specific, but compile for host testing)
DRIVER_SOURCES = driver.c \
                 serial.c \
                 usb_serial.c \
                 stdio_usb_descriptors.c \
                 i2c.c \
                 spi.c
# Note: ioports.c removed for basic 3-axis control

# All sources
SOURCES = $(GRBL_SOURCES) $(DRIVER_SOURCES)

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Target executable
TARGET = grblhal_robot_arm

# Build rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)
	@echo "Build complete: $(TARGET)"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

# Test the executable
test: $(TARGET)
	@echo "Testing grblHAL Robot Arm binary..."
	@echo "Note: This is a host test build, not Pico firmware"
	@echo "For Pico build, use CMake with Pico SDK"

# Show missing files
check-files:
	@echo "Checking for required grblHAL files..."
	@echo "=== Core grblHAL Files ==="
	@ls -la $(GRBL_DIR)/ 2>/dev/null || echo "grbl/ directory missing"
	@echo "=== Required Files Status ==="
	@for file in grbl.h main.c grbllib.h; do \
		if [ -f "$(GRBL_DIR)/$$file" ]; then \
			echo "✓ $$file present"; \
		else \
			echo "✗ $$file MISSING"; \
		fi; \
	done

# Count source files
count:
	@echo "Source file count:"
	@echo "grblHAL core files: $(words $(GRBL_SOURCES))"
	@echo "Driver files: $(words $(DRIVER_SOURCES))"
	@echo "Total files: $(words $(SOURCES))"

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build the grblHAL robot arm executable"
	@echo "  clean        - Remove build artifacts"
	@echo "  test         - Build and test info"
	@echo "  check-files  - Check which required files are present/missing"
	@echo "  count        - Count source files"
	@echo "  help         - Show this help message"

.PHONY: all clean test check-files count help 