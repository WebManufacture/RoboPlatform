   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.11.10 - 06 Jul 2017
   3                     ; Generator (Limited) V4.4.7 - 05 Oct 2017
3215                     ; 16 char InitUART(){
3217                     	switch	.text
3218  0000               _InitUART:
3222                     ; 17 	UART_CR1 = 0; //PARITY ODD + 9Bit (8d+1p)
3224  0000 725f5234      	clr	_UART1_CR1
3225                     ; 18 	UART_CR2 = bit5 + bit3 + bit2; //  RXIE + RX + TX
3227  0004 352c5235      	mov	_UART1_CR2,#44
3228                     ; 19 	UART_CR3 = 0; //1 STOP
3230  0008 725f5236      	clr	_UART1_CR3
3231                     ; 20 	UART_CR4 = 0;
3233  000c 725f5237      	clr	_UART1_CR4
3234                     ; 21 	UART_CR5 = 0;
3236  0010 725f5238      	clr	_UART1_CR5
3237                     ; 30 	UART_BRR2 = 0x0B08 >> 8;
3239  0014 350b5233      	mov	_UART1_BRR2,#11
3240                     ; 31 	UART_BRR1 = 0x0B08;
3242  0018 35085232      	mov	_UART1_BRR1,#8
3243                     ; 32 }
3246  001c 81            	ret
3270                     ; 34 char CheckUart(void){
3271                     	switch	.text
3272  001d               _CheckUart:
3276                     ; 35 	return urState.phase == 06 ? urState.size : 0;
3278  001d b604          	ld	a,_urState
3279  001f a106          	cp	a,#6
3280  0021 2604          	jrne	L01
3281  0023 b605          	ld	a,_urState+1
3282  0025 2001          	jra	L21
3283  0027               L01:
3284  0027 4f            	clr	a
3285  0028               L21:
3288  0028 81            	ret
3314                     ; 38 char* GetUartData(void){
3315                     	switch	.text
3316  0029               _GetUartData:
3320                     ; 39 	if (urState.phase >= 6){
3322  0029 b604          	ld	a,_urState
3323  002b a106          	cp	a,#6
3324  002d 2503          	jrult	L3522
3325                     ; 40 		return readDataBuf;
3327  002f be00          	ldw	x,_readDataBuf
3330  0031 81            	ret
3331  0032               L3522:
3332                     ; 43 		return 0;
3334  0032 5f            	clrw	x
3337  0033 81            	ret
3361                     ; 47 void ClearUart(void){
3362                     	switch	.text
3363  0034               _ClearUart:
3367                     ; 48 	urState.phase = 0;
3369  0034 3f04          	clr	_urState
3370                     ; 49 }
3373  0036 81            	ret
3397                     ; 51 void WaitSend(void){
3398                     	switch	.text
3399  0037               _WaitSend:
3403  0037               L1032:
3404                     ; 52 	while(uwState.phase > 0 && uwState.phase < 3){
3406  0037 3d00          	tnz	_uwState
3407  0039 2706          	jreq	L5032
3409  003b b600          	ld	a,_uwState
3410  003d a103          	cp	a,#3
3411  003f 25f6          	jrult	L1032
3412  0041               L5032:
3413                     ; 55 }
3416  0041 81            	ret
3453                     ; 57 void UartSendData(uchar size){
3454                     	switch	.text
3455  0042               _UartSendData:
3457  0042 88            	push	a
3458       00000000      OFST:	set	0
3461                     ; 58 	WaitSend();
3463  0043 adf2          	call	_WaitSend
3465                     ; 62 	uwState.phase = 0;
3467  0045 3f00          	clr	_uwState
3468                     ; 64 	uwState.index = 0;
3470  0047 3f02          	clr	_uwState+2
3471                     ; 65 	uwState.size = size;
3473  0049 7b01          	ld	a,(OFST+1,sp)
3474  004b b701          	ld	_uwState+1,a
3475                     ; 69 	UART_CR2 |= bit7;//(TIEN) TXE interrupt	
3477  004d 721e5235      	bset	_UART1_CR2,#7
3478                     ; 70 }
3481  0051 84            	pop	a
3482  0052 81            	ret
3520                     ; 72 void UART_RX_handler(void){
3521                     	switch	.text
3522  0053               _UART_RX_handler:
3524  0053 88            	push	a
3525       00000001      OFST:	set	1
3528                     ; 75 	_asm("SIM");
3531  0054 9b            SIM
3533                     ; 76 	res(UART_SR,5);
3535  0055 721b5230      	bres	_UART1_SR,#5
3536                     ; 77 	val = UART_DR;
3539  0059 c65231        	ld	a,_UART1_DR
3540  005c 6b01          	ld	(OFST+0,sp),a
3542                     ; 78 	switch (urState.phase){
3544  005e b604          	ld	a,_urState
3546                     ; 113 		break;
3547  0060 4d            	tnz	a
3548  0061 2760          	jreq	L3332
3549  0063 4a            	dec	a
3550  0064 2747          	jreq	L1332
3551  0066 4a            	dec	a
3552  0067 2712          	jreq	L7232
3553  0069 4a            	dec	a
3554  006a 266d          	jrne	L5532
3555                     ; 79 		case 3: 
3555                     ; 80 			urState.phase = val == 3 ? 6 : 0;
3557  006c 7b01          	ld	a,(OFST+0,sp)
3558  006e a103          	cp	a,#3
3559  0070 2604          	jrne	L62
3560  0072 a606          	ld	a,#6
3561  0074 2001          	jra	L03
3562  0076               L62:
3563  0076 4f            	clr	a
3564  0077               L03:
3565  0077 b704          	ld	_urState,a
3566                     ; 81 		break;
3568  0079 205e          	jra	L5532
3569  007b               L7232:
3570                     ; 82 		case 2:
3570                     ; 83 			if (urState.index >= urState.size){
3572  007b b606          	ld	a,_urState+2
3573  007d b105          	cp	a,_urState+1
3574  007f 2519          	jrult	L7532
3575                     ; 84 				urState.phase = val == 3 ? 6 : (val == urState.crcCode ? 3 : 0);
3577  0081 7b01          	ld	a,(OFST+0,sp)
3578  0083 a103          	cp	a,#3
3579  0085 2604          	jrne	L23
3580  0087 a606          	ld	a,#6
3581  0089 200b          	jra	L43
3582  008b               L23:
3583  008b 7b01          	ld	a,(OFST+0,sp)
3584  008d b107          	cp	a,_urState+3
3585  008f 2604          	jrne	L63
3586  0091 a603          	ld	a,#3
3587  0093 2001          	jra	L04
3588  0095               L63:
3589  0095 4f            	clr	a
3590  0096               L04:
3591  0096               L43:
3592  0096 b704          	ld	_urState,a
3594  0098 203f          	jra	L5532
3595  009a               L7532:
3596                     ; 87 				readDataBuf[urState.index] = val;
3598  009a b606          	ld	a,_urState+2
3599  009c 5f            	clrw	x
3600  009d 97            	ld	xl,a
3601  009e 7b01          	ld	a,(OFST+0,sp)
3602  00a0 92d700        	ld	([_readDataBuf.w],x),a
3603                     ; 88 				urState.crcCode ^= val;
3605  00a3 b607          	ld	a,_urState+3
3606  00a5 1801          	xor	a,	(OFST+0,sp)
3607  00a7 b707          	ld	_urState+3,a
3608                     ; 89 				urState.index++;
3610  00a9 3c06          	inc	_urState+2
3611  00ab 202c          	jra	L5532
3612  00ad               L1332:
3613                     ; 92 		case 1:
3613                     ; 93 			if (val > 0){
3615  00ad 0d01          	tnz	(OFST+0,sp)
3616  00af 270a          	jreq	L3632
3617                     ; 94 				urState.phase = 2;
3619  00b1 35020004      	mov	_urState,#2
3620                     ; 95 				urState.size = val;				
3622  00b5 7b01          	ld	a,(OFST+0,sp)
3623  00b7 b705          	ld	_urState+1,a
3625  00b9 2004          	jra	L5632
3626  00bb               L3632:
3627                     ; 98 				urState.phase = 0;
3629  00bb 3f04          	clr	_urState
3630                     ; 99 				urState.size = 0;
3632  00bd 3f05          	clr	_urState+1
3633  00bf               L5632:
3634                     ; 101 			urState.index = 0;
3636  00bf 3f06          	clr	_urState+2
3637                     ; 102 		break;
3639  00c1 2016          	jra	L5532
3640  00c3               L3332:
3641                     ; 103 		case 0:
3641                     ; 104 			if (val == 1 || val == 2){
3643  00c3 7b01          	ld	a,(OFST+0,sp)
3644  00c5 a101          	cp	a,#1
3645  00c7 2706          	jreq	L1732
3647  00c9 7b01          	ld	a,(OFST+0,sp)
3648  00cb a102          	cp	a,#2
3649  00cd 2608          	jrne	L7632
3650  00cf               L1732:
3651                     ; 108 				urState.crcCode = 222;
3653  00cf 35de0007      	mov	_urState+3,#222
3654                     ; 110 				urState.phase = val;
3656  00d3 7b01          	ld	a,(OFST+0,sp)
3657  00d5 b704          	ld	_urState,a
3658  00d7               L7632:
3659                     ; 112 			urState.index = 0;
3661  00d7 3f06          	clr	_urState+2
3662                     ; 113 		break;
3664  00d9               L5532:
3665                     ; 115 	_asm("RIM");
3668  00d9 9a            RIM
3670                     ; 116 }
3673  00da 84            	pop	a
3674  00db 81            	ret
3712                     ; 118 void UART_TX_handler(void){
3713                     	switch	.text
3714  00dc               _UART_TX_handler:
3716  00dc 88            	push	a
3717       00000001      OFST:	set	1
3720                     ; 121 	_asm("SIM");
3723  00dd 9b            SIM
3725                     ; 122 	if (uwState.index >= uwState.size){
3727  00de b602          	ld	a,_uwState+2
3728  00e0 b101          	cp	a,_uwState+1
3729  00e2 2516          	jrult	L1142
3730                     ; 127 			if (uwState.phase < 6){
3732  00e4 b600          	ld	a,_uwState
3733  00e6 a106          	cp	a,#6
3734  00e8 240a          	jruge	L3142
3735                     ; 138 					UART_DR = 03;//EOT
3737  00ea 35035231      	mov	_UART1_DR,#3
3738                     ; 139 					uwState.phase = 6;
3740  00ee 35060000      	mov	_uwState,#6
3742  00f2 203d          	jra	L7142
3743  00f4               L3142:
3744                     ; 143 				UART_CR2 = UART_CR2 & ~bit7; //UART TX OFF
3746  00f4 721f5235      	bres	_UART1_CR2,#7
3747  00f8 2037          	jra	L7142
3748  00fa               L1142:
3749                     ; 148 		if (uwState.phase == 2){
3751  00fa b600          	ld	a,_uwState
3752  00fc a102          	cp	a,#2
3753  00fe 2616          	jrne	L1242
3754                     ; 149 			val = writeDataBuf[uwState.index];
3756  0100 b602          	ld	a,_uwState+2
3757  0102 5f            	clrw	x
3758  0103 97            	ld	xl,a
3759  0104 92d600        	ld	a,([_writeDataBuf.w],x)
3760  0107 6b01          	ld	(OFST+0,sp),a
3762                     ; 150 			UART_DR = val;
3764  0109 7b01          	ld	a,(OFST+0,sp)
3765  010b c75231        	ld	_UART1_DR,a
3766                     ; 151 			uwState.index++;			
3768  010e 3c02          	inc	_uwState+2
3769                     ; 152 			uwState.crcCode ^= val;
3771  0110 b603          	ld	a,_uwState+3
3772  0112 1801          	xor	a,	(OFST+0,sp)
3773  0114 b703          	ld	_uwState+3,a
3774  0116               L1242:
3775                     ; 154 		if (uwState.phase == 1){
3777  0116 b600          	ld	a,_uwState
3778  0118 a101          	cp	a,#1
3779  011a 2609          	jrne	L3242
3780                     ; 155 			UART_DR = uwState.size;
3782  011c 5500015231    	mov	_UART1_DR,_uwState+1
3783                     ; 156 			uwState.phase = 2;
3785  0121 35020000      	mov	_uwState,#2
3786  0125               L3242:
3787                     ; 158 		if (uwState.phase == 0){
3789  0125 3d00          	tnz	_uwState
3790  0127 2608          	jrne	L7142
3791                     ; 163 			UART_DR = 1;//SOH
3793  0129 35015231      	mov	_UART1_DR,#1
3794                     ; 164 			uwState.phase = 1;
3796  012d 35010000      	mov	_uwState,#1
3797  0131               L7142:
3798                     ; 171 	_asm("RIM");
3801  0131 9a            RIM
3803                     ; 172 }
3806  0132 84            	pop	a
3807  0133 81            	ret
3831                     ; 175 @interrupt void UART_RX_IRQHandler(void){
3832                     	switch	.text
3833  0134               _UART_RX_IRQHandler:
3835  0134 8a            	push	cc
3836  0135 84            	pop	a
3837  0136 a4bf          	and	a,#191
3838  0138 88            	push	a
3839  0139 86            	pop	cc
3840  013a 3b0002        	push	c_x+2
3841  013d be00          	ldw	x,c_x
3842  013f 89            	pushw	x
3843  0140 3b0002        	push	c_y+2
3844  0143 be00          	ldw	x,c_y
3845  0145 89            	pushw	x
3848                     ; 176 	UART_RX_handler();
3850  0146 cd0053        	call	_UART_RX_handler
3852                     ; 177 }
3855  0149 85            	popw	x
3856  014a bf00          	ldw	c_y,x
3857  014c 320002        	pop	c_y+2
3858  014f 85            	popw	x
3859  0150 bf00          	ldw	c_x,x
3860  0152 320002        	pop	c_x+2
3861  0155 80            	iret
3885                     ; 179 @interrupt void UART_TX_IRQHandler(void){
3886                     	switch	.text
3887  0156               _UART_TX_IRQHandler:
3889  0156 8a            	push	cc
3890  0157 84            	pop	a
3891  0158 a4bf          	and	a,#191
3892  015a 88            	push	a
3893  015b 86            	pop	cc
3894  015c 3b0002        	push	c_x+2
3895  015f be00          	ldw	x,c_x
3896  0161 89            	pushw	x
3897  0162 3b0002        	push	c_y+2
3898  0165 be00          	ldw	x,c_y
3899  0167 89            	pushw	x
3902                     ; 180 	UART_TX_handler();
3904  0168 cd00dc        	call	_UART_TX_handler
3906                     ; 181 }
3909  016b 85            	popw	x
3910  016c bf00          	ldw	c_y,x
3911  016e 320002        	pop	c_y+2
3912  0171 85            	popw	x
3913  0172 bf00          	ldw	c_x,x
3914  0174 320002        	pop	c_x+2
3915  0177 80            	iret
3985                     	xdef	_UART_TX_IRQHandler
3986                     	xdef	_UART_RX_IRQHandler
3987                     	xdef	_UART_TX_handler
3988                     	xdef	_UART_RX_handler
3989                     	xdef	_WaitSend
3990                     	xref.b	_writeDataBuf
3991                     	xref.b	_readDataBuf
3992                     	switch	.ubsct
3993  0000               _uwState:
3994  0000 00000000      	ds.b	4
3995                     	xdef	_uwState
3996  0004               _urState:
3997  0004 00000000      	ds.b	4
3998                     	xdef	_urState
3999                     	xdef	_UartSendData
4000                     	xdef	_ClearUart
4001                     	xdef	_GetUartData
4002                     	xdef	_CheckUart
4003                     	xdef	_InitUART
4004                     	xref.b	c_x
4005                     	xref.b	c_y
4025                     	end
