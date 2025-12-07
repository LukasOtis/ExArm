# grblHAL firmware (6-axis RP2350B robot arm)

## Quick build (6-axis RP2350B board)

```bash
# From repo root
cmake -S grblhal_integration/firmware_GRBL -B grblhal_integration/firmware_GRBL/build -G Ninja -DPICO_BOARD=pico2
cmake --build grblhal_integration/firmware_GRBL/build
```

**Note**: The build uses `PICO_BOARD=pico2` which targets the RP2350B microcontroller. The board configuration is selected via `BOARD_ROBOTARM_6AXIS_RP2350B` in `my_machine.h`.

- Output UF2: `grblhal_integration/firmware_GRBL/build/grblHAL.uf2`

### Toolchain/env (if needed)
```bash
export PICO_SDK_PATH="$HOME/.pico-sdk/sdk/2.1.1"   # VSCode Pico ext default
# If Arm GNU Toolchain not on PATH (macOS example):
export PATH="/Applications/ArmGNUToolchain/14.3.rel1/arm-none-eabi/bin:$PATH"
```

### Flashing
```bash
# Hold BOOTSEL while plugging in Pico 2
cp grblhal_integration/firmware_GRBL/build/grblHAL.uf2 /Volumes/RP2350/
```

## Implemented core features

- Axis steps via RP2040/2350 PIO state machine
  - Contiguous STEP pins starting at GPIO 2; directions on GPIO 7..11; hardware enable on GPIO 12. (5-axis Pico 2 board)
  - 6-axis RP2350B board: STEP GPIO 16..21; DIR GPIO 22..27; FAULT GPIO 10..15; enable GPIO 28.
- PWM servo control (M280)
  - `M280 P<id> S<angle>` where angle is 0..180 degrees.
  - 50 Hz servo PWM, 0.544 ms..2.4 ms pulse mapping. Lazy attach on first use.
  - Channels (P): 0..5 mapped to `AUXOUTPUT0..5_PWM_PIN` (6 channels on RP2350B board).
- Analog PWM output (M68 immediate)
  - `M68 E<port> Q<value>` sets duty cycle immediately.
  - Q range: 0..100 (%). Default PWM: 5 kHz.
  - Ports E=0..5 map to analog aux outputs (6 channels on RP2350B board).
- Aux digital outputs (M281)
  - `M281 P<n> S0|1` toggles dedicated digital outputs.
  - P range: 0..6 mapping to GPIO 40/39/38/37/36/35/47 (7 channels on RP2350B board).
  - P6 (GPIO 47) is reserved for LED indicator to show ROS connection status.

## Board pinout (from boards/5axis_robotarm.h)

- Step/Dir/Enable
  - STEP (PIO, contiguous):
    - X: GPIO 2
    - Y: GPIO 3
    - Z: GPIO 4
    - A (M3): GPIO 5
    - B (M4): GPIO 6
  - DIR (GPIO):
    - X: GPIO 7
    - Y: GPIO 8
    - Z: GPIO 9
    - A: GPIO 10
    - B: GPIO 11
  - Enable (all steppers): GPIO 12

- Limits
  - X: GPIO 15
  - Y: GPIO 17
  - Z: GPIO 18
  - A (M3): GPIO 19
  - B (M4): GPIO 20

- Analog/PWM aux outputs (for M280/M68)
  - `AUXOUTPUT0_PWM_PIN` → GPIO 13 (E=0 / P=2)
  - `AUXOUTPUT1_PWM_PIN` → GPIO 14 (E=1 / P=1)
  - `AUXOUTPUT2_PWM_PIN` → GPIO 16 (E=2 / P=0)
  - Note: Servo numbering (M280 P) is assigned in reverse order of definition by the core; effectively:
    - P=0 → GPIO 16, P=1 → GPIO 14, P=2 → GPIO 13

