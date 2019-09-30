#include "DeviceConfig.h"
#include "Modules.h"

volatile struct Settings sysSettings @0x4000;
volatile union TaskData EEPROM[taskHigh] @0x4000;
struct WorkSettings settings;

unsigned char currentTaskId = 0;
union TaskData currentTask;
unsigned long ticks = 0;

struct UartData{
	unsigned char command;
	unsigned int address;
	union TaskData data;
} uartDataBuf;

struct uartState{
	unsigned char phase;
	unsigned char size;
	unsigned char index;
} urState, uwState;


unsigned char NoTaskHandler(union TaskData task, long ticks);

@near TaskHandler handlers[16] = {
	NoTaskHandler,   // 0
	NoTaskHandler,  // 1
	NoTaskHandler, // 2 Gradient Up
	NoTaskHandler, // 3 Gradient Down
	NoTaskHandler,  // 4
	NoTaskHandler,  // 5
	NoTaskHandler, // 6 Gradient Up
	NoTaskHandler, // 7 Gradient Down
	NoTaskHandler,  // 8
	NoTaskHandler,   // 9
	NoTaskHandler,   // 10
	NoTaskHandler,   // 11
	NoTaskHandler, //12
	NoTaskHandler,  //13
	NoTaskHandler,  //14
	NoTaskHandler   //15
};


void RegisterTaskHandler(unsigned char index, TaskHandler handler){
	handlers[index] = handler;
}

unsigned char ProcessTask(union TaskData *task){
	TaskHandler handler;
	if (task->sysTask.start  <= ticks/settings.TimeDiv){
		handler = handlers[task->type];
		return handler(*task, ticks);
	}
	return 0;
}

unsigned char NoTaskHandler(union TaskData data, long ticks){
	return 1;
}

void Reset(void){
	WWDG_CR = bit7;
}

void Restart(void){
	unsigned char i;
	TIM4_CR1 &= 254;
	//TIM2_CR1 &= 254;	
	
	ticks = 0;
	currentTaskId = 1;
	currentTask = EEPROM[currentTaskId];
	settings.TimeBase = sysSettings.TimeBase;
	
	switch (settings.TimeBase){
		case 0: settings.TimeDiv = 1000; break;
		case 1: settings.TimeDiv = 100; break;
		case 2: settings.TimeDiv = 10; break;
		case 3: settings.TimeDiv = 1; break;
		default: settings.TimeDiv = 1000;
	}
}

void Init(void){
	unsigned char i;
	CLK_CKDIVR = 0; //SET 16 MHZ
	// 2MHz
				
	TIM2_CR1 = bit7;    //Разрешаем буферизацию ARR
	TIM2_ARRH = 0;      //1000000 / 100 ~ 10000Hz;
	TIM2_ARRL = 50;    //1000000 / 100 ~ 10000Hz;
	TIM2_IER =  1;      //Update overflow and Capture/Compare 1
	TIM2_PSCR = 4;      //Предделитель 16000000 /16 = 1000000 Hz
	//TIM2_CR1 |= 1;      //En
		
	TIM4_CR1 = bit7;    //Разрешаем буферизацию ARR
	//TIM4_ARR = 78;     // 2000000 /78 = 25500 Hz;
	TIM4_ARR = 125;     // 125000 / 125 = 1000;
	TIM4_IER =  1;       //Update overflow and Capture/Compare 1
	TIM4_PSCR = 7;       //Предделитель 16000000 / 128 = 125000 Hz
	//TIM4_PSCR = 3;       //Предделитель 16000000 / 8 = 2000000 Hz
	//TIM4_CR1 |= 1;
	
	UART_CR1 = 0; //PARITY ODD + 9Bit (8d+1p)
	UART_CR2 = bit5 + bit3 + bit2; // RX + TX + RXIE
	UART_CR3 = 0; //1 STOP
	UART_CR4 = 0;
	UART_CR5 = 0;
	//UART_BRR2 = 0x03; //9600 16 MHz
	//UART_BRR1 = 0x68; //9600 16 MHz
	
	UART_BRR2 = 0x0B; //115200 16 MHz
	UART_BRR1 = 0x08; //115200 16 MHz
	
	//UART_BRR2 = 0x0D; //128000 16 MHz
	//UART_BRR1 = 0x07; //128000 16 MHz
	//UART_CR1 |= bit1; //EN
	
	//UART_BRR2 = 0x00; //9600 2 MHz
	//UART_BRR1 = 0x0D; //9600 2 MHz
	//UART_CR1 |= 1;
			
	//TIM2_CR1 |= 1;
	
		
	ticks = 0;
	currentTaskId = 1;
	currentTask = EEPROM[currentTaskId];
	
	settings.DeviceAddr = sysSettings.DeviceAddr;
	settings.Version = sysSettings.Version;
	settings.WorkMode = sysSettings.BeginMode;
	settings.BeginMode = sysSettings.BeginMode;
	settings.EndMode = sysSettings.EndMode;
	settings.TimeBase = sysSettings.TimeBase;
	
	switch (settings.TimeBase){
		case 0: settings.TimeDiv = 1000; break;
		case 1: settings.TimeDiv = 100; break;
		case 2: settings.TimeDiv = 10; break;
		case 3: settings.TimeDiv = 1; break;
		default: settings.TimeBase = 1000;
	}
		
	TIM4_CR1 |= 1;
	
	#asm
	rim
	#endasm
}

void Stop(void){
	TIM4_CR1 &= 254;
	//TIM2_CR1 &= 254;	
	
	settings.WorkMode = 0;
}


