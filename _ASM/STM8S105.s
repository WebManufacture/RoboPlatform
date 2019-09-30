PA_ODR:	 equ 0x5000
PA_IDR:	 equ 0x5001
PA_DDR:	 equ 0x5002
PA_CR1:	 equ 0x5003
PA_CR2:	 equ 0x5004
PB_ODR:	 equ 0x5005
PB_IDR:	 equ 0x5006
PB_DDR:	 equ 0x5007
PB_CR1:	 equ 0x5008
PB_CR2:	 equ 0x5009
PC_ODR:	 equ 0x500A
PC_IDR:	 equ 0x500B
PC_DDR:	 equ 0x500C
PC_CR1:	 equ 0x500D
PC_CR2:	 equ 0x500E
PD_ODR:	 equ 0x500F
PD_IDR:	 equ 0x5010
PD_DDR:	 equ 0x5011
PD_CR1:	 equ 0x5012
PD_CR2:	 equ 0x5013
PE_ODR:	 equ 0x5014
PE_IDR:	 equ 0x5015
PE_DDR:	 equ 0x5016
PE_CR1:	 equ 0x5017
PE_CR2:	 equ 0x5018
PG_ODR:	 equ 0x501E
PG_IDR:	 equ 0x501F
PG_DDR:	 equ 0x5020
PG_CR1:	 equ 0x5021
PG_CR2:	 equ 0x5022
FLASH_CR1:	 equ 0x505A
FLASH_CR2:	 equ 0x505B
FLASH_NCR2:	 equ 0x505C
FLASH_FPR:	 equ 0x505D
FLASH_NFPR:	 equ 0x505E
FLASH_IAPSR:	 equ 0x505F
FLASH_PUKR:	 equ 0x5062
FLASH_DUKR:	 equ 0x5064
EXTI_CR1:	 equ 0x50A0
EXTI_CR2:	 equ 0x50A1
RST_SR:	 equ 0x50B3
CLK_ICKR:	 equ 0x50C0
CLK_ECKR:	 equ 0x50C1
CLK_CMSR:	 equ 0x50C3
CLK_SWR:	 equ 0x50C4
CLK_SWCR:	 equ 0x50C5
CLK_CKDIVR:	 equ 0x50C6
CLK_PCKENR1:	 equ 0x50C7
CLK_CSSR:	 equ 0x50C8
CLK_CCOR:	 equ 0x50C9
CLK_PCKENR2:	 equ 0x50CA
CLK_CANCCR:	 equ 0x50CB
CLK_HSITRIMR:	 equ 0x50CC
CLK_SWIMCCR:	 equ 0x50CD
WWDG_CR:	 equ 0x50D1
WWDG_WR:	 equ 0x50D2
IWDG_KR:	 equ 0x50E0
IWDG_PR:	 equ 0x50E1
IWDG_RLR:	 equ 0x50E2
AWU_CSR:	 equ 0x50F0
AWU_APR:	 equ 0x50F1
AWU_TBR:	 equ 0x50F2
BEEP_CSR:	 equ 0x50F3
SPI_CR1:	 equ 0x5200
SPI_CR2:	 equ 0x5201
SPI_ICR:	 equ 0x5202
SPI_SR:	 equ 0x5203
SPI_DR:	 equ 0x5204
SPI_CRCPR:	 equ 0x5205
SPI_RXCRCR:	 equ 0x5206
SPI_TXCRCR:	 equ 0x5207
I2C_CR1:	 equ 0x5210
I2C_CR2:	 equ 0x5211
I2C_FREQR:	 equ 0x5212
I2C_OARL:	 equ 0x5213
I2C_OARH:	 equ 0x5214
I2C_DR:	 equ 0x5216
I2C_SR1:	 equ 0x5217
I2C_SR2:	 equ 0x5218
I2C_SR3:	 equ 0x5219
I2C_ITR:	 equ 0x521A
I2C_CCRL:	 equ 0x521B
I2C_CCRH:	 equ 0x521C
I2C_TRISER:	 equ 0x521D
I2C_PECR:	 equ 0x521E
UART2_SR:	 equ 0x5240
UART2_DR:	 equ 0x5241
UART2_BRR1:	 equ 0x5242
UART2_BRR2:	 equ 0x5243
UART2_CR1:	 equ 0x5244
UART2_CR2:	 equ 0x5245
UART2_CR3:	 equ 0x5246
UART2_CR4:	 equ 0x5247
UART2_CR6:	 equ 0x5249
UART2_GTR:	 equ 0x524A
UART2_PSCR:	 equ 0x524B
TIM1_CR1:	 equ 0x5250
TIM1_CR2:	 equ 0x5251
TIM1_SMCR:	 equ 0x5252
TIM1_ETR:	 equ 0x5253
TIM1_IER:	 equ 0x5254
TIM1_SR1:	 equ 0x5255
TIM1_SR2:	 equ 0x5256
TIM1_EGR:	 equ 0x5257
TIM1_CCMR1:	 equ 0x5258
TIM1_CCMR2:	 equ 0x5259
TIM1_CCMR3:	 equ 0x525A
TIM1_CCMR4:	 equ 0x525B
TIM1_CCER1:	 equ 0x525C
TIM1_CCER2:	 equ 0x525D
TIM1_CNTRH:	 equ 0x525E
TIM1_CNTRL:	 equ 0x525F
TIM1_PSCRH:	 equ 0x5260
TIM1_PSCRL:	 equ 0x5261
TIM1_ARRH:	 equ 0x5262
TIM1_ARRL:	 equ 0x5263
TIM1_RCR:	 equ 0x5264
TIM1_CCR1H:	 equ 0x5265
TIM1_CCR1L:	 equ 0x5266
TIM1_CCR2H:	 equ 0x5267
TIM1_CCR2L:	 equ 0x5268
TIM1_CCR3H:	 equ 0x5269
TIM1_CCR3L:	 equ 0x526A
TIM1_CCR4H:	 equ 0x526B
TIM1_CCR4L:	 equ 0x526C
TIM1_BKR:	 equ 0x526D
TIM1_DTR:	 equ 0x526E
TIM1_OISR:	 equ 0x526F
TIM2_CR1:	 equ 0x5300
TIM2_IER:	 equ 0x5301
TIM2_SR1:	 equ 0x5302
TIM2_SR2:	 equ 0x5303
TIM2_EGR:	 equ 0x5304
TIM2_CCMR1:	 equ 0x5305
TIM2_CCMR2:	 equ 0x5306
TIM2_CCMR3:	 equ 0x5307
TIM2_CCER1:	 equ 0x5308
TIM2_CCER2:	 equ 0x5309
TIM2_CNTRH:	 equ 0x530A
TIM2_CNTRL:	 equ 0x530B
TIM2_PSCR:	 equ 0x530C
TIM2_ARRH:	 equ 0x530D
TIM2_ARRL:	 equ 0x530E
TIM2_CCR1H:	 equ 0x530F
TIM2_CCR1L:	 equ 0x5310
TIM2_CCR2H:	 equ 0x5311
TIM2_CCR2L:	 equ 0x5312
TIM2_CCR3H:	 equ 0x5313
TIM2_CCR3L:	 equ 0x5314
TIM3_CR1:	 equ 0x5320
TIM3_IER:	 equ 0x5321
TIM3_SR1:	 equ 0x5322
TIM3_SR2:	 equ 0x5323
TIM3_EGR:	 equ 0x5324
TIM3_CCMR1:	 equ 0x5325
TIM3_CCMR2:	 equ 0x5326
TIM3_CCER1:	 equ 0x5327
TIM3_CNTRH:	 equ 0x5328
TIM3_CNTRL:	 equ 0x5329
TIM3_PSCR:	 equ 0x532A
TIM3_ARRH:	 equ 0x532B
TIM3_ARRL:	 equ 0x532C
TIM3_CCR1H:	 equ 0x532D
TIM3_CCR1L:	 equ 0x532E
TIM3_CCR2H:	 equ 0x532F
TIM3_CCR2L:	 equ 0x5330
TIM4_CR1:	 equ 0x5340
TIM4_IER:	 equ 0x5341
TIM4_SR:	 equ 0x5342
TIM4_EGR:	 equ 0x5343
TIM4_CNTR:	 equ 0x5344
TIM4_PSCR:	 equ 0x5345
TIM4_ARR:	 equ 0x5346
ADC_DB0RH:	 equ 0x53E0
ADC_DB0RL:	 equ 0x53E1
ADC_DB1RH:	 equ 0x53E2
ADC_DB1RL:	 equ 0x53E3
ADC_DB2RH:	 equ 0x53E4
ADC_DB2RL:	 equ 0x53E5
ADC_DB3RH:	 equ 0x53E6
ADC_DB3RL:	 equ 0x53E7
ADC_DB4RH:	 equ 0x53E8
ADC_DB4RL:	 equ 0x53E9
ADC_DB5RH:	 equ 0x53EA
ADC_DB5RL:	 equ 0x53EB
ADC_DB6RH:	 equ 0x53EC
ADC_DB6RL:	 equ 0x53ED
ADC_DB7RH:	 equ 0x53EE
ADC_DB7RL:	 equ 0x53EF
ADC_DB8RH:	 equ 0x53F0
ADC_DB8RL:	 equ 0x53F1
ADC_DB9RH:	 equ 0x53F2
ADC_DB9RL:	 equ 0x53F3
ADC_CSR:	 equ 0x5400
ADC_CR1:	 equ 0x5401
ADC_CR2:	 equ 0x5402
ADC_CR3:	 equ 0x5403
ADC_DRH:	 equ 0x5404
ADC_DRL:	 equ 0x5405
ADC_TDRH:	 equ 0x5406
ADC_TDRL:	 equ 0x5407
ADC_HTRH:	 equ 0x5408
ADC_HTRL:	 equ 0x5409
ADC_LTRH:	 equ 0x540A
ADC_LTRL:	 equ 0x540B
ADC_AWSRH:	 equ 0x540C
ADC_AWSRL:	 equ 0x540D
ADC_AWCRH:	 equ 0x540E
ADC_AWCRL:	 equ 0x540F
CFG_GCR:	 equ 0x7F60
ITC_SPR1:	 equ 0x7F70
ITC_SPR2:	 equ 0x7F71
ITC_SPR3:	 equ 0x7F72
ITC_SPR4:	 equ 0x7F73
ITC_SPR5:	 equ 0x7F74
ITC_SPR6:	 equ 0x7F75
ITC_SPR7:	 equ 0x7F76
