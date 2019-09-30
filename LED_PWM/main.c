/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "STM8S103F.h"

#define TIM_CR_bit_CEN 0
#define TIM_CR_bit_ARPE 7
#define TIM_CCER1_bit_CCE2 4
#define TIM_CCER1_bit_CCP2 5
#define TIM_CCER1_bit_CCE1 0
#define TIM_CCER1_bit_CCP1 1
#define TIM_CCER2_bit_CCE3 0
#define TIM_CCER2_bit_CCP3 1
#define TIM_CCMR_bit_OCPE 3
#define TIM_CCMR_bit_OCM4 6
#define TIM_CCMR_bit_OCM2 5
#define TIM_CCMR_bit_OCM1 4
#define TIM4_CR1_bit_OPM 3	
#define PD_CR1_bit_C10 0
#define PD_CR2_bit_C20 0

int counter = 0;
unsigned char mask1[5];
unsigned char mask2[5];

void Init(void){
	PC_DDR = 255;
	PC_CR1 = 255;
	PC_CR2 = 0;
	
	mask1[0] = 0b11111000;
	mask2[0] = 0b11111000;
	mask1[1] = 0b11111000;
	mask2[1] = 0b11111000;
	mask1[2] = 0b11111000;
	mask2[2] = 0b11111000;
	mask1[3] = 0b11111000;
	mask2[3] = 0b11111000;
	mask1[4] = 0b11111000;
	mask2[4] = 0b11111000;
	mask1[5] = 0b11111000;
	mask2[5] = 0b11111000;
	
	//TIM2_CR1 |= 0x80;      //Разрешаем буферизацию ARR
	//TIM2_IER =  1;                   //Update overflow and Capture/Compare 1
	//TIM2_PSCR = 7;       //Предделитель 16000000 / 1 = 16000000 Hz
	
	#asm
	rim
	#endasm
}

main()
{
	int i = 0;
	int k = 0;
	int high = 1000;
	Init();
	PC_ODR = 255;
	while(1){
		 for (i = 0; i < high; i++){
				PC_ODR = 8;
				for (k = high; k > i; k--);
				PC_ODR = 16;
				for (k = 0; k < i; k++);
		 }
		 for (i = 0; i < high; i++){
				PC_ODR = 16;
				for (k = high; k > i; k--);
				PC_ODR = 32;
				for (k = 0; k < i; k++);
		 }
		 for (i = 0; i < high; i++){
				PC_ODR = 32;
				for (k = high; k > i; k--);
				PC_ODR = 64;
				for (k = 0; k < i; k++);
		 }
		 for (i = 0; i < high; i++){
				PC_ODR = 64;
				for (k = high; k > i; k--);
				PC_ODR = 128;
				for (k = 0; k < i; k++);
		 }
		 for (i = 0; i < high; i++){
				PC_ODR = 128;
				for (k = high; k > i; k--);
				PC_ODR = 8;
				for (k = 0; k < i; k++);
		 }
  };
}

@interrupt void iTim2_overflow(void){
	_asm("SIM");
	TIM2_SR1 &= 254;
	PC_ODR |= mask1[0];
	_asm("RIM");
}

@interrupt void iTim2_compare(void){
	_asm("SIM");
	TIM2_SR1 &= 254;
	
	_asm("RIM");
}