void uartSend(void);

main()
{
	char dataState;
	Init();
	while (1){
		if (settings.WorkMode > 0){
			if (settings.WorkMode == 2){
				if (currentTaskId >= taskHigh || currentTask.data == 0){
					 if (settings.EndMode == 3){
						 Reset();
					 }
					 settings.WorkMode = settings.EndMode;
					 if (settings.WorkMode == 2 || settings.WorkMode == 0){
						 Restart();
						 continue;
					 }
				}
				if (ProcessTask(&currentTask)){
					currentTaskId++;
					currentTask = EEPROM[currentTaskId];
				}
			}
		}
		if (urState.phase == 3){
			if (uartDataBuf.command == 1){ //get
				//Stop();
				uartDataBuf.data = EEPROM[uartDataBuf.address];
			}
			if (uartDataBuf.command == 2){ //set
				Stop();
				dataState = FLASH_IAPSR;
				dataState &= bit3;
				if (dataState == 0){
					FLASH_DUKR = 0xAE;
					FLASH_DUKR = 0x56;
				}
				EEPROM[uartDataBuf.address] = uartDataBuf.data;
			}
			if (uartDataBuf.command == 5){ //set task dirty
				ProcessTask(&uartDataBuf.data);
			}			
			if (uartDataBuf.command == 6){ //Pause
				TIM4_CR1 &= 254;
				settings.WorkMode = 1;
			}			
			if (uartDataBuf.command == 7){ //Resume
				TIM4_CR1 |= 1;
				settings.WorkMode = 2;
			}
			if (uartDataBuf.command == 8){ //Restart
				settings.WorkMode = 2;
				Restart();
			}
			if (uartDataBuf.command == 9){ //Stop
				settings.WorkMode = 0;
				Restart();
			}
			if (uartDataBuf.command == 10){ //Reset
				Reset();
			}
			uartSend();		
			urState.phase = 0;
		}
	}
}


@interrupt void iTim4_OVF(void){
	unsigned char i;
	//_asm("SIM");
	TIM4_SR &= 254;
	if (settings.WorkMode >= 2){
		ticks++;
	}
	//_asm("RIM");
}

@interrupt void iUART_RX(void){
	unsigned char val;
	unsigned int index;
	_asm("SIM");
	res(UART_SR,5);
	val = UART_DR;
	if (urState.phase < 3){
		if (urState.phase == 2){
			if (urState.index >= urState.size){
					urState.phase = val == 04 ? 3 : 0;
			}
			else{
				*((char*)&uartDataBuf + urState.index) = val;
				urState.index++;
			}
		}
		if (urState.phase == 1){
			if (val > 0 && val <= sizeof(struct UartData)){
				urState.phase = 2;
				urState.size = val;				
			}
			else{
				urState.phase = 0;
				urState.size = 0;
			}
			urState.index = 0;
		}
		if (urState.phase == 0){
			if (val == 1 || val == 2){
				urState.phase = val;
				urState.size = sizeof(struct UartData);
			}
			urState.index = 0;
		}
	}
	_asm("RIM");
}

void uartSend(void){
	unsigned char size;
	uwState.phase = 0;
	uwState.index = 0;
	size = sizeof(struct UartData);
	uwState.size = size;
	UART_DR = 01;//SOH
	UART_CR2 |= bit7;//(TIEN) TXE interrupt	
}

@interrupt void iUART_TX(void){
	unsigned char *val;
	unsigned char wv;
	_asm("SIM");
	if (uwState.index >= uwState.size){
		if (uwState.phase < 3){
			UART_DR = 04;//EOT
			uwState.phase = 3;
		}
		else{
			UART_CR2 = UART_CR2 & ~bit7; //UART TX OFF
		}		
	}
	else{
		val = ((char*)&uartDataBuf) + uwState.index;
		if (uwState.phase == 2){
			wv = *val & 0b00001111;
			UART_DR = wv + 0xF0;
			uwState.index++;
			uwState.phase = 1;
		}
		else{
			if (uwState.phase == 1){
				wv = *val >> 4;
				UART_DR = wv + 0xF0;
				uwState.phase = 2;
			}
			else{
				if (uwState.phase == 0){
					UART_DR = uwState.size*2;
					uwState.phase = 1;
				}
			}
		}
	}
	_asm("RIM");
}


@interrupt void iI2C_handler(void)
{
	unsigned char data;
  if ((I2C->SR2) != 0)
  {
		I2C->SR2 = 0;
  }
  Event = I2C_GetLastEvent();
  switch (Event)
  {
      /******* Slave transmitter ******/
      /* check on EV1 */
    case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:
      Tx_Idx = 0;
      break;

      /* check on EV3 */
    case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:
      /* Transmit data */
      I2C_SendData(Slave_Buffer_Tx[Tx_Idx++]);
      break;
      /******* Slave receiver **********/
      /* check on EV1*/
    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
			//ShowDot(5);
			Rx_Idx = 0;
      break;

      /* Check on EV2*/
    case I2C_EVENT_SLAVE_BYTE_RECEIVED:
			data = I2C_ReceiveData();
			Slave_Buffer_Rx[Rx_Idx++] = data;
      break;

      /* Check on EV4 */
    case (I2C_EVENT_SLAVE_STOP_DETECTED):
        /* write to CR2 to clear STOPF flag */
        I2C->CR2 |= I2C_CR2_ACK;				
				ProcessCommand(Rx_Idx);
				//HideDot(5);
      break;

    default:
      break;
  }
}