- Digital aux outputs (for M281)
  - `AUXOUTPUT1_PIN` → GPIO 26 (P=0)
  - `AUXOUTPUT2_PIN` → GPIO 27 (P=1)
  - `AUXOUTPUT3_PIN` → GPIO 28 (P=2)

- Aux inputs / misc
  - `AUXINPUT2_PIN` → GPIO 21

- Spindle/Coolant
  - Fully disabled in this build (pins not claimed).

## Board pinout (6-axis RP2350B, from boards/robotarm_6axis_rp2350b.h)

Pin layout organized for optimal PCB routing:

- **UART** (GPIO 0-1)
  - TX: GPIO 0
  - RX: GPIO 1

- **Limits, Aux Input & E-stop** (GPIO 2-9)
  - X limit: GPIO 2
  - Y limit: GPIO 3
  - Z limit: GPIO 4
  - A limit (M3): GPIO 5
  - B limit (M4): GPIO 6
  - C limit (M5): GPIO 7
  - Aux input (`AUXINPUT2_PIN`): GPIO 8 (can be used as PROBE, M66 wait-on-input, etc.)
  - E-stop (`RESET_PIN`): GPIO 9 (active-low, requires `CONTROL_ENABLE & CONTROL_ESTOP`)

- **Axis Fault Inputs** (GPIO 10-15, active-low, pulled up to 3V3)
  - X fault: GPIO 10
  - Y fault: GPIO 11
  - Z fault: GPIO 12
  - A fault (M3): GPIO 13
  - B fault (M4): GPIO 14
  - C fault (M5): GPIO 15

- **STEP pins** (GPIO 16-21, PIO, contiguous for single state machine)
  - X: GPIO 16
  - Y: GPIO 17
  - Z: GPIO 18
  - A (M3): GPIO 19
  - B (M4): GPIO 20
  - C (M5): GPIO 21

- **DIR pins** (GPIO 22-27)
  - X: GPIO 22
  - Y: GPIO 23
  - Z: GPIO 24
  - A (M3): GPIO 25
  - B (M4): GPIO 26
  - C (M5): GPIO 27

- **Enable** (GPIO 28)
  - Steppers enable (all axes): GPIO 28

- **Analog/PWM aux outputs** (GPIO 29-34, for M280/M68, 6 channels)
  - `AUXOUTPUT0_PWM_PIN` → GPIO 29 (PWM 6B) - M280 P0, M68 E0
  - `AUXOUTPUT1_PWM_PIN` → GPIO 30 (PWM 7A) - M280 P1, M68 E1
  - `AUXOUTPUT2_PWM_PIN` → GPIO 31 (PWM 7B) - M280 P2, M68 E2
  - `AUXOUTPUT3_PWM_PIN` → GPIO 32 (PWM 8A) - M280 P3, M68 E3
  - `AUXOUTPUT4_PWM_PIN` → GPIO 33 (PWM 8B) - M280 P4, M68 E4
  - `AUXOUTPUT5_PWM_PIN` → GPIO 34 (PWM 9A) - M280 P5, M68 E5
  - M280: 50 Hz servo PWM, 0.544ms (0°) to 2.4ms (180°) pulse width
  - M68: 5 kHz PWM, 0-100% duty cycle

- **Digital aux outputs** (GPIO 35-40, 47, for M281, 7 channels)
  - Pin order inverted for PCB routing:
  - `AUXOUTPUT1_PIN` → GPIO 40 (P0)
  - `AUXOUTPUT2_PIN` → GPIO 39 (P1)
  - `AUXOUTPUT3_PIN` → GPIO 38 (P2)
  - `AUXOUTPUT4_PIN` → GPIO 37 (P3)
  - `AUXOUTPUT5_PIN` → GPIO 36 (P4)
  - `AUXOUTPUT6_PIN` → GPIO 35 (P5)
  - `AUXOUTPUT7_PIN` → GPIO 47 (P6 - LED indicator for ROS connection status)

