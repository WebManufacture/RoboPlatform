   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.11.10 - 06 Jul 2017
   3                     ; Generator (Limited) V4.4.7 - 05 Oct 2017
3177                     	bsct
3178  0000               _readDataBuf:
3179  0000 0000          	dc.w	_buffers
3180  0002               _writeDataBuf:
3181  0002 029e          	dc.w	_buffers+670
3323                     ; 23 main()
3323                     ; 24 {	
3325                     	switch	.text
3326  0000               _main:
3328  0000 88            	push	a
3329       00000001      OFST:	set	1
3332                     ; 32   PROGRAM_PIN_DDR = 0;
3334  0001 72135002      	bres	_PpA+2,#1
3335                     ; 34   if (PROGRAM_PIN_IN == 0){
3337  0005 c65001        	ld	a,_PpA+1
3338  0008 a502          	bcp	a,#2
3339  000a 2606          	jrne	L5032
3340                     ; 36     _asm("JPF $8500");
3343  000c ac008500      JPF $8500
3346  0010               L7032:
3347                     ; 64 }
3350  0010 84            	pop	a
3351  0011 81            	ret
3352  0012               L5032:
3353                     ; 38     InitUART(); //115200
3355  0012 cd0000        	call	_InitUART
3357                     ; 39     InitDebug();
3359  0015 cd0000        	call	_InitDebug
3361                     ; 40     UnblockEEPROM();
3363  0018 cd0000        	call	_UnblockEEPROM
3365                     ; 42     readDataBuf[0] = 0;
3367  001b 923f00        	clr	[_readDataBuf.w]
3368                     ; 43     readDataBuf[1] = 0;
3370  001e be00          	ldw	x,_readDataBuf
3371  0020 6f01          	clr	(1,x)
3372                     ; 44     readDataBuf[2] = 0;
3374  0022 be00          	ldw	x,_readDataBuf
3375  0024 6f02          	clr	(2,x)
3376                     ; 45     readDataBuf[3] = 0;
3378  0026 be00          	ldw	x,_readDataBuf
3379  0028 6f03          	clr	(3,x)
3380                     ; 47     PROGRAM_PIN_DDR = 1;
3382  002a 72125002      	bset	_PpA+2,#1
3383                     ; 49     _asm("RIM");
3386  002e 9a            RIM
3388                     ; 51     PROGRAM_PIN_OUT = 1;
3390  002f 72125000      	bset	_PpA,#1
3392  0033 200c          	jra	L5132
3393  0035               L1132:
3394                     ; 56       if (CheckUart()){
3396  0035 cd0000        	call	_CheckUart
3398  0038 4d            	tnz	a
3399  0039 2706          	jreq	L5132
3400                     ; 57         if (ProcessPacket(readDataBuf)){
3402  003b be00          	ldw	x,_readDataBuf
3403  003d cd0000        	call	_ProcessPacket
3405  0040 4d            	tnz	a
3406  0041               L5132:
3407                     ; 53     while (!deviceState.ResetFlag){
3409  0041 7b01          	ld	a,(OFST+0,sp)
3410  0043 a501          	bcp	a,#1
3411  0045 27ee          	jreq	L1132
3412                     ; 62     Reset();
3414  0047 cd0000        	call	_Reset
3416  004a 20c4          	jra	L7032
3488                     	xdef	_main
3489                     	xdef	_writeDataBuf
3490                     	xdef	_readDataBuf
3491                     	switch	.bss
3492  0000               _buffers:
3493  0000 000000000000  	ds.b	702
3494                     	xdef	_buffers
3495                     	xref	_ProcessPacket
3496                     	xref	_InitDebug
3497                     	xref	_CheckUart
3498                     	xref	_InitUART
3499                     	xref	_Reset
3500                     	xref	_UnblockEEPROM
3520                     	end
