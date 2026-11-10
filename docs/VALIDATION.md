# Validation — 2026-11-10

For subsequent assembler/linker, bootstrap and source-license findings, see [recovery progress](RECOVERY-PROGRESS.md). The initial release checks below remain scoped as described.

- Private research main was checked remotely and matches local commit 179d9823be6dadc796574099aad6c8789f347e23. Its working tree was left unchanged.
- Published source files are byte-preserved from the private V74 candidate, with hashes recorded in provenance/imports.json. This repository starts a new history without private research commits or machine-specific build scripts.
- The historical V74 .op1 in the owner's Downloads matches SHA256 d242c706a2813de3fbb5a26fe1ed6bcf546bf1eba903f9f759f39510597cc941. Its CRC, single LZMA stream and 116-member container were checked in memory. The main loader hash also matches the recorded historical identity. No files were extracted or installed; the artifact is not distributed here.
- Private preparation checks passed for all eleven input mappings, edge/batch/reset behavior and invalid-state refusal. Those input files are now included under the approved runtime license scope; the historical checks did not access hardware.
- The retained timing test passes without sanitizers and with UBSan. AddressSanitizer stalls during host initialization: a minimal program also times out without reaching its main marker, and disabling leak detection does not resolve it. This also occurs outside the sandbox. The precise host/runtime cause is unresolved; no ASan pass is claimed.
- The public source audit, pure frame-queue UBSan regression and retained timing UBSan regression are reproducible with the commands in README.md. These do not model interrupts, DMA or hardware timing.

The initial release performed no full firmware build, simulator replay, new device test, software installation or firmware installation. Subsequent bounded simulator checks are documented in recovery progress; no device or firmware installation occurred. Known hardware risks and combined-executable licensing questions remain open.
