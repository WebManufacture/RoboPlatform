#include "STM8S103F3P.h"
#include "Consts.h"

#define taskHigh 80
#define portCount 14

#define UART_CR1 UART1_CR1
#define UART_CR2 UART1_CR2
#define UART_CR3 UART1_CR3
#define UART_CR4 UART1_CR4
#define UART_CR5 UART1_CR5
#define UART_BRR2 UART1_BRR2
#define UART_BRR1 UART1_BRR1
#define UART_SR UART1_SR
#define UART_DR UART1_DR

#define SysR1 PA.Out |= bit2
#define SysR0 PA.Out &= ~bit2
#define SysG1 PA.Out |= bit3
#define SysG0 PA.Out &= ~bit3
#define SysB1 PA.Out |= bit1
#define SysB0 PA.Out &= ~bit1
//#define SysB1 PD.Out |= bit1
//#define SysB0 PD.Out &= ~bit1
       
@near unsigned char bits[portCount] = {
 bit1,//0     PD1
 bit1,//1     PA1
 bit2,//2     PA2
 bit3,//3     PA3
 bit4,//4     PB4
 bit5,//5     PB5
 bit3,//6     PC3
 bit4,//7     PC4
 bit5,//8     PC5
 bit6,//9     PC6
 bit7,//10    PC7
 bit2,//11    PD2
 bit3,//12    PD3
 bit4//13    PD4
 //bit5,//14    PD5 UART!!!
 //bit6 //15    PD6 UART!!!
};            

@near struct Pins *pins[portCount] = {
	&PpD,//0    PD1
	&PpA,//1    PA1
	&PpA,//2    PA2
  &PpA,//3    PA3
	&PpB,//4    PB4
	&PpB,//5    PB5
	&PpC,//6    PC3
	&PpC,//7    PC4
	&PpC,//8    PC5
	&PpC,//9    PC6
	&PpC,//10   PC7
	&PpD,//11   PD2
	&PpD,//12   PD3
	&PpD//13   PD4
	//&PpD,//14   PD5
	//&PpD //15   PD6
};        

@near struct Port * ports[portCount] = {
	&PD,//0    PD1
	&PA,//1    PA1
	&PA,//2    PA2
  &PA,//3    PA3
	&PB,//4    PB4
	&PB,//5    PB5
	&PC,//6    PC3
	&PC,//7    PC4
	&PC,//8    PC5
	&PC,//9    PC6
	&PC,//10   PC7
	&PD,//11   PD2
	&PD,//12   PD3
	&PD //13   PD4
	//&PD,//14   PD5
	//&PD //15   PD6
};                 
         