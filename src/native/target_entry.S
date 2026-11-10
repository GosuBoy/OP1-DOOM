.section .start,"ax"
.global _start
_start:
 SP.L=0;SP.H=0x0094;SP+=-12;
 CALL _test_events;
 HLT;
