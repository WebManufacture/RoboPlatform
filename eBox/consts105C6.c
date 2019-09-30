#include "Consts105c6.h";
#include "Consts.h";

volatile struct Port PE @PE_ODR;
volatile struct Port PG @PG_ODR;
volatile struct Pins PpE @PE_ODR;
volatile struct Pins PpG @PG_ODR;

@near unsigned char bits[portCount] = {
 bit7,//1    PA4              
 bit6,//2    PA5     
 bit4,//3              PE0      
 bit5,//4    PA3               
 bit2,//5    			PD4              
 bit3,//6     			PD2        
 bit0,//7     			PD3       
 bit1,//8     			PD0         
 
 bit1,//0        PC1             
 bit2,//1        PC2             
 bit3,//2        PC3
 bit4,//3        PC4             
 bit5,//4        PC5             
 bit6,//5        PC6             
 bit7,//6        PC7       

 bit5,
 bit4,//15    PA4                               
 bit5,//16    PA5            
 bit6 //17    PA6 
};          

@near struct Pins *pins[portCount] = {
     
	&PpB,//7     			PD0    
	&PpB,//8     			PD2
	&PpB,//9     			PD3    
	&PpB,//10    			PD4    
	&PpB,//11              PE0
	&PpB,//12    PA3          
	&PpB,//13    PA4          
	&PpB,//14    PA5          
	
	&PpC,//0        PC1                      
	&PpC,//1        PC2       
	&PpC,//2        PC3
	&PpC,//3        PC4       
	&PpC,//4        PC5       
	&PpC,//5        PC6       
	&PpC,//6        PC7  
	
	&PpE, //18
	&PpA,//15    PA6                     
	&PpA,//16    PB0          
	&PpA//17
};       
@near struct Port * ports[portCount] = {
  
	&PB,//7     			PD0    
	&PB,//8     			PD2
	&PB,//9     			PD3    
	&PB,//10    			PD4    
	&PB,//11              PE0
	&PB,//12    PA3          
	&PB,//13    PA4          
	&PB,//14    PA5  
       
	&PC,//0        PC1       
	&PC,//1        PC2       
	&PC,//2        PC3
	&PC,//3        PC4       
	&PC,//4        PC5       
	&PC,//5        PC6       
	&PC,//6        PC7     
			 
	&PE,	
	&PA,//15    PA6         
	&PA,//16    PB0          
	&PA//17    PB1   
};   
