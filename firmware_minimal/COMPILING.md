## Compile the firmware (Pico2, 5-axis)

### Prerequisites

- Arm GNU Toolchain installed at `/Applications/ArmGNUToolchain/14.3.rel1` (as used here)
- Pico SDK installed (the VSCode Pico extension sets it under `~/.pico-sdk`)
- CMake and Ninja

### One-time environment setup (new shell)

Export the Arm toolchain so CMake uses the right compiler and specs files.

```bash
export PICO_TOOLCHAIN_PATH=/Applications/ArmGNUToolchain/14.3.rel1
export PATH="/Applications/ArmGNUToolchain/14.3.rel1/arm-none-eabi/bin:$PATH"
```

Optional: ensure Pico SDK is discoverable (usually handled by the VSCode Pico extension):

```bash
# Only if you keep a custom SDK path; otherwise skip
export PICO_SDK_PATH="$HOME/.pico-sdk/sdk/2.1.1"
```

### Configure and build

Run these from the repository root (`ExArm`). This targets Pico2 and uses Ninja.

```bash
rm -rf firmware_minimal/build
cmake -S firmware_minimal -B firmware_minimal/build -G Ninja -DPICO_BOARD=pico2
cmake --build firmware_minimal/build
```

### Output

- Flashable UF2: `firmware_minimal/build/grblHAL.uf2`

### Verifying 5-axis

This project is configured for 5 axes:
- `BOARD_GENERIC_5AXIS` and `N_AXIS=5` are applied so the core and driver agree on axis count.
- `$pins` should show steps on `PIO2/3/4/5/6` and directions on pins `7/8/9/10/11` (X/Y/Z/A/B).

### Common pitfalls and fixes

- nosys.specs not found during link
  - Cause: Wrong compiler picked up.
  - Fix: Re-run the environment setup above to put `/Applications/ArmGNUToolchain/14.3.rel1/arm-none-eabi/bin` first in `PATH`.

- Generator mismatch ("Ninja" vs "Unix Makefiles")
  - Fix: Delete the `firmware_minimal/build` directory before reconfiguring.

- Board/platform mismatch (pico vs pico2)
  - Fix: Configure with `-DPICO_BOARD=pico2` and regenerate the build directory.

### Rebuild quickly after changes

```bash
cmake --build firmware_minimal/build
```

If you changed generator, toolchain, or board: clean the build dir and reconfigure first.


