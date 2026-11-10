/* SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once
#include <stdint.h>
#define TITLE_POOL_BYTES 7340032u
#define TITLE_RAW_BYTES (TITLE_POOL_BYTES + 127u)
#define TITLE_SOURCE_WIDTH 440u
#define TITLE_SOURCE_HEIGHT 200u
#define TITLE_SOURCE_BYTES (TITLE_SOURCE_WIDTH*TITLE_SOURCE_HEIGHT*4u)
#define TITLE_SURFACE_BYTES 0x30000u
/* Pure arithmetic: no dereference. Returns zero if the complete guarded span
 * cannot be represented or does not fit the supplied allocation. */
uintptr_t title_pool_layout(uintptr_t raw, unsigned bytes);
/* Source is exactly a 440x200 packed 0x00RRGGBB frame. Destination is an owned,
 * unpublished 0x30000-byte native surface, not an arbitrary row pointer list.
 * Caller must establish that both supplied extents belong to live objects.
 * Geometry maps logical(x,y) to native word((375-x)*256+y). The geometry's
 * address extent is proved offline; visual orientation remains a hardware gate.
 * Validation failures write nothing. The caller flushes/publishes afterward. */
int title_surface_convert(const uint32_t *source, unsigned source_bytes,
                          uint16_t *surface, unsigned surface_bytes);
