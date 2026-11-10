# Follow-up recovery findings

## One more source file cleared

`src/native/column_cached.c` retains an explicit **GPL version 2 or later** notice with id Software and Simon Howard attribution. The first export's narrow notice filter missed that wording. It is now included byte-for-byte, and its historical license-file link is supplied under THIRD_PARTY_LICENSES. A subsequent reconstruction verified that the inherited V68 composite-object recipe selects this implementation for V74, despite the retained source comment's earlier experimental status.

Current main therefore has 206 imported source files. The original v0.1.0-source tag remains unchanged at its 205-file snapshot. Twenty port-specific runtime files still lack a resolved license declaration and remain withheld.

## Binutils and bootstrap reproduction

The official binutils 2.44 source archive has SHA256:

`ce2017e059d63e67ddb9240e9d4ec49c2893605035cd60e92ad53177f4377237`

That matches the [publisher's release checksum](https://sourceware.org/pipermail/binutils/2025-February/139195.html). The older receipt's mismatch against an earlier saved pin is not evidence that this official archive is corrupt. The recovery commands were located in the private project's recorded Linux rebuild guide; the original pre-recovery configure commands remain unknown.

An approved local build of binutils 2.44 for bfin-elf completed on the Mac without `make install` or new system dependencies. The initial build hit a bundled-zlib/macOS-SDK compile conflict; rebuilding with `--with-system-zlib` used the existing system library and passed.

Using that assembler, linker and objcopy on the retained private V74 bootstrap source produced exactly 628 bytes with SHA256:

`6afffcbd48d3293842e4d2b6afc1b7de72d7ef8b750aaa0388a8bcf0c216766f`

This matches the historical reviewed V74 bootstrap. The inspection ELF retains a linker warning about an RWX load segment. No bootstrap binary, tool executable, proprietary input or unresolved bootstrap source is distributed here. This checks one component's byte identity; it does not establish source redistribution rights, full firmware reproduction or device safety.

## Private inputs and source lineage

The exact historical WAD was verified in an existing owner-supplied local attachment and independently recovered from the owner's pinned V74 package; both match the historical hash. It remains private. V72/V73 Library archive transfers still report that the approved materialization capability is unavailable.

Earlier Git history establishes matches for the V69 control codec, input-state proposal and bindings, and V65 origins for many other withheld runtime files. The remaining session-limit and diagnostic-screen differences are small, identifiable project changes. Origin and authorship metadata do not independently supply license terms; the rights holder's decision is still needed.

The retained bootstrap assembly traces to the earlier private factory/Run source design and contains source-level calls to retained stock services. Reproduction is now verified; license and vendor-derived-material questions remain separate. Container/firmware code and images remain excluded.

## Native-code reconstruction now matches V74

GCC 13.2.0 and its source prerequisites were recovered from official GNU sources. The compiler archive matches its historical SHA256; GMP 6.2.1, MPFR 4.1.0 and MPC 1.2.1 were verified against GCC's publisher-supplied prerequisite checksums. GMP is an explicitly pinned prerequisite for this Mac build, not a claim about the historical Linux host library version.

The full all-gcc build hit an auxiliary gcov/libc++ macro conflict on this Mac SDK. Required `cc1`, `xgcc` and internal-header targets built successfully from unpatched source, using system zlib and the locally built Blackfin binutils. No `make install` or system dependency installation was performed. This verifies the C compiler needed here, not every tool in the GCC distribution.

Three private reconstruction attempts were retained:

| Attempt | Result against original V74 native load |
| --- | --- |
| Direct source rebuild | Compiles/links; code/data region differs and is 128 bytes longer |
| Recorded source-path remapping | Correct region size; 2,847 differing text bytes, concentrated in renderer functions |
| Restored reviewed V68 composite-renderer recipe | **Both complete load regions match byte-for-byte** |

The missing object transformation was recovered from the earlier reviewed V68 integration script: rename the reference `_R_DrawColumn` symbol and function section, compile column_cached.c with its recorded O2/long-call flags, combine the objects with a relocatable link, and let the final garbage-collecting link discard the unused reference column. This was a recovered recipe step, not a guessed source patch.

An independent comparison also concatenated the original loader's 21 new COPY records into its two exact intervals and compared them against the rebuilt ELF's loadable segments, including zero fill:

- WAD region at `0x00a00000`: 4,196,020 bytes, exact match; game data stays private.
- Native code/data region at `0x00eff000`: 954,480 bytes, exact match. SHA256 `314c9175fb9b8d4d30d7209c2571b72f93d8d0282bbc97a2be4a7399416fe306`.

The [comparison receipt](../provenance/native-load-reproduction.json) records the exact identities. Whole-ELF identity with the historical ELF is not claimed; source-path/symbol metadata is separate from loaded bytes.

This closes native-load and bootstrap reproduction within the private source/input scope. V72/V73 archives are still unavailable as historical evidence, but were not needed for this independent native-load rebuild. Public source completeness still requires the withheld-file license decision. A stock-firmware-to-update builder, container provenance/rights review, simulator qualification and physical safety remain separate work. No new .op1 package was built and the rebuilt native image was not executed.

## Simulator build is not a validation pass

The historical GDB 13.2 source archive was recovered and SHA256 verified. Its Blackfin simulator built locally from unpatched source using existing system zlib, without installation.

A minimal assembler fixture containing OUTC and HLT prints the expected byte and exits successfully. A separate fixture containing a CALL prints its initial marker but does not reach the callee/return markers before the cycle watchdog stops it. The retained pure target timing regression also fails to produce its success marker under the bounded configurations tried; the default run times out. The configuration/host-simulator cause remains unresolved. The build is therefore **not accepted for target validation**, and no full Doom, vendor firmware, peripheral timing or hardware result is inferred. Only small pure fixture programs were run.
