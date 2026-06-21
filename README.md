# Hextree Glitch Tag

Software for the [Hextree](https://www.hextree.io/) [Glitch Tag](https://1bitsquared.com/products/glitch-tag).

## Setup

### Prerequisites

- [uv](https://docs.astral.sh/uv/) (Python package manager)
- [Zephyr SDK](https://github.com/zephyrproject-rtos/sdk-ng) installed (e.g. `/opt/zephyr-sdk-1.0.1`), minimal bundle recommended
    - `arm-zephyr-eabi` toolchain installed (via `setup.sh -t arm-zephyr-eabi -h -c`)

### One-time workspace setup

```bash
cd faultier_glitchtag_projects

# Python deps (west + full Zephyr requirements)
uv sync

# In-place T2 west workspace: manifest repo is also the workspace root.
uv run west update --narrow --fetch-opt=--depth=1
uv run west zephyr-export
```

## Building

From the repo root:

```bash
uv run west build -b glitchtag -p always apps/simple_glitch
```

Replace `apps/simple_glitch` with any app under `apps/`.

After a build, `compile_commands.json` is symlinked to the repository root for IDE use (clangd, etc.).

## Twister

Each app includes a `testcase.yaml` for compile-only verification on the GlitchTag board. From the repo root:

```bash
uv run west twister -T apps -p glitchtag
```

## Flashing

```bash
uv run west flash
```

Flash runners (JLink, pyOCD, nrfjprog) are configured in `boards/hextree/glitchtag/board.cmake`.

## Layout

```
faultier_glitchtag_projects/
├── pyproject.toml  # uv project + Zephyr Python deps
├── uv.lock
├── west.yml
├── zephyr/module.yml
├── apps/           # application firmware
├── boards/         # glitchtag out-of-tree board
├── common/         # shared app code
└── deps/           # west-managed Zephyr + modules (gitignored)
```

To refresh Python dependencies after a Zephyr upgrade:

```bash
uv add --requirement deps/zephyr/scripts/requirements.txt --group zephyr
```
