# Source provenance and licensing

The engine's original preserved upstream checkout pins [DoomGeneric revision dcb7a8d](https://github.com/ozkl/doomgeneric/tree/dcb7a8dbc7a16ce3dda29382ac9aae9d77d21284), dated 2026-04-12. That revision is also present in the independently fetched public upstream history. The GPL text in this repository matches its LICENSE byte-for-byte.

The private staging comparison examined 384 engine-file copies: 362 match that upstream revision exactly; 22 copies differ across 18 filenames. Those differences include two historical engine snapshots; this public collection imports the V74 candidate snapshot and selected native overrides only. Its exact per-file identities are recorded in provenance/imports.json. The immediate research source commit is 179d9823be6dadc796574099aad6c8789f347e23.

Files retain their id Software, Simon Howard and other original notices. Headerless upstream files are traced to the pinned GPL repository; native runtime files retain their notices or are covered for their project-original portions by the owner-approved [GPL-2.0-or-later scope](../LICENSE-SCOPE.md). The 20 newly included files retain exact source bytes and import hashes. No third-party rights are replaced. The withheld-file inventory describes the remaining exclusions. Existing file-specific notices control; the GPL text is not a license for unrelated vendor assets.

## Deliberate exclusions

- Proprietary firmware, OTP, keys, WADs, compiled target outputs and toolchains.
- Bootstrap and firmware-container recipes with unresolved provenance/combined-output questions.
- Embedded icon.c artwork. Its bytes match upstream, which establishes its origin but does not independently establish artwork redistribution rights.
- Later audio/emulator investigations and private delivery metadata.

Some published runtime routines call stock services at fixed addresses. These are source-level bridges, not a distribution of the firmware implementing those services. No legal clearance is claimed for a linked or packaged executable. Before distributing such an output, resolve the applicable GPL corresponding-source obligations and vendor/game-data rights. A local firmware/WAD builder is not automatic clearance.

Manufacturer reference: [Teenage Engineering IP policy](https://teenage.engineering/ip-rights-te-products). Its creative-material permission does not extend to code. The project does not rely on that policy as permission to distribute vendor firmware.
