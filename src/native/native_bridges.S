/* SPDX-License-Identifier: GPL-2.0-or-later
 * Explicit ordinary-call bridge with 12-byte outgoing home area. No ROM calls.
 * Linker binds stock symbols either to pinned imports (native) or test stubs. */
.section .text.title_bridges,"ax"
.macro BRIDGE name, target
.global \name
\name:
    LINK 0;
    SP += -12;
    P1.L=\target; P1.H=\target;
    CALL(P1);
    SP += 12;
    UNLINK;
    RTS;
.endm
BRIDGE _title_stock_alloc, _title_import_alloc
BRIDGE _title_stock_uptime, _title_import_uptime
BRIDGE _title_stock_sleep, _title_import_sleep
BRIDGE _title_stock_flush, _title_import_flush
.global _title_stock_publish
_title_stock_publish:
    /* Complete preceding cache writebacks BEFORE exposing the new base. */
    SSYNC;
    P0.L=0x64b0; P0.H=0x018b;
    [P0]=R0;
    SSYNC;
    P0.L=0x64ad; P0.H=0x018b;
    R0=1; B[P0]=R0;
    SSYNC;
    RTS;

.global _title_current_sp
_title_current_sp:
    R0=SP;
    RTS;
