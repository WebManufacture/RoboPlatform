   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.11.10 - 06 Jul 2017
   3                     ; Generator (Limited) V4.4.7 - 05 Oct 2017
3045                     ; 3 void UnblockEEPROM(void){
3047                     	switch	.text
3048  0000               _UnblockEEPROM:
3050  0000 88            	push	a
3051       00000001      OFST:	set	1
3054                     ; 5 	dataState = FLASH_IAPSR;
3056  0001 c6505f        	ld	a,_FLASH_IAPSR
3057  0004 6b01          	ld	(OFST+0,sp),a
3059                     ; 6 	dataState &= bit3;
3061  0006 7b01          	ld	a,(OFST+0,sp)
3062  0008 a408          	and	a,#8
3063  000a 6b01          	ld	(OFST+0,sp),a
3065                     ; 7 	if (dataState == 0){
3067  000c 0d01          	tnz	(OFST+0,sp)
3068  000e 2608          	jrne	L1412
3069                     ; 8 		FLASH_DUKR = 0xAE;
3071  0010 35ae5064      	mov	_FLASH_DUKR,#174
3072                     ; 9 		FLASH_DUKR = 0x56;
3074  0014 35565064      	mov	_FLASH_DUKR,#86
3075  0018               L1412:
3076                     ; 11 }
3079  0018 84            	pop	a
3080  0019 81            	ret
3116                     ; 13 void UnblockFLASH(void){
3117                     	switch	.text
3118  001a               _UnblockFLASH:
3120  001a 88            	push	a
3121       00000001      OFST:	set	1
3124                     ; 15 	dataState = FLASH_IAPSR;
3126  001b c6505f        	ld	a,_FLASH_IAPSR
3127  001e 6b01          	ld	(OFST+0,sp),a
3129                     ; 16 	dataState &= bit3;
3131  0020 7b01          	ld	a,(OFST+0,sp)
3132  0022 a408          	and	a,#8
3133  0024 6b01          	ld	(OFST+0,sp),a
3135                     ; 17 	if (dataState == 0){
3137  0026 0d01          	tnz	(OFST+0,sp)
3138  0028 2608          	jrne	L1612
3139                     ; 18 		FLASH_PUKR = 0x56;
3141  002a 35565062      	mov	_FLASH_PUKR,#86
3142                     ; 19 		FLASH_PUKR = 0xAE;
3144  002e 35ae5062      	mov	_FLASH_PUKR,#174
3145  0032               L1612:
3146                     ; 21 }
3149  0032 84            	pop	a
3150  0033 81            	ret
3174                     ; 23 void Reset(void){
3175                     	switch	.text
3176  0034               _Reset:
3180                     ; 24 	WWDG_CR = bit7;
3182  0034 358050d1      	mov	_WWDG_CR,#128
3183                     ; 25 }
3186  0038 81            	ret
3199                     	xdef	_Reset
3200                     	xdef	_UnblockFLASH
3201                     	xdef	_UnblockEEPROM
3220                     	end
