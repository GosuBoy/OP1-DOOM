# OP1-DOOM

An experimental Doom port for the **original Teenage Engineering OP-1**.

This first public release is an **audited source research snapshot**, based on the V74 wide-HUD experiment. The original owner tested V74 as playable on firmware 246 / TE-BOOT 2.30. **Hardware audio does not work. OP-1 Field is unsupported.**

## Release scope

The repository contains the Doom engine changes and the native runtime files whose GPL notices or upstream license provenance have been verified. It does **not** contain a complete firmware build kit, a flashable download, or installation instructions. Some port-specific files are withheld until their license declarations are resolved; bootstrap and firmware-container tooling are also withheld. This is a partial source release, not a claim that all V74 build inputs are publicly available.

No firmware, game WAD, OTP/keys, toolchain, screenshot or game artwork is distributed. The private research repository remains separate.

## Source layout

- `src/engine`: V74 engine working-source snapshot, derived from DoomGeneric. Unused desktop backends and their old Makefile are omitted.
- `src/native`: retained GPL-noticed native runtime, display and simulator components. These are research components; several refer to unpublished dependencies.
- `provenance/imports.json`: exact source identities, hashes and withheld-file inventory.
- [Build status](docs/BUILD.md), [licensing](docs/LICENSING.md), [controls](docs/CONTROLS.md), [hardware risks](docs/HARDWARE.md), and [validation](docs/VALIDATION.md).

In the historical build, native g_game.c, m_menu.c, wi_stuff.c, i_system.c, i_input.c and i_video.c override the engine/platform counterparts. The inherited renderer recipe also combines a renamed reference r_draw object with the optimized column_cached implementation, letting the final link discard the old column function. This layout preserves the distinct source files; it does not supply an automatic link recipe.

## Check the source

With Python 3.9+ and an already installed C compiler:

```sh
python3 tools/check_sources.py
python3 tools/check_frame_queue.py
```

These run offline and do not install dependencies or access hardware. The host regression checks queue ownership/refusal behavior with UBSan. A private native rebuild now matches V74's loaded bytes exactly; see [recovery findings](docs/RECOVERY-PROGRESS.md). The public snapshot remains incomplete, and no full-Doom simulator run, firmware-package build or hardware test is claimed.

## Hardware warning

**Normal USB, charging and power behavior remain unresolved.** The historical owner procedure required brief battery-only gameplay and restoring the known-good baseline before charging or normal USB use. Recovery has worked on the owner's unit but is not guaranteed. Never interrupt an active update. This source release does not authorize or recommend installing a newly built image; read [the hardware limitations](docs/HARDWARE.md).

## License

Published engine and runtime code is GPL licensed, commonly **GPL-2.0-or-later** as specified by individual notices. Original copyright and license notices are retained; see [LICENSE](LICENSE) and [provenance details](docs/LICENSING.md). No rights to vendor firmware or Doom game data are granted.

This is an independent project with no claimed endorsement by Teenage Engineering or id Software.
