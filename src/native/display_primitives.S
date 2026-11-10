/* SPDX-License-Identifier: GPL-2.0-or-later. Actual native primitives;
 * simulator build must substitute the fatal path and MMR reader explicitly. */
.section .text.display_primitives,"ax"
.global _display_lock,_display_unlock,_display_sync,_display_fatal
.global _display_dma_status,_display_shared_source
_display_lock:
    CLI R0;
    RTS;
_display_unlock:
    SSYNC;
    STI R0;
    RTS;
_display_sync:
    SSYNC;
    RTS;
_display_fatal:
    P0.L=0xf178;P0.H=0x00ff;
    JUMP(P0);
_display_dma_status:
    P0.L=0x0c28;P0.H=0xffc0;
    R0=W[P0](Z);
    RTS;
_display_shared_source:
    P0.L=0x64b0;P0.H=0x018b;
    R0=[P0];
    RTS;