## Homing, limits & driver faults (6-axis RP2350B)

- **Homing & limits**:
  - One NC (normally closed) switch per axis, wired to the limit inputs on GPIO 2–7.
  - Switches act as both homing reference and runtime hard limit.
- **Driver faults**:
  - One FAULT input per axis (active-low, pulled up to 3V3):
    - X/Y/Z/A/B/C: GPIO 10/11/12/13/14/15
  - On a new fault edge, the `axis_fault_monitor` plugin raises `Alarm_MotorFault`
    and emits a line like `[AXIS_MOTOR_FAULT:XA]` so the host can see which axes failed.
- **E-stop**:
  - E-stop input on GPIO 9 (`RESET_PIN`) - active-low, normally closed switch.
  - Requires `CONTROL_ENABLE & CONTROL_ESTOP` to be enabled in `my_machine.h`.
  - When triggered, immediately halts all motion and raises an alarm.
- **Hard overtravel / E-stop chain**:
  - For an additional physical "beyond limit" switch, wire it in series with the NC
    limit switch or into a separate E-stop chain that cuts enable/driver power.

## ROS / host integration hints

- Monitor:
  - **Alarms** (e.g. `Alarm_MotorFault`) from the status/error stream.
  - `$pins` / status reports for limit and aux input states.
  - `M66` to wait on specific aux inputs when needed (e.g. `AUXINPUT2_PIN` on GPIO 8).
- Recommended split:
  - Let grblHAL enforce motion safety (limits, faults, E-stop).
  - Use ROS to consume high-level status (`[AXIS_MOTOR_FAULT:..]`, alarm codes, `$pins`)
    and decide how to recover or log.

## Reserved GPIOs (RP2350B, 6-axis board)

- Currently unused and safe for future features (subject to board layout and RP2350B specs):
  - GPIO 41–46.

## Serial & comms
- USB CDC enabled by default. UART available in driver if needed.

## How GRBLHAL handles pin mapping

GRBLHAL uses a **declarative pin mapping system** rather than auto-detecting GPIO capabilities:

1. **Board configuration file** (`boards/robotarm_6axis_rp2350b.h`):
   - Defines which GPIO pins are used for each function (STEP, DIR, LIMIT, AUXOUTPUT, etc.)
   - No need to specify total GPIO count - only the pins you want to use

2. **Driver initialization** (`driver.c`):
   - Builds static arrays (`inputpin[]`, `outputpin[]`) at compile time based on board config
   - Loops through these arrays during initialization to count available pins
   - Assigns port numbers sequentially (0, 1, 2, ...) based on array order

3. **IOPorts system** (`grbl/ioports.c`):
   - Enumerates the pin arrays to determine how many ports are available
   - Creates port mappings: `hal.port.num_digital_out` = count of entries in `outputpin[]` with `PinGroup_AuxOutput`
   - Provides `hal.port.get_pin_info(type, dir, port)` to query pin information by port number

4. **Why adding outputs "just works"**:
   - Define `AUXOUTPUT6_PIN` in board file → added to `outputpin[]` array
   - Driver counts array entries → `aux_outputs.n_pins++`
   - IOPorts enumerates → port 5 becomes available
   - No need to know total GPIO count - GRBLHAL only knows about pins you explicitly define

**Key insight**: GRBLHAL doesn't query the hardware for available GPIOs. It only knows about pins you define in the board config. The number of available ports = number of entries in the pin arrays, not the chip's total GPIO count.

## Notes
- `N_AXIS=5` (X,Y,Z,A,B). Spindle/coolant disabled for the original Pico 2 board.
- `N_AXIS=6` (X,Y,Z,A,B,C) with extended PWM and aux outputs on the RP2350B board.
- M67 (synced analog) is compiled-in by grblHAL but typically use `M68` for immediate.
- Plugins enabled: PWM servo (M280, 6 channels), digital aux out (M281, 6 channels) and axis fault monitor.
