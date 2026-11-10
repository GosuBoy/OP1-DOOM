# Build status

There is no supported firmware build command in this release. It is a partial source research snapshot. Do not assume that compiling the included C files produces the owner-tested V74 firmware.

The private historical recipe links previously built engine objects, WAD data, runtime overrides and stock-firmware imports. V74 uses 440x200 engine defines and a wide status bar; the earlier 320x200 reference build is not a substitute. Native runtime engine overrides are identified in README.md.

The historical bootstrap depends on V72/V73 development checkpoints, pinned Blackfin tools and a modified V49 firmware parent. That parent is not a pristine stock firmware file. No guessed patcher or automatic firmware downloader is provided here. Some bootstrap/container routines also contain vendor-origin patch context, so they are withheld pending provenance review.

## Remaining engineering work

1. Resolve licenses for the withheld port-specific source files listed in provenance/imports.json, and reconcile the bootstrap/container material before considering publication of those components.
2. Recover the pinned V72/V73 and toolchain archives privately. The Library archives have been located; this task's transfer helper reports that file materialization is unavailable. No compiler was installed or target build run.
3. The native-code recipe is now reconstructed and verified privately: the required GCC 13.2.0 C driver/backend and binutils 2.44 tools build locally, the native load regions match V74 exactly, and the bootstrap also reproduces. See [follow-up findings](RECOVERY-PROGRESS.md). The public snapshot still withholds files pending license decisions. The simulator builds but fails a bounded CALL fixture and remains unqualified; full firmware packaging is separate work.
4. Derive a reviewed recipe from exact user-supplied stock firmware and lawful WAD data. Keep inputs out of Git and reject unknown versions. Providing a local builder does not automatically resolve licensing of a combined executable.
5. Preserve the verified native-load and bootstrap identities, then separately verify the final loader/container derivation, relocation contracts and memory checks for any new package. Loaded-byte identity has been established; whole historical ELF identity and a newly built update package have not.
6. Review USB/power and recovery behavior, obtain separate installation authorization, and test the exact candidate before presenting a public installer.

The existing V74 artifact was independently hash-checked and matched its historical record. Its identity check is separate from the subsequent native-load reconstruction described above. No new firmware update package was built, no new hardware result is claimed, and no binary from that package is included.
