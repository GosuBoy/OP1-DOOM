# Stock plaintext to V49 change ledger

This is a read-only comparison of the pinned stock246 **plaintext main loader** and the pinned historical V49 loader. It does not yet reproduce the encrypted stock-package-to-plaintext step, create a firmware package, or establish installability. Exact input hashes, every differing span's preimage/postimage hash and record identities are in [the ledger receipt](../provenance/stock-to-v49-ledger.json). No raw firmware bytes or bootstrap source are included.

## Complete final-state delta

Stock has 800 records; V49 has 801. Ordered record matching shows one inserted 728-byte COPY payload at 0x00fff000, before FINAL. All other destination/count/control tuples match. The FIRST argument increases by 744 bytes (728 plus its 16-byte header); both opaque tails are identical at 256 bytes. The other matching-record changes total **29 payload bytes**, classified below. Whole fields are listed even when only some bytes differ.

| Field/window | Final-state purpose and evidence |
| --- | --- |
| 0x018c1f54 / 0x018c1f58 | Indexed-template and application-boot counts: seven to one; exact V43 patch preimage/postimage |
| 0x018c1fcc, 32 bytes | Private application template, priority/name/factory; exact V43 patch preimage/postimage |
| 0x018c20cc, 8 bytes | Persistent private boot pair; exact V43 patch preimage/postimage |
| 0x018c2718, 4 bytes | Constructor-dispatch list points to private queue initializer list; exact V43 patch preimage/postimage |
| 0x01919aa8, 4 bytes | Terminal fault diversion; exact V43 patch preimage/postimage |
| 0xffa05836, 8 bytes | IRQ13 producer target points to private no-write leaf; exact V43 patch preimage/postimage |
| 0x018c1ffc, 4 bytes | Old graphics stack count 0x4000 to 0x4100; retained V37 prepare/verify recipe explains the inherited setting |
| 0x018e6290, 4 bytes | Heap0 length reduced by 4092 bytes to reserve private payload; retained V37 recipe and heap table agree |
| 0x018f738a, 1 byte | Diagnostic version-label R to T; retained diagnostic packager identifies this exact single-byte change |

The V37 observer hook at 0x019ab8b2 is restored to stock in V49, matching the explicit V43 restoration specification; there is no residual difference there. All changed matching-record spans fit the classified windows. This establishes the final stock-plaintext→V49 difference and its source explanations. It does not independently reconstruct every intermediate V43/V47/V48/V49 package or license the new private payload.

## Static memory ownership

| Region | Half-open interval | Evidence/limit |
| --- | --- | --- |
| Heap0 | [0x00000100, 0x00fff000) | Exact V49 heap table base/length |
| Private payload record | [0x00fff000, 0x00fff2d8) | 728-byte inserted record within 4092-byte reserve; 3364 bytes of reserve remain |
| Surface0 | [0x02000000, 0x02030000) | V43 private linker/design reservation |
| Surface1 | [0x03000000, 0x03030000) | V43 private linker/design reservation |
| Heap1 | [0x03040000, 0x03fffdb0) | Exact unchanged heap1 table base/length |

All five static intervals are disjoint; the payload stays below the private reserve limit 0x00fffffc. The nine-slot thread-table configuration, separate kernel-idle template, boot/timer hook words, CPLB mode/table state and heap1 table window are unchanged from the pinned stock plaintext loader. Their exact window hashes are in the receipt. The inherited graphics-stack modification is outside the one selected private template; that historical field change is not evidence that its old worker remains running, nor a reason to remove it without a separate review.

These checks do not prove live free heap, allocator admission, transitive stack depth, IRQ concurrency, DMA/cache correctness, power servicing or recovery. In particular, one private application thread plus kernel idle does not recreate the stock USB/file/application workers.

## Next supported step

The final delta can now serve as an exact audit baseline, without a guessed modified parent. Before claiming a reproducible builder, independently verify the encrypted stock246→plaintext derivation and decide the separate bootstrap license scope. Preserve the recorded semantic edits and preimages rather than publishing copied vendor patch bodies. Review the exact output's vendor/SDK/game-data rights and GPL obligations before distributing any combined executable. Power/USB behavior still needs its own service-path investigation and separately authorized hardware validation.

No firmware output was written, no device accessed and TE-BOOT was not modified. Bootstrap files remain private.
