/*
  axis_fault_monitor.c - per-axis driver fault monitoring for RP2350B robot arm

  Monitors dedicated FAULT inputs for each axis and raises Alarm_MotorFault
  when any axis reports a fault. Also emits a short status message indicating
  which axes are affected so the host (e.g. ROS) can react.

  Fault inputs are defined in the board map, e.g. robotarm_6axis_rp2350b.h:
    X_FAULT_PIN, Y_FAULT_PIN, Z_FAULT_PIN, M3_FAULT_PIN, M4_FAULT_PIN, M5_FAULT_PIN

  Fault inputs are assumed active-low with a pull-up to 3V3 and an open-drain
  / open-collector output from the driver.
*/

#include "driver.h"

#if defined(AXIS_FAULT_MONITOR_ENABLE) && AXIS_FAULT_MONITOR_ENABLE

#include <string.h>

#include "grbl/system.h"
#include "grbl/protocol.h"
#include "grbl/report.h"
#include "hardware/gpio.h"

// Map logical axes 0..N_AXIS-1 to fault GPIOs from the board header.
static const uint8_t fault_pins[N_AXIS] = {
    X_FAULT_PIN,
    Y_FAULT_PIN,
    Z_FAULT_PIN,
#if N_AXIS > 3
    M3_FAULT_PIN,
#endif
#if N_AXIS > 4
    M4_FAULT_PIN,
#endif
#if N_AXIS > 5
    M5_FAULT_PIN,
#endif
};

static bool last_fault[N_AXIS] = {0};
static on_report_options_ptr on_report_options;

static void axis_fault_poll (void *data)
{
    (void)data;

    bool new_fault = false;
    char msg[40] = "[AXIS_MOTOR_FAULT:";
    size_t len = strlen(msg);
    static const char axis_labels[] = { 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E' };

    for(uint_fast8_t i = 0; i < N_AXIS && i < sizeof(axis_labels); i++) {

        bool fault = !DIGITAL_IN(fault_pins[i]); // active-low

        if(fault && !last_fault[i]) {
            // Rising edge: record this axis in the message
            if(len < sizeof(msg) - 2) {
                msg[len++] = axis_labels[i];
            }
            new_fault = true;
        }

        last_fault[i] = fault;
    }

    if(new_fault) {
        msg[len++] = ']';
        msg[len++] = '\n';
        msg[len] = '\0';

        // Send feedback message FIRST so client sees the description before the alarm code
        // This will display "[MSG:Motor fault - clear, then reset to continue]"
        grbl.report.feedback_message(Message_MotorFault);
        // Small delay to ensure message is sent before alarm (similar to report_alarm_message)
        hal.delay_ms(50, NULL);
        
        // Raise a motor fault alarm - this will send "ALARM:17"
        system_raise_alarm(Alarm_MotorFault);
        
        // Also send our custom axis-specific message
        hal.stream.write(msg);
    }
}

static void onReportOptions (bool newopt)
{
    if(on_report_options)
        on_report_options(newopt);

    if(!newopt)
        report_plugin("Axis fault monitor", "0.01");
}

void axis_fault_monitor_init (void)
{
    // Configure fault pins as inputs with pull-ups (active-low, so pull-up = no fault)
    for(uint_fast8_t i = 0; i < N_AXIS; i++) {
        gpio_init(fault_pins[i]);
        gpio_set_dir(fault_pins[i], GPIO_IN);
        gpio_pull_up(fault_pins[i]);  // Pull-up to 3.3V = HIGH = no fault
    }

    // Initialize last_fault state by reading current pin states
    for(uint_fast8_t i = 0; i < N_AXIS; i++) {
        last_fault[i] = !DIGITAL_IN(fault_pins[i]); // active-low
    }

    // Hook into report system to show plugin in $I output
    on_report_options = grbl.on_report_options;
    grbl.on_report_options = onReportOptions;

    // Start polling on SysTick so we see faults quickly without adding a new timer.
    task_add_systick(axis_fault_poll, NULL);
}

#endif


