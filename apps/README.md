# Applications

Zephyr firmware targets for the [Hextree Glitch Tag](https://1bitsquared.com/products/glitch-tag). Each app exposes a `glitch_target()` function (often in RAM via `__ramfunc`), pulses a GPIO trigger for the Faultier glitcher, and reports status over UART.

Build any app from the repo root:

```bash
uv run west build -b glitchtag -p always apps/<name>
uv run west flash
```

Many apps include a Jupyter notebook under `notebooks/` for automated glitch sweeps with the `faultier` Python library.

## Table of contents

- [Intro targets](#intro-targets)
- [Instruction characterization](#instruction-characterization)
- [Countermeasure testing](#countermeasure-testing)
- [Shared conventions](#shared-conventions)

## Intro targets

Practical glitching exercises with a clear success condition. Start here.

| App | Description |
|-----|-------------|
| [`simple_glitch`](simple_glitch/) | Nested loop counter check. Glitch past the validation to print a flag. Includes a watchdog. |
| [`password_glitch`](password_glitch/) | Fake wallet that checks a password over UART. Glitch past `strcmp` to reveal an obfuscated seed. |
| [`firmware_print_glitch`](firmware_print_glitch/) | Prints a bootloader string from a custom linker section. Useful for studying glitches on boot/print paths. |

## Instruction characterization

Calibration firmware with inline ARM assembly and NOP padding around specific instructions. UART markers (`RST`, `OKAY`, `BRAN`, etc.) and register dumps help map when and how a glitch affects execution. Work through these in order — from simple data movement to control flow and memory loads.

| App | Description |
|-----|-------------|
| [`mov_characterization_basic`](mov_characterization_basic/) | Two `MOV` instructions with NOP windows between them. |
| [`mov_characterization_all_registers`](mov_characterization_all_registers/) | Same MOV sequence, then `STM` to dump r0–r12 and LR. |
| [`int_characterization_basic`](int_characterization_basic/) | C-level assignment (`foo = 0x55555555`); reports `foo` and r7. |
| [`conditional_characterization`](conditional_characterization/) | `CMP` / `BEQ` branch — success path prints `SUCC` instead of `OKAY`. |
| [`bx_characterization`](bx_characterization/) | `BX` unconditional branch to a target function. |
| [`blx_characterization`](blx_characterization/) | `BLX` branch-with-link; extra UART padding for the glitcher. |
| [`ldm_characterization`](ldm_characterization/) | `LDM` load from memory, then dumps all registers over UART. |
| [`ldm_exploit_characterization`](ldm_exploit_characterization/) | Buffer of function pointers copied via `uint32_copy` — models PC hijacking. |

## Countermeasure testing

Defensive patterns inspired by secure-hardware practice. Test whether glitching can bypass each countermeasure.

| App | Description |
|-----|-------------|
| [`countermeasure_delays`](countermeasure_delays/) | Random NOP delay before a value check. Glitch success prints `SUCC`. |
| [`countermeasure_double_read`](countermeasure_double_read/) | Triple-read consistency check on memory; mismatch triggers a fatal crash. |
| [`countermeasure_secbool`](countermeasure_secbool/) | Trezor-style `secbool` / `ensure()` macro. Success bypasses a check that should always fail. |

## Shared conventions

- **Board**: all apps target `glitchtag` and include a `testcase.yaml` for compile-only Twister verification (`uv run west twister -T apps -p glitchtag`).
- **Shared code**: UART, GPIO, and helpers live in [`common/`](../common/).
- **Optimization**: glitch targets are compiled with `-O0` so instruction layout is predictable.
