/*
  platform.h - Platform-specific definitions for grblHAL on Raspberry Pi Pico
  
  This file provides platform-specific macros and definitions needed by grblHAL
  when building for the RP2040/Pico platform.
*/

#ifndef PLATFORM_H
#define PLATFORM_H

#include "pico.h"

// ============================================================================
// PLATFORM IDENTIFICATION
// ============================================================================

#define PLATFORM_PICO 1
#define PLATFORM_RP2040 1

// ============================================================================
// MEMORY AND STORAGE
// ============================================================================

// Flash memory configuration
#define FLASH_SIZE (2 * 1024 * 1024)  // 2MB for Pico
#define FLASH_PAGE_SIZE 256
#define FLASH_SECTOR_SIZE (4 * 1024)  // 4KB sectors

// RAM configuration
#define RAM_SIZE (264 * 1024)  // 264KB for Pico
#define STACK_SIZE (8 * 1024)  // 8KB stack

// ============================================================================
// TIMING AND CLOCKS
// ============================================================================

// System clock frequency
#define SYSTEM_CLOCK_FREQ 125000000  // 125MHz default

// Timer configuration
#define TIMER_FREQ 1000000  // 1MHz timer
#define TIMER_RESOLUTION 1  // 1us resolution

// ============================================================================
// GPIO AND PERIPHERALS
// ============================================================================

// GPIO configuration
#define GPIO_COUNT 30
#define GPIO_MAX_PIN 29

// UART configuration
#define UART_COUNT 2
#define UART_BAUD_DEFAULT 115200

// I2C configuration
#define I2C_COUNT 2
#define I2C_FREQ_DEFAULT 100000

// SPI configuration
#define SPI_COUNT 2
#define SPI_FREQ_DEFAULT 1000000

// PWM configuration
#define PWM_COUNT 8
#define PWM_FREQ_DEFAULT 50000

// ============================================================================
// INTERRUPT PRIORITIES
// ============================================================================

#define IRQ_PRIORITY_HIGHEST 0
#define IRQ_PRIORITY_HIGH 1
#define IRQ_PRIORITY_NORMAL 2
#define IRQ_PRIORITY_LOW 3
#define IRQ_PRIORITY_LOWEST 4

// ============================================================================
// DEBUG AND DEVELOPMENT
// ============================================================================

#ifdef DEBUG
#define PLATFORM_DEBUG_PRINT(x) printf x
#else
#define PLATFORM_DEBUG_PRINT(x)
#endif

// ============================================================================
// FORMAT MACROS
// ============================================================================

// Integer format macros for printf
#define UINT32FMT "%lu"
#define INT32FMT "%ld"
#define UINT16FMT "%u"
#define INT16FMT "%d"
#define UINT8FMT "%u"
#define INT8FMT "%d"

// ============================================================================
// COMPILER-SPECIFIC
// ============================================================================

// Force functions to stay in RAM (not flash)
#define RAM_FUNC __not_in_flash_func

// Interrupt service routine attribute
#define ISR_ATTR __attribute__((interrupt))

// ============================================================================
// PLATFORM-SPECIFIC MACROS
// ============================================================================

// Sleep functions
#define platform_sleep_ms(ms) sleep_ms(ms)
#define platform_sleep_us(us) sleep_us(us)

// Time functions
#define platform_get_time() time_us_32()

// Reset functions
#define platform_reset() reset()

#endif // PLATFORM_H
