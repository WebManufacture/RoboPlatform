   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.11.10 - 06 Jul 2017
   3                     ; Generator (Limited) V4.4.7 - 05 Oct 2017
3177                     	bsct
3178  0000               _DeviceAddr:
3179  0000 00            	dc.b	0
3180  0001               _RouterAddr:
3181  0001 00            	dc.b	0
3239                     ; 17 void _msg_routing_SendFactoryNum(char* msg){
3241                     	switch	.text
3242  0000               __msg_routing_SendFactoryNum:
3244  0000 89            	pushw	x
3245  0001 88            	push	a
3246       00000001      OFST:	set	1
3249                     ; 19 	if (msg[1] == 5) {//if (msg->dstType == 5)) {
3251  0002 e601          	ld	a,(1,x)
3252  0004 a105          	cp	a,#5
3253  0006 2604          	jrne	L5422
3254                     ; 20 		msg[1] = 4;
3256  0008 a604          	ld	a,#4
3257  000a e701          	ld	(1,x),a
3258  000c               L5422:
3259                     ; 22 	msg[0] = msg[2];//msg->dstAddr = msg->srcAddr;
3261  000c 1e02          	ldw	x,(OFST+1,sp)
3262  000e e602          	ld	a,(2,x)
3263  0010 f7            	ld	(x),a
3264                     ; 23 	msg[2] = DeviceAddr;	//msg->srcAddr = DeviceAddr;	
3266  0011 1e02          	ldw	x,(OFST+1,sp)
3267  0013 b600          	ld	a,_DeviceAddr
3268  0015 e702          	ld	(2,x),a
3269                     ; 24 	for (i = 0; i < sizeof (struct FactoryRecord); i++){
3271  0017 0f01          	clr	(OFST+0,sp)
3273  0019               L7422:
3274                     ; 25 		msg[i+4] = *((char*)(&DeviceConfig.factoryRec) + i);//msg->data[i] = *((char*)(&DeviceConfig.factoryRec) + i);	
3276  0019 7b01          	ld	a,(OFST+0,sp)
3277  001b 5f            	clrw	x
3278  001c 97            	ld	xl,a
3279  001d 72fb02        	addw	x,(OFST+1,sp)
3280  0020 7b01          	ld	a,(OFST+0,sp)
3281  0022 905f          	clrw	y
3282  0024 9097          	ld	yl,a
3283  0026 90d64000      	ld	a,(_DeviceConfig,y)
3284  002a e704          	ld	(4,x),a
3285                     ; 24 	for (i = 0; i < sizeof (struct FactoryRecord); i++){
3287  002c 0c01          	inc	(OFST+0,sp)
3291  002e 7b01          	ld	a,(OFST+0,sp)
3292  0030 a108          	cp	a,#8
3293  0032 25e5          	jrult	L7422
3294                     ; 27 	UartSendData(msg, 4 + sizeof (struct FactoryRecord));
3296  0034 4b0c          	push	#12
3297  0036 1e03          	ldw	x,(OFST+2,sp)
3298  0038 cd0000        	call	_UartSendData
3300  003b 84            	pop	a
3301                     ; 28 }
3304  003c 5b03          	addw	sp,#3
3305  003e 81            	ret
3353                     ; 30 void _msg_routing_SetDeviceAddr(char* msg){
3354                     	switch	.text
3355  003f               __msg_routing_SetDeviceAddr:
3357  003f 89            	pushw	x
3358  0040 88            	push	a
3359       00000001      OFST:	set	1
3362                     ; 32 	for (i = 0; i < sizeof (struct FactoryRecord); i++){
3364  0041 0f01          	clr	(OFST+0,sp)
3366  0043               L7722:
3367                     ; 33 		if (msg[i+4] != *((char*)(&DeviceConfig.factoryRec) + i)) return;
3369  0043 7b01          	ld	a,(OFST+0,sp)
3370  0045 5f            	clrw	x
3371  0046 97            	ld	xl,a
3372  0047 7b01          	ld	a,(OFST+0,sp)
3373  0049 905f          	clrw	y
3374  004b 9097          	ld	yl,a
3375  004d 72f902        	addw	y,(OFST+1,sp)
3376  0050 90e604        	ld	a,(4,y)
3377  0053 d14000        	cp	a,(_DeviceConfig,x)
3378  0056 2614          	jrne	L01
3381                     ; 32 	for (i = 0; i < sizeof (struct FactoryRecord); i++){
3383  0058 0c01          	inc	(OFST+0,sp)
3387  005a 7b01          	ld	a,(OFST+0,sp)
3388  005c a108          	cp	a,#8
3389  005e 25e3          	jrult	L7722
3390                     ; 35 	DeviceAddr = msg[3];
3392  0060 1e02          	ldw	x,(OFST+1,sp)
3393  0062 e603          	ld	a,(3,x)
3394  0064 b700          	ld	_DeviceAddr,a
3395                     ; 36 	RouterAddr = msg[2];
3397  0066 1e02          	ldw	x,(OFST+1,sp)
3398  0068 e602          	ld	a,(2,x)
3399  006a b701          	ld	_RouterAddr,a
3400                     ; 37 	return;
3401  006c               L01:
3404  006c 5b03          	addw	sp,#3
3405  006e 81            	ret
3469                     ; 40 void _msg_debug_GetMem(char* msg){
3470                     	switch	.text
3471  006f               __msg_debug_GetMem:
3473  006f 89            	pushw	x
3474  0070 5205          	subw	sp,#5
3475       00000005      OFST:	set	5
3478                     ; 44 	uchar counter = 0;
3480  0072 0f05          	clr	(OFST+0,sp)
3482                     ; 45 	if (!msg[2] || !msg[0]) return;
3484  0074 6d02          	tnz	(2,x)
3485  0076 2703          	jreq	L3432
3487  0078 7d            	tnz	(x)
3488  0079 2603          	jrne	L1432
3489  007b               L3432:
3492  007b cc00ff        	jra	L02
3493  007e               L1432:
3494                     ; 46 	addr = (uint)((msg[0+4] << 8) + msg[1+4]);
3496  007e 1e06          	ldw	x,(OFST+1,sp)
3497  0080 e604          	ld	a,(4,x)
3498  0082 5f            	clrw	x
3499  0083 97            	ld	xl,a
3500  0084 4f            	clr	a
3501  0085 02            	rlwa	x,a
3502  0086 01            	rrwa	x,a
3503  0087 1606          	ldw	y,(OFST+1,sp)
3504  0089 90eb05        	add	a,(5,y)
3505  008c 2401          	jrnc	L41
3506  008e 5c            	incw	x
3507  008f               L41:
3508  008f 02            	rlwa	x,a
3509  0090 1f01          	ldw	(OFST-4,sp),x
3510  0092 01            	rrwa	x,a
3512                     ; 47 	count = (uint)((msg[2+4] << 8) + msg[3+4]);
3514  0093 1e06          	ldw	x,(OFST+1,sp)
3515  0095 e606          	ld	a,(6,x)
3516  0097 5f            	clrw	x
3517  0098 97            	ld	xl,a
3518  0099 4f            	clr	a
3519  009a 02            	rlwa	x,a
3520  009b 01            	rrwa	x,a
3521  009c 1606          	ldw	y,(OFST+1,sp)
3522  009e 90eb07        	add	a,(7,y)
3523  00a1 2401          	jrnc	L61
3524  00a3 5c            	incw	x
3525  00a4               L61:
3526  00a4 02            	rlwa	x,a
3527  00a5 1f03          	ldw	(OFST-2,sp),x
3528  00a7 01            	rrwa	x,a
3530                     ; 48 	msg[0] = msg[2];
3532  00a8 1e06          	ldw	x,(OFST+1,sp)
3533  00aa e602          	ld	a,(2,x)
3534  00ac f7            	ld	(x),a
3535                     ; 49 	msg[2] = DeviceAddr;
3537  00ad 1e06          	ldw	x,(OFST+1,sp)
3538  00af b600          	ld	a,_DeviceAddr
3539  00b1 e702          	ld	(2,x),a
3540                     ; 50 	msg[3] = msg[1];
3542  00b3 1e06          	ldw	x,(OFST+1,sp)
3543  00b5 e601          	ld	a,(1,x)
3544  00b7 e703          	ld	(3,x),a
3545                     ; 51 	msg[1] = 10;
3547  00b9 1e06          	ldw	x,(OFST+1,sp)
3548  00bb a60a          	ld	a,#10
3549  00bd e701          	ld	(1,x),a
3551  00bf 202d          	jra	L1532
3552  00c1               L5432:
3553                     ; 53 		msg[counter + 4] = *(char*)addr;
3555  00c1 7b05          	ld	a,(OFST+0,sp)
3556  00c3 5f            	clrw	x
3557  00c4 97            	ld	xl,a
3558  00c5 72fb06        	addw	x,(OFST+1,sp)
3559  00c8 1601          	ldw	y,(OFST-4,sp)
3560  00ca 90f6          	ld	a,(y)
3561  00cc e704          	ld	(4,x),a
3562                     ; 54 		addr++;
3564  00ce 1e01          	ldw	x,(OFST-4,sp)
3565  00d0 1c0001        	addw	x,#1
3566  00d3 1f01          	ldw	(OFST-4,sp),x
3568                     ; 55 		counter++;
3570  00d5 0c05          	inc	(OFST+0,sp)
3572                     ; 56 		if (counter >= MESSAGE_SIZE-4){
3574  00d7 7b05          	ld	a,(OFST+0,sp)
3575  00d9 a110          	cp	a,#16
3576  00db 250a          	jrult	L5532
3577                     ; 57 			UartSendData(msg, MESSAGE_SIZE);
3579  00dd 4b14          	push	#20
3580  00df 1e07          	ldw	x,(OFST+2,sp)
3581  00e1 cd0000        	call	_UartSendData
3583  00e4 84            	pop	a
3584                     ; 58 			counter = 0;
3586  00e5 0f05          	clr	(OFST+0,sp)
3588  00e7               L5532:
3589                     ; 52 	for (;count > 0; count--){
3591  00e7 1e03          	ldw	x,(OFST-2,sp)
3592  00e9 1d0001        	subw	x,#1
3593  00ec 1f03          	ldw	(OFST-2,sp),x
3595  00ee               L1532:
3598  00ee 1e03          	ldw	x,(OFST-2,sp)
3599  00f0 26cf          	jrne	L5432
3600                     ; 61 	if (counter > 0){
3602  00f2 0d05          	tnz	(OFST+0,sp)
3603  00f4 2709          	jreq	L7532
3604                     ; 62 		UartSendData(msg, counter);
3606  00f6 7b05          	ld	a,(OFST+0,sp)
3607  00f8 88            	push	a
3608  00f9 1e07          	ldw	x,(OFST+2,sp)
3609  00fb cd0000        	call	_UartSendData
3611  00fe 84            	pop	a
3612  00ff               L7532:
3613                     ; 64 }
3614  00ff               L02:
3617  00ff 5b07          	addw	sp,#7
3618  0101 81            	ret
3681                     ; 67 void _msg_debug_SetMem(char *msg){
3682                     	switch	.text
3683  0102               __msg_debug_SetMem:
3685  0102 89            	pushw	x
3686  0103 5204          	subw	sp,#4
3687       00000004      OFST:	set	4
3690                     ; 70 	uchar byte = 0;
3692                     ; 71 	if (!msg[2] || !msg[0]) return;
3694  0105 6d02          	tnz	(2,x)
3695  0107 2703          	jreq	L5142
3697  0109 7d            	tnz	(x)
3698  010a 2602          	jrne	L3142
3699  010c               L5142:
3702  010c 203a          	jra	L62
3703  010e               L3142:
3704                     ; 72 	bytesCount = msg[3];
3706  010e 1e05          	ldw	x,(OFST+1,sp)
3707  0110 e603          	ld	a,(3,x)
3708  0112 6b04          	ld	(OFST+0,sp),a
3710                     ; 73 	addr = (char*)((uint)((msg[0+4] << 8) + msg[1+4]));
3712  0114 1e05          	ldw	x,(OFST+1,sp)
3713  0116 e604          	ld	a,(4,x)
3714  0118 5f            	clrw	x
3715  0119 97            	ld	xl,a
3716  011a 4f            	clr	a
3717  011b 02            	rlwa	x,a
3718  011c 01            	rrwa	x,a
3719  011d 1605          	ldw	y,(OFST+1,sp)
3720  011f 90eb05        	add	a,(5,y)
3721  0122 2401          	jrnc	L42
3722  0124 5c            	incw	x
3723  0125               L42:
3724  0125 02            	rlwa	x,a
3725  0126 1f01          	ldw	(OFST-3,sp),x
3726  0128 01            	rrwa	x,a
3729  0129 2019          	jra	L3242
3730  012b               L7142:
3731                     ; 75 		byte = msg[(bytesCount-1)+2+4];
3733  012b 7b04          	ld	a,(OFST+0,sp)
3734  012d 5f            	clrw	x
3735  012e 97            	ld	xl,a
3736  012f 72fb05        	addw	x,(OFST+1,sp)
3737  0132 e605          	ld	a,(5,x)
3738  0134 6b03          	ld	(OFST-1,sp),a
3740                     ; 76 		*addr = byte;
3742  0136 7b03          	ld	a,(OFST-1,sp)
3743  0138 1e01          	ldw	x,(OFST-3,sp)
3744  013a f7            	ld	(x),a
3745                     ; 77 		addr++;
3747  013b 1e01          	ldw	x,(OFST-3,sp)
3748  013d 1c0001        	addw	x,#1
3749  0140 1f01          	ldw	(OFST-3,sp),x
3751                     ; 74 	for (;bytesCount > 0; bytesCount--){
3753  0142 0a04          	dec	(OFST+0,sp)
3755  0144               L3242:
3758  0144 0d04          	tnz	(OFST+0,sp)
3759  0146 26e3          	jrne	L7142
3760                     ; 79 }
3761  0148               L62:
3764  0148 5b06          	addw	sp,#6
3765  014a 81            	ret
3800                     ; 82 void _msg_debug_CopyMem(char* msg){
3801                     	switch	.text
3802  014b               __msg_debug_CopyMem:
3806                     ; 84 }
3809  014b 81            	ret
3847                     ; 87 void _msg_debug_UnblockMem(char* msg){
3848                     	switch	.text
3849  014c               __msg_debug_UnblockMem:
3853                     ; 88 	if (!msg[2] || !msg[0]) return;
3855  014c 6d02          	tnz	(2,x)
3856  014e 2703          	jreq	L5642
3858  0150 7d            	tnz	(x)
3859  0151 2601          	jrne	L3642
3860  0153               L5642:
3864  0153 81            	ret
3865  0154               L3642:
3866                     ; 89 	UnblockEEPROM();
3868  0154 cd0000        	call	_UnblockEEPROM
3870                     ; 90 	UnblockFLASH();
3872  0157 cd0000        	call	_UnblockFLASH
3874                     ; 91 }
3877  015a 81            	ret
3912                     ; 94 void _msg_debug_Call(char* msg){
3913                     	switch	.text
3914  015b               __msg_debug_Call:
3918                     ; 96 }
3921  015b 81            	ret
3956                     ; 99 void _msg_debug_GoTo(char* msg){
3957                     	switch	.text
3958  015c               __msg_debug_GoTo:
3962                     ; 101 }
3965  015c 81            	ret
4002                     ; 104 void _msg_debug_ResetDevice(char* msg){
4003                     	switch	.text
4004  015d               __msg_debug_ResetDevice:
4008                     ; 105 	if (!msg[2] || !msg[0]) return;
4010  015d 6d02          	tnz	(2,x)
4011  015f 2703          	jreq	L3452
4013  0161 7d            	tnz	(x)
4014  0162 2601          	jrne	L1452
4015  0164               L3452:
4019  0164 81            	ret
4020  0165               L1452:
4021                     ; 106 	Reset();
4023  0165 cd0000        	call	_Reset
4025                     ; 107 }
4028  0168 81            	ret
4067                     ; 109 schar ProcessPacket(char* data){
4068                     	switch	.text
4069  0169               _ProcessPacket:
4071  0169 89            	pushw	x
4072       00000000      OFST:	set	0
4075                     ; 110 	switch (data[1]){
4077  016a e601          	ld	a,(1,x)
4079                     ; 116 		return 1;
4080  016c a002          	sub	a,#2
4081  016e 2706          	jreq	L5452
4082  0170 a003          	sub	a,#3
4083  0172 270b          	jreq	L7452
4084  0174 2012          	jra	L1752
4085  0176               L5452:
4086                     ; 111 		case 2: 
4086                     ; 112 			_msg_routing_SetDeviceAddr(data);
4088  0176 1e01          	ldw	x,(OFST+1,sp)
4089  0178 cd003f        	call	__msg_routing_SetDeviceAddr
4091                     ; 113 		return 1;
4093  017b a601          	ld	a,#1
4095  017d 2007          	jra	L44
4096  017f               L7452:
4097                     ; 114 		case 5: 
4097                     ; 115 			_msg_routing_SendFactoryNum(data);
4099  017f 1e01          	ldw	x,(OFST+1,sp)
4100  0181 cd0000        	call	__msg_routing_SendFactoryNum
4102                     ; 116 		return 1;
4104  0184 a601          	ld	a,#1
4106  0186               L44:
4108  0186 85            	popw	x
4109  0187 81            	ret
4110  0188               L1752:
4111                     ; 118 	if (data[0] != DeviceAddr) return 1;
4113  0188 1e01          	ldw	x,(OFST+1,sp)
4114  018a f6            	ld	a,(x)
4115  018b b100          	cp	a,_DeviceAddr
4116  018d 2704          	jreq	L3752
4119  018f a601          	ld	a,#1
4121  0191 20f3          	jra	L44
4122  0193               L3752:
4123                     ; 119 	if (data[1] >= 13 && data[1] < 20) {
4125  0193 1e01          	ldw	x,(OFST+1,sp)
4126  0195 e601          	ld	a,(1,x)
4127  0197 a10d          	cp	a,#13
4128  0199 2522          	jrult	L5752
4130  019b 1e01          	ldw	x,(OFST+1,sp)
4131  019d e601          	ld	a,(1,x)
4132  019f a114          	cp	a,#20
4133  01a1 241a          	jruge	L5752
4134                     ; 120 		Handlers[data[1] - 13](data);
4136  01a3 1e01          	ldw	x,(OFST+1,sp)
4137  01a5 1601          	ldw	y,(OFST+1,sp)
4138  01a7 90e601        	ld	a,(1,y)
4139  01aa 905f          	clrw	y
4140  01ac 9097          	ld	yl,a
4141  01ae 9058          	sllw	y
4142  01b0 72a2001a      	subw	y,#26
4143  01b4 90ee00        	ldw	y,(_Handlers,y)
4144  01b7 90fd          	call	(y)
4146                     ; 121 		return 1;
4148  01b9 a601          	ld	a,#1
4150  01bb 20c9          	jra	L44
4151  01bd               L5752:
4152                     ; 123 	return 0;
4154  01bd 4f            	clr	a
4156  01be 20c6          	jra	L44
4191                     ; 126 signed char __evt_init_debug(char event, char* data){
4192                     	switch	.text
4193  01c0               ___evt_init_debug:
4197                     ; 127 	InitDebug();
4199  01c0 ad01          	call	_InitDebug
4201                     ; 128 }
4204  01c2 81            	ret
4255                     ; 130 void InitDebug(void)
4255                     ; 131 {
4256                     	switch	.text
4257  01c3               _InitDebug:
4259  01c3 5214          	subw	sp,#20
4260       00000014      OFST:	set	20
4263                     ; 133 	Handlers[0] = _msg_debug_GetMem; 
4265  01c5 ae006f        	ldw	x,#__msg_debug_GetMem
4266  01c8 bf00          	ldw	_Handlers,x
4267                     ; 134 	Handlers[1] = _msg_debug_SetMem; 
4269  01ca ae0102        	ldw	x,#__msg_debug_SetMem
4270  01cd bf02          	ldw	_Handlers+2,x
4271                     ; 135 	Handlers[2] = _msg_debug_CopyMem;
4273  01cf ae014b        	ldw	x,#__msg_debug_CopyMem
4274  01d2 bf04          	ldw	_Handlers+4,x
4275                     ; 136 	Handlers[3] = _msg_debug_UnblockMem;	
4277  01d4 ae014c        	ldw	x,#__msg_debug_UnblockMem
4278  01d7 bf06          	ldw	_Handlers+6,x
4279                     ; 137 	Handlers[4] = _msg_debug_Call;
4281  01d9 ae015b        	ldw	x,#__msg_debug_Call
4282  01dc bf08          	ldw	_Handlers+8,x
4283                     ; 138 	Handlers[5] = _msg_debug_GoTo;
4285  01de ae015c        	ldw	x,#__msg_debug_GoTo
4286  01e1 bf0a          	ldw	_Handlers+10,x
4287                     ; 139 	Handlers[6] = _msg_debug_ResetDevice;
4289  01e3 ae015d        	ldw	x,#__msg_debug_ResetDevice
4290  01e6 bf0c          	ldw	_Handlers+12,x
4291                     ; 140 	data[1] = 01;
4293  01e8 a601          	ld	a,#1
4294  01ea 6b02          	ld	(OFST-18,sp),a
4296                     ; 141 	data[3] = 01;
4298  01ec a601          	ld	a,#1
4299  01ee 6b04          	ld	(OFST-16,sp),a
4301                     ; 142 	_msg_routing_SendFactoryNum(data);
4303  01f0 96            	ldw	x,sp
4304  01f1 1c0001        	addw	x,#OFST-19
4305  01f4 cd0000        	call	__msg_routing_SendFactoryNum
4307                     ; 143 }
4310  01f7 5b14          	addw	sp,#20
4311  01f9 81            	ret
4356                     	xdef	__msg_debug_ResetDevice
4357                     	xdef	__msg_debug_GoTo
4358                     	xdef	__msg_debug_Call
4359                     	xdef	__msg_debug_UnblockMem
4360                     	xdef	__msg_debug_CopyMem
4361                     	xdef	__msg_debug_SetMem
4362                     	xdef	__msg_debug_GetMem
4363                     	xdef	__msg_routing_SetDeviceAddr
4364                     	xdef	__msg_routing_SendFactoryNum
4365                     	switch	.ubsct
4366  0000               _Handlers:
4367  0000 000000000000  	ds.b	14
4368                     	xdef	_Handlers
4369                     	xref	_UartSendData
4370                     	xdef	_RouterAddr
4371                     	xdef	_DeviceAddr
4372                     	xdef	_ProcessPacket
4373                     	xdef	_InitDebug
4374                     	xdef	___evt_init_debug
4375                     	xref	_Reset
4376                     	xref	_UnblockFLASH
4377                     	xref	_UnblockEEPROM
4397                     	end
