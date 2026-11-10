# Build status

There is no supported firmware build command in this release. It is a partial source research snapshot. Do not assume that compiling the included C files produces the owner-tested V74 firmware.

The private historical recipe links previously built engine objects, WAD data, runtime overrides and stock-firmware imports. V74 uses 440x200 engine defines and a wide status bar; the earlier 320x200 reference build is not a substitute. Native runtime engine overrides are identified in README.md.

The historical bootstrap depends on V72/V73 development checkpoints, pinned Blackfin tools and a modified V49 firmware parent. That parent is not a pristine stock firmware file. No guessed patcher or automatic firmware downloader is provided here. Some bootstrap/container routines also contain vendor-origin patch context, so they are withheld pending provenance review.

## Remaining engineering work

1. Resolve licenses for the withheld port-specific source files listed in provenance/imports.json, and reconcile the bootstrap/container material before considering publication of those components.
2. Recover the pinned V72/V73 and toolchain archives privately. The Library archives have been located; this task's transfer helper reports that file materialization is unavailable. No compiler was installed or target build run.
3. Recover the exact compiler configuration and older objects, then reconstruct all engine/runtime overrides. Recorded Linux recovery commands for GCC 13.2.0, binutils 2.44 and GDB 13.2 have now been located. The official binutils archive checksum is verified, and a local Mac assembler/linker reproduces the V74 bootstrap exactly; see [follow-up findings](RECOVERY-PROGRESS.md). The full C compiler/simulator and full V74 build remain separate work.
4. Derive a reviewed recipe from exact user-supplied stock firmware and lawful WAD data. Keep inputs out of Git and reject unknown versions. Providing a local builder does not automatically resolve licensing of a combined executable.
5. Verify native loadable bytes, relocations, bootstrap and final loader/container against preserved evidence. Metadata/path differences require explicit treatment.
6. Review USB/power and recovery behavior, obtain separate installation authorization, and test the exact candidate before presenting a public installer.

The existing V74 artifact was independently hash-checked during this preparation and matched its historical record. That verifies the old package's identity; it is not a rebuild or new hardware result. No binary from that package is included.
