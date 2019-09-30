#include "Consts105k4-Карась.h";
#include "Consts.h";

#ifndef _Hardware_Ext
#define _Hardware_Ext 1
volatile struct Port PE @PE_ODR;
volatile struct Port PF @PF_ODR;
volatile struct Pins PpE @PE_ODR;
volatile struct Pins PpF @PF_ODR;

@near unsigned char bits[27] = {
 bit4,//0      PF4
 bit5,//1      PD5
 bit6,//2      PD6
 bit1,//3      PC1
 bit3,//4      PD3
 bit4,//5      PD4
 bit2,//6      PD2
 bit0,//7      PD0
 bit7,//8      PC7
 bit6,//9      PC6
 bit5,//10     PC5
 bit5,//11     PE5
 bit4,//12     PC4
 bit3,//13     PC3
 bit2,//14     PC2
 bit0,//15     PB0
 bit1,//16     PB1
 bit2,//17     PB2
 bit3,//18     PB3
 bit4,//19     PB4
 bit1,//20     PA1
 bit2,//21     PA2
 bit5, //22     PB5
 
 bit3,//23     PA3
 bit4, //24     PB4
 bit5,//25     PA5
 bit6 //26     PB6
};          

@near struct Pins *pins[27] = {
	&PpF,//0    PF4
	&PpD,//1    PD5
	&PpD,//2    PD6
  &PpC,//3    PC1
	&PpD,//4    PD3
	&PpD,//5    PD4
	&PpD,//6    PD2
	&PpD,//7    PD0
	&PpC,//8    PC7
	&PpC,//9    PC6
	&PpC,//10   PC5
	&PpE,//11   PE5
	&PpC,//12   PC4
	&PpC,//13   PC3
	&PpC,//14   PC2
	&PpB,//15   PB0
	&PpB,//16   PB1
	&PpB,//17   PB2
	&PpB,//18   PB3
	&PpB,//19   PB4
	&PpA,//20   PA1
	&PpA,//21   PA2
	&PpB,//22   PB5
	
	&PpA,//23   PA3
	&PpA,//24   PA4
	&PpA,//25   PA5
	&PpA//26   PA6
};        

@near struct Port * ports[27] = {
	&PF,//0    PF4
	&PD,//1    PD5
	&PD,//2    PD6
  &PC,//3    PC1
	&PD,//4    PD3
	&PD,//5    PD4
	&PD,//6    PD2
	&PD,//7    PD0
	&PC,//8    PC7
	&PC,//9    PC6
	&PC,//10   PC5
	&PE,//11   PE5
	&PC,//12   PC4
	&PC,//13   PC3
	&PC,//14   PC2
	&PB,//15   PB0
	&PB,//16   PB1
	&PB,//17   PB2
	&PB,//18   PB3
	&PB,//19   PB4
	&PA,//20   PA1
	&PA,//21   PA2
	&PB,//22   PB5
	
	&PA,//23   PA3
	&PA,//24   PA4
	&PA,//25   PA5
	&PA//26   PA6
};                 
#endif