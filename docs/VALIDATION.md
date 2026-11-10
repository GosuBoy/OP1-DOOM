# Validation — 2026-11-10

- Private research main was checked remotely and matches local commit 179d9823be6dadc796574099aad6c8789f347e23. Its working tree was left unchanged.
- Published source files are byte-preserved from the private V74 candidate, with hashes recorded in provenance/imports.json. This repository starts a new history without private research commits or machine-specific build scripts.
- The historical V74 .op1 in the owner's Downloads matches SHA256 d242c706a2813de3fbb5a26fe1ed6bcf546bf1eba903f9f759f39510597cc941. Its CRC, single LZMA stream and 116-member container were checked in memory. The main loader hash also matches the recorded historical identity. No files were extracted or installed; the artifact is not distributed here.
- Private preparation checks passed for all eleven input mappings, edge/batch/reset behavior and invalid-state refusal. Those input files are not part of this initial public subset.
- The retained timing test passes without sanitizers and with UBSan. AddressSanitizer stalls during host initialization: a minimal program also times out without reaching its main marker, and disabling leak detection does not resolve it. This also occurs outside the sandbox. The precise host/runtime cause is unresolved; no ASan pass is claimed.
- The public source audit and pure frame-queue UBSan regression are reproducible with the commands in README.md. These do not model interrupts, DMA or hardware timing.

No full firmware build, new simulator replay, new device test, software installation or firmware installation occurred. Known hardware risks and combined-executable licensing questions remain open.
