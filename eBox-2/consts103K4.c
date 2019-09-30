#include "Consts105c6.h";
#include "Consts.h";

volatile struct Port PE @PE_ODR;
volatile struct Port PG @PG_ODR;
volatile struct Pins PpE @PE_ODR;
volatile struct Pins PpG @PG_ODR;


@near unsigned char bits[portCount] = {
 bit1,//0        PC1             
 bit3,//1           PD3             
 bit4,//2           PD4             
 bit2,//3           PD2             
 bit0,//4           PD0             
 bit7,//5        PC7             
 bit6,//6        PC6             
 bit5,//7        PC5             
 bit5,//8              PE5
 bit4,//9        PC4             
 bit3,//10       PC3             
 bit2,//11       PC2             
 bit0,//12    PB0                          
 bit1,//13    PB1             
 bit2,//14    PB2             
 bit3,//15    PB3             
 bit4,//16    PB4             
 bit1,//17 PA1                          
 bit2,//18 PA2                          
 bit5,//19    PB5              
 bit3,//20 PA3             
 bit4,//21    PB4             
 bit5,//22 PA5             
 bit6 //23    PB6             
};          

@near struct Pins *pins[portCount] = {
  &PpC,//0        PC1      
	&PpD,//1           PD3   
	&PpD,//2           PD4   
	&PpD,//3           PD2   
	&PpD,//4           PD0   
	&PpC,//5        PC7      
	&PpC,//6        PC6      
	&PpC,//7        PC5      
	&PpE,//8              PE5
	&PpC,//9        PC4      
	&PpC,//10       PC3      
	&PpC,//11       PC2      
	&PpB,//12    PB0         
	&PpB,//13    PB1         
	&PpB,//14    PB2         
	&PpB,//15    PB3         
	&PpB,//16    PB4         
	&PpA,//17 PA1            
	&PpA,//18 PA2            
	&PpB,//19    PB5         	
	&PpA,//20 PA3            
	&PpA,//21    PB4         
	&PpA,//22 PA5            
	&PpA //23    PB6         
};        

@near struct Port * ports[portCount] = {
	&PC,//0        PC1      
	&PD,//1           PD3   
	&PD,//2           PD4   
	&PD,//3           PD2   
	&PD,//4           PD0   
	&PC,//5        PC7      
	&PC,//6        PC6      
	&PC,//7        PC5      
	&PE,//8              PE5
	&PC,//9        PC4      
	&PC,//10       PC3      
	&PC,//11       PC2      
	&PB,//12    PB0         
	&PB,//13    PB1         
	&PB,//14    PB2         
	&PB,//15    PB3         
	&PB,//16    PB4         
	&PA,//17 PA1            
	&PA,//18 PA2            
	&PB,//19    PB5         	
	&PA,//20 PA3            
	&PA,//21    PB4         
	&PA,//22 PA5            
	&PA //23    PB6         
};  
