# grblHAL firmware (Pico 2, 5-axis)

## Quick build (RP2350 Pico 2)

```bash
# From repo root
cmake -S grblhal_integration/firmware_GRBL -B grblhal_integration/firmware_GRBL/build -G Ninja -DPICO_BOARD=pico2
cmake --build grblhal_integration/firmware_GRBL/build
```

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
  - Contiguous STEP pins starting at GPIO 2; directions on GPIO 7..11; hardware enable on GPIO 12.
- PWM servo control (M280)
  - `M280 P<id> S<angle>` where angle is 0..180 degrees.
  - 50 Hz servo PWM, 0.544 ms..2.4 ms pulse mapping. Lazy attach on first use.
  - Channels (P): 0..2 mapped to `AUXOUTPUT0/1/2` PWM pins (see Pinout).
- Analog PWM output (M68 immediate)
  - `M68 E<port> Q<value>` sets duty cycle immediately.
  - Q range: 0..100 (%). Default PWM: 5 kHz.
  - Ports E=0..2 map to analog aux outputs on GPIO 13/14/16.
- Aux digital outputs (M281)
  - `M281 P<n> S0|1` toggles dedicated digital outputs.
  - P range: 0..2 mapping to GPIO 26/27/28.

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

## Serial & comms
- USB CDC enabled by default. UART available in driver if needed.

## Notes
- `N_AXIS=5` (X,Y,Z,A,B). Spindle/coolant disabled.
- M67 (synced analog) is compiled-in by grblHAL but typically use `M68` for immediate.
- Plugins enabled: PWM servo (M280) and digital aux out (M281).
