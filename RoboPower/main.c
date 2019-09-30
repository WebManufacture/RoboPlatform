#include "utils.h"
#include "deviceConfig.h"
#include "uart.h"

//EVENT LOOP

uchar mode = 0;
char packet[4];
 
void UartSendPacket(void){
	UartSendMessage(00, 12, 00, 12, (char*)&packet, 4);
}

void preInit(void){	
	CLK_CKDIVR = 0;
}

unsigned int readADC(char channel){
	unsigned int tmp;
	unsigned char hi;
	unsigned char lo;
	ADC_CSR = channel;// 0..5 CH Selection
	ADC_CR2 = 0; //+bit1 // Scan mode
	//ADC_CR3 = 0;// Scan mode
	ADC_CR1 = 1;// Start Conversion				
	while ((ADC_CSR & 128) == 0);	
	hi = ADC_DRH;
	lo = ADC_DRL;
	tmp = (hi << 2) + (lo);
	return tmp;
}
	
void main(){
	unsigned int voltage, current;	
	unsigned int voltage2, current2;
	int d = 0;		
	int e = 0;		
	preInit();

  _asm("RIM");
	InitUART_simple(0x0B08);
	
	ADC_CR1 = 1;// ADC 
	while (mode != 0xFF){
		d++;
		if (d < 30){
			voltage += readADC(4);
			continue;
		}
		voltage /= 30;
		d = 0;
		packet[0] = ((voltage*255)/1024);
		Delay(100000);
		UartSendData((char*)&packet, 1);
	}
}