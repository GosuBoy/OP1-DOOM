/* Ordinary C ABI: R0 address, R1 unique tag; static callback. */
.section .text.input_stock_request,"ax"
.global _input_stock_request
_input_stock_request:
 LINK 0;
 SP += -20;
 [SP+16]=R1;
 P1.L=_input_native_reply;P1.H=_input_native_reply;
 [SP+12]=P1;
 R1=0x12;R2=2;
 P1.L=0x9238;P1.H=0x01a6;
 CALL(P1);
 SP += 20;
 UNLINK;
 RTS;
