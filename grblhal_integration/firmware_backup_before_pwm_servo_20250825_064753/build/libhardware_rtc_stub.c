/*
 * Stub implementation for hardware_rtc library
 * Created to satisfy linker when hardware_rtc is not available
 */

#include <stdbool.h>

// Empty implementations - RTC functionality disabled for robot arm
void rtc_init(void) { }
bool rtc_running(void) { return false; }
bool rtc_get_datetime(void *t) { return false; }
bool rtc_set_datetime(const void *t) { return false; }
void rtc_set_alarm(const void *t, void *callback) { }
void rtc_enable_alarm(void) { }
void rtc_disable_alarm(void) { }
bool rtc_alarm_pending(void) { return false; }
