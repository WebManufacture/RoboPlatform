#include "utils.h"
#include "deviceConfig.h"
#include "generalConfig.h"
#include "events.h"
#include "routing.h"
#include "uart.h"
#include "tinyDebug.h"

//EVENT LOOP

struct UartBuffers{
	char read[670];
	char write[32];
};

@near struct UartBuffers buffers;

char* readDataBuf = (char*)&buffers.read;
char* writeDataBuf = (char*)&buffers.write;

extern @far void* real_program_srart;

main()
{	
	struct DeviceState deviceState;
	
	//config = LoadConfiguration();	

  //Using Internal clock
  //CLK_CKDIVR = DeviceConfig.deviceSettings.ClockDivider;
	
  PROGRAM_PIN_DDR = 0;
  
  if (PROGRAM_PIN_IN == 0){
    //goto real_program_srart; 
    _asm("JPF $8500");
  } else {
    InitUART(); //115200
    InitDebug();
    UnblockEEPROM();
    
    readDataBuf[0] = 0;
    readDataBuf[1] = 0;
    readDataBuf[2] = 0;
    readDataBuf[3] = 0;
    
    PROGRAM_PIN_DDR = 1;
    
    _asm("RIM");
    
    PROGRAM_PIN_OUT = 1;
    
    while (!deviceState.ResetFlag){
      //iteration++;
      
      if (CheckUart()){
        if (ProcessPacket(readDataBuf)){
        
        };
      }
    }
    Reset();
  }
}