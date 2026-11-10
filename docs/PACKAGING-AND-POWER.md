# Bootstrap, packaging and power investigation

The audited source release remains the supported public deliverable. The runtime license approval does not cover the separate bootstrap or firmware-container work.

## New offline container evidence

Read-only comparison of the pinned historical V49 and V74 packages verifies that all 115 non-main tar members and their raw headers are identical. Those same members match the local stock246 package; stock additionally contains te-boot.ldr, which V74 omits. V74 therefore still contains substantial vendor material. Member absence does not establish a guaranteed recovery path or prove every updater effect.

The existing loader delta is confined to four intended payload windows (59 changed bytes), the FIRST record's length/checksum, and 21 added ordinary COPY records. Its 256-byte tail and other existing headers are unchanged. The [comparison receipt](../provenance/container-startup-audit.json) records identities and bounds. No package was built, installed or executed for this check.

A source-only stock-to-V49 change ledger, with verified preimages and memory reservations, is the next engineering step before claiming a builder from user-supplied stock firmware. The existing modified-parent recipe is not yet such a public recipe. Vendor/SDK/game-data rights and GPL compliance for the actual combined output remain unresolved. The [manufacturer's IP policy](https://teenage.engineering/ip-rights-te-products) excludes code from its creative-use permission; no firmware redistribution grant is inferred.

## Bootstrap status

The V74 bootstrap traces to the earlier private V43 factory/Run source, with later panel, fill and title/Sleep changes. It reproduces exactly, but its assembly, linker script and builder lack an explicit project license declaration. They remain private pending a separate decision for project-original portions and review of any vendor-derived content. No firmware body or patch-context bytes are being added publicly.

## Power evidence and next investigation

The owner reported that V49 needed USB disconnection for shutdown/recovery consistently, while the same behavior occurred occasionally on stock. Historical battery-only reboot/recovery successes do not prove normal USB or charging behavior for V74.

New offline comparison confirms seven stock application boot templates versus one in V49 and V74. The dedicated design intentionally omits stock application/USB/file workers while retaining kernel idle and the private thread. Missing application-level power/USB service is a plausible lead, not an established root cause. The next offline investigation is to trace stock switch/USB-present events to their consumers and dependencies. Enabling a worker without closing its queues, buffers, callbacks and memory ownership would not be a justified fix.

No battery damage or charger-current conclusion is established by this evidence. No device, firmware installation or TE-BOOT change occurred. Existing [hardware limitations](HARDWARE.md) remain in force; there is no new installation recommendation.

## Stock plaintext-to-V49 ledger completed

The [new final-state ledger](STOCK-TO-V49-LEDGER.md) classifies all 29 changed existing payload bytes, verifies the retained V43 source patch against exact preimages/postimages, and checks static memory ownership. The encrypted stock-to-plaintext derivation, separate bootstrap grant, packaged-output rights and hardware safety remain open.
