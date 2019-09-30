#include "utils.h"
#include "deviceConfig.h"
#include "generalConfig.h"
#include "events.h"
#include "uart.h"

//ДЛЯ РЕГИСТРАЦИИ InitUARTHandler нужна ф-я
//НУЖЕН МОДУЛЬ DEBUG!

//EVENT LOOP

main()
{	
	unsigned long iteration = 0;
	struct CONFIGURATION_STRUCT* config = LoadConfiguration();	
	FireEvent(EVT_INIT, (char*)config);
	while (!deviceState.ResetFlag){
		iteration++;
		//ProcessMessages(iteration);
		FireEvent(EVT_IDLE, iteration);
	}
	FireEvent(EVT_RESET, NULL);
	Reset();
	
	/*
	unsigned long iteration = 0;
	struct CONFIGURATION_STRUCT* config;
	
	#ifdef DEBUG_MODULE
	
	UnblockEEPROM();
	DeviceConfig.events[0].eventNum = 1;
	DeviceConfig.events[0].handler = __evt_init_routing;
	DeviceConfig.events[1].eventNum = 1;
	DeviceConfig.events[1].handler = __evt_init_uart;
	DeviceConfig.events[2].eventNum = 1;
	DeviceConfig.events[2].handler = __evt_init_debug;
		
	#endif
	
	config = LoadConfiguration();	
	if (DeviceConfig.deviceSettings.ClockSource) {
		CLK_ECKR = 0;//HSEEN Разрешаем работу генератора с внешним кварцем (HSEEN)
		CLK_SWCR = 1;//SWENРазрешаем переключение генераторов;
		CLK_SWR = 0xB4 ;//Выбираем clock от кварцевого генератора (HSE)
		CLK_CKDIVR = DeviceConfig.deviceSettings.ClockDivider;
	}	
	else{
		CLK_CKDIVR = DeviceConfig.deviceSettings.ClockDivider;
	}	
	FireEvent(EVT_INIT, (char*)config);
	_asm("RIM");
	FireEvent(EVT_AFTER_INIT, (char*)config);
	while (!deviceState.ResetFlag){
		//iteration++;
		if (deviceState.DebugFlag) {
			FireEvent(EVT_DEBUG, NULL);
		}
		else{
			FireEvent(EVT_IDLE, NULL);
		}
	}
	FireEvent(EVT_RESET, NULL);
	Reset();*/
}