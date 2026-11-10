# Release follow-up

This source-only research release is ready for inspection. A usable firmware builder remains blocked by:

1. Provenance and license review of bootstrap/container code. The 20 runtime files are now published under their approved project-original GPL-2.0-or-later scope.
2. Recovery of remaining historical archive evidence and simulator qualification. A locally built C compiler/backend, binutils, native load and bootstrap are now verified; the original V72/V73 archives are still unavailable. The original pinned patched simulator now passes bounded pure checks; full-program and peripheral validation remain open.
3. A verified stock-firmware-to-update derivation using user-supplied firmware/WAD inputs. Native object/override reconstruction is now verified against the original V74 loaded bytes.
4. Applicable rights and GPL compliance for any combined executable distribution.
5. Review of unresolved USB/power behavior and exact-candidate hardware/recovery testing under separate installation authorization.

Source publication is not evidence that those gates have been completed. Do not attach proprietary recovery archives or firmware packages to public issues or releases.
