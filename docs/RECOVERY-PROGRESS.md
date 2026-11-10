# Follow-up recovery findings

## One more source file cleared

`src/native/column_cached.c` retains an explicit **GPL version 2 or later** notice with id Software and Simon Howard attribution. The first export's narrow notice filter missed that wording. It is now included byte-for-byte, and its historical license-file link is supplied under THIRD_PARTY_LICENSES. It remains an experimental renderer component; inclusion is not a claim that the V74 production recipe uses it.

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
