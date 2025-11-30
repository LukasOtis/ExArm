### For Host Build (Testing)

## Building the Firmware (Pico 2 / RP2350)

See the authoritative guide: `grblhal_integration/firmware_GRBL/README.md`.

Quick commands from repo root:
```bash
cmake -S grblhal_integration/firmware_GRBL -B grblhal_integration/firmware_GRBL/build -G Ninja -DPICO_BOARD=pico2
cmake --build grblhal_integration/firmware_GRBL/build
```

Flashing:
```bash
# Hold BOOTSEL while plugging in Pico 2
cp grblhal_integration/firmware_GRBL/build/grblHAL.uf2 /Volumes/RP2350/
```

If needed, set environment:
```bash
export PICO_SDK_PATH="$HOME/.pico-sdk/sdk/2.1.1"
export PATH="/Applications/ArmGNUToolchain/14.3.rel1/arm-none-eabi/bin:$PATH"
```
