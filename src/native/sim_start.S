/* Test-only entry: absent from native link. */
.section .sim_start,"ax"
.global _sim_start
_sim_start:
 SP.L=0;SP.H=0x0094;
 P0.L=0xff4c;P0.H=0x008f;
 P1.L=0;P1.H=0x0094;R0.L=0xbeef;R0.H=0xdead;
8: CC=P0==P1;IF CC JUMP 9f;[P0++]=R0;JUMP 8b;
9: P0.L=__bss_start;P0.H=__bss_start;P1.L=__bss_end;P1.H=__bss_end;R0=0;
1: CC=P0==P1;IF CC JUMP 2f;[P0++]=R0;JUMP 1b;
2: SP+=-12;P0.L=_sim_test;P0.H=_sim_test;CALL(P0);HLT;
