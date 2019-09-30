#include "Consts105c6.h";
#include "Consts.h";

volatile struct Port PE @PE_ODR;
volatile struct Port PG @PG_ODR;
volatile struct Pins PpE @PE_ODR;
volatile struct Pins PpG @PG_ODR;

@near unsigned char bits[portCount] = {
 bit1,//0        PC1             
 bit2,//1        PC2             
 bit3,//2        PC3
 bit4,//3        PC4             
 bit5,//4        PC5             
 bit6,//5        PC6             
 bit7,//6        PC7             
 bit0,//7     			PD0             
 bit2,//8     			PD2
 bit3,//9     			PD3             
 bit4,//10    			PD4             
 bit0,//11              PE0             
 bit3,//12    PA3               
 bit4,//13    PA4             
 bit5,//14    PA5            
 bit6,//15    PA6   
                             
 bit0,//16    PB0            
 bit1,//17    PB1                         
 bit2,//18    PB2                         
 bit3,//19    PB3                         
 bit4,//20    PB4                                     
 bit5,//21    PB5                         
 bit6,//22    PB6                        
 bit7 //23    PB7               
};          

@near struct Pins *pins[portCount] = {
  &PpC,//0        PC1                      
	&PpC,//1        PC2       
	&PpC,//2        PC3
	&PpC,//3        PC4       
	&PpC,//4        PC5       
	&PpC,//5        PC6       
	&PpC,//6        PC7       
	&PpD,//7     			PD0    
	&PpD,//8     			PD2
	&PpD,//9     			PD3    
	&PpD,//10    			PD4    
	&PpE,//11              PE0
	&PpA,//12    PA3          
	&PpA,//13    PA4          
	&PpA,//14    PA5          
	&PpA,//15    PA6   
                  
	&PpB,//16    PB0          
	&PpB,//17    PB1          
	&PpB,//18    PB2          	
	&PpB,//19    PB3          
	&PpB,//20    PB4          
	&PpB,//21    PB5          
	&PpB,//22    PB6          
	&PpB //23    PB7             
};       
@near struct Port * ports[portCount] = {
	&PC,//0        PC1       
	&PC,//1        PC2       
	&PC,//2        PC3
	&PC,//3        PC4       
	&PC,//4        PC5       
	&PC,//5        PC6       
	&PC,//6        PC7       
	&PD,//7     			PD0    
	&PD,//8     			PD2
	&PD,//9     			PD3    
	&PD,//10    			PD4    
	&PE,//11              PE0
	&PA,//12    PA3          
	&PA,//13    PA4          
	&PA,//14    PA5          
	&PA,//15    PA6   
           
	&PB,//16    PB0          
	&PB,//17    PB1          
	&PB,//18    PB2          	
	&PB,//19    PB3          
	&PB,//20    PB4          
	&PB,//21    PB5          
	&PB,//22    PB6          
	&PB //23    PB7                       
};   
