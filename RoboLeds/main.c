#include "Consts103F3.h"

struct PinTask{
	/*
	  TTypes 
		00 - simple;
		01 - inc;
		10 - dec;
		11 - system
	*/	
	unsigned char type : 4;
	unsigned char port : 4;
	unsigned char value;	
	signed int increment;
};

struct SystemTask{	
	unsigned char type : 2;
	unsigned char func : 6;
	unsigned char Param0;	
	unsigned char Param1;
	unsigned char Param2;
};

struct Settings{
	unsigned int DeviceAddr;
	unsigned char Version;	
	unsigned char TimeBase : 2;
	unsigned char BeginMode : 3;
	unsigned char EndMode : 3;
};

union TaskData{
	struct PinTask pinTask;
	struct SystemTask sysTask;
	unsigned long data;
	struct Settings settings;
	unsigned char type : 4;
};

struct PWMdata{
	unsigned char value;
	signed char increment;
	unsigned int nextTick;
};

volatile struct Settings settings @0x4000;
volatile union TaskData EEPROM[taskHigh] @0x4000;

typedef unsigned char (*TaskHandler)(union TaskData);

unsigned char NoTaskHandler(union TaskData task);
unsigned char PinTaskHandler(union TaskData task);
unsigned char GradTaskHandler(union TaskData task);
unsigned char PwmTaskHandler(union TaskData task);
unsigned char BeeperTaskHandler(union TaskData task);
unsigned char SysTaskHandler(union TaskData task);

@near TaskHandler handlers[16] = {
	NoTaskHandler,
	PinTaskHandler,
	GradTaskHandler,
	GradTaskHandler,
	PwmTaskHandler,
	NoTaskHandler,	
	NoTaskHandler,
	NoTaskHandler,
	NoTaskHandler,	
	NoTaskHandler,
	NoTaskHandler,
	NoTaskHandler,
	BeeperTaskHandler,
	SysTaskHandler,
	SysTaskHandler,
	SysTaskHandler
};

void ProcessTask(union TaskData *task){
	if (task->start <= ticks/(255*TimeDiv)){
		handlers[task->type](*task);
	}
}

struct PWMdata PWM[portCount];
unsigned char currentTaskId = 0;
union TaskData currentTask;
unsigned long ticks = 0;
unsigned char ticksByte = 0;
unsigned char ticksDiv = 0;
unsigned char workMode = 0;
unsigned char TimeDiv;

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

void Reset(void){
	WWDG_CR = bit7;
}



void Restart(void){
	unsigned char i;
	TIM4_CR1 &= 254;
	//TIM2_CR1 &= 254;	
  PA_CR2 = PB_CR2 = PC_CR2 = PD_CR2 = 0;
	PA_ODR = PB_ODR = PC_ODR = PD_ODR = 0;
	PA_DDR = PB_DDR = PC_DDR = PD_DDR = 0;
	PA_CR1 = PB_CR1 = PC_CR1 = PD_CR1 = 0;
			
	for (i = 0; i< portCount; i++){
		PWM[i].value = 0;
		PWM[i].increment = 0;
		PWM[i].nextTick = 0;
	}
		
	ticks = 0;
	ticksByte = 0;
	ticksDiv = 0;
	
	currentTaskId = 1;
	currentTask = EEPROM[currentTaskId];
	switch (settings.TimeBase){
		case 0: timeDiv = 100; break;
		case 1: timeDiv = 10; break;
		case 2: timeDiv = 1; break;
		case 3: timeDiv = 1; break;
		default: timeDiv = 100;
	}
	if (workMode > 0){
		//TIM2_CR1 |= 1;
		TIM4_CR1 |= 1;
	}	
}

void Init(void){
	unsigned char i;
	CLK_CKDIVR = 0; //SET 16 MHZ
	// 2MHz
	
	PA_DDR = PB_DDR = PC_DDR = PD_DDR = 0;
	PA_ODR = PB_ODR = PC_ODR = PD_ODR = 0;
	PA_CR1 = PB_CR1 = PC_CR1 = PD_CR1 = 0;
  PA_CR2 = PB_CR2 = PC_CR2 = PD_CR2 = 0;
	
	SysR1;
	TIM2_CR1 = bit7;    //Разрешаем буферизацию ARR
	TIM2_ARRH = 0;      //1000000 / 100 ~ 10000Hz;
	TIM2_ARRL = 50;    //1000000 / 100 ~ 10000Hz;
	TIM2_IER =  1;      //Update overflow and Capture/Compare 1
	TIM2_PSCR = 4;      //Предделитель 16000000 /16 = 1000000 Hz
	//TIM2_CR1 |= 1;      //En
		
	TIM4_CR1 = bit7;    //Разрешаем буферизацию ARR
	TIM4_ARR = 78;     // 2000000 /78 = 25500 Hz;
	//TIM4_ARR = 125;     // 125000 / 125 = 1000;
	TIM4_IER =  1;       //Update overflow and Capture/Compare 1
	//TIM4_PSCR = 7;       //Предделитель 16000000 / 128 = 125000 Hz
	TIM4_PSCR = 3;       //Предделитель 16000000 / 8 = 2000000 Hz
	//TIM4_CR1 |= 1;
	
	UART_CR1 = bit2 + bit4; //PARITY ODD + 9Bit (8d+1p)
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
	UART_CR1 |= bit1; //EN
	
	//UART_BRR2 = 0x00; //9600 2 MHz
	//UART_BRR1 = 0x0D; //9600 2 MHz
	//UART_CR1 |= 1;
			
	//TIM2_CR1 |= 1;
	workMode = settings.BeginMode;
	switch (settings.TimeBase){
		case 0: timeDiv = 100; break;
		case 1: timeDiv = 10; break;
		case 2: timeDiv = 1; break;
		case 3: timeDiv = 1; break;
		default: timeDiv = 100;
	}
	if (workMode  > 0){
		currentTaskId = 1;
		currentTask = EEPROM[currentTaskId];
		TIM4_CR1 |= 1;
	}
	else{
		
	}
	#asm
	rim
	#endasm
}

unsigned char PinTaskHandler(union TaskData data){
	struct PinTask task = data.pinTask;
	struct Port *port;
	unsigned char bitNum;
	port = ports[task.port];
	bitNum = bits[task.port];
	port->DDR |= bitNum; //DDR
	port->CR1 |= bitNum; //СR1
	port->CR2 |= bitNum; //СR2
	PWM[task.port].value = task.value;
	if (task.value > 0){
		port->Out |= bitNum;
	}
	else{		
		port->Out &= ~bitNum;
	}
}

unsigned char GradTaskHandler(union TaskData data){
	struct PinTask task = data.pinTask;
	PWM[task.port].increment = task.type == 2 ? task.increment : -task.increment;
	PWM[task.port].nextTick = ticks + abs(task.increment);
	return PinTaskHandler(data);	
}

unsigned char PwmTaskHandler(union TaskData data){

}

unsigned char BeeperTaskHandler(union TaskData data){
	BEEP_CSR = data.sysTask.Param0;
}

unsigned char NoTaskHandler(union TaskData data){

}

unsigned char SysTaskHandler(union TaskData data){

}


void uartSend(void);

main()
{
	unsigned int pwmCursor = 0;
	struct PWMdata* pwm;
	unsigned int overflows;
	signed short value;
	char dataState;
	Init();
	overflows = 0;
	while (1){
		if (workMode > 0){
			if (PWM[pwmCursor].increment != 0){
				pwm = &PWM[pwmCursor];
				if (pwm->nextTick <= ticks){
					if (abs(pwm->increment) <= 10){
						value = (signed short)pwm->value + sign(pwm->increment)*(11 - abs(pwm->increment));
						pwm->value = value;
						if (value >= 0 && value <= 255){
							pwm->value = value;
						}
						else{
							if (value > 255){
								pwm->increment = 0;
								pwm->value = 255;
							}
							if (value < 0){
								pwm->increment = 0;
								pwm->value = 0;
							}
						}
						pwm->nextTick = ticks + 1;
					}
					else{
						pwm->nextTick = ticks + abs(pwm->increment - 10);
						pwm->value += sign(pwm->increment);
					}
					if (pwm->value == 255 || pwm->value == 0){
						pwm->increment = 0;
						pwm->nextTick = 0;
						if (pwm->value == 255){
							ports[pwmCursor]->Out |= bits[pwmCursor];
						}
						if (pwm->value == 0){
							ports[pwmCursor]->Out &= ~bits[pwmCursor];
						}
					}
				}
			}
			pwmCursor++;
			if (pwmCursor == portCount){
				pwmCursor = 0;
				overflows++;
			}
		}
		if (urState.phase == 3){
			if (uartDataBuf.command == 1){ //get
				uartDataBuf.data = EEPROM[uartDataBuf.address];
			}
			if (uartDataBuf.command == 2){ //set
				dataState = FLASH_IAPSR;
				dataState &= bit3;
				if (dataState == 0){
					FLASH_DUKR = 0xAE;
					FLASH_DUKR = 0x56;
				}
				if (uartDataBuf.address == 0){
					settings = uartDataBuf.data.settings;
				}
				else{
					EEPROM[uartDataBuf.address] = uartDataBuf.data;
					ProcessTask(&uartDataBuf.data);
				}				
			}
			if (uartDataBuf.command == 6){ //set task dirty
				workMode = 1;
				ProcessTask(&uartDataBuf.data);
			}
			if (uartDataBuf.command == 7){ //Restart
				workMode = 1;
				Restart();
			}
			if (uartDataBuf.command == 9){ //Stop
				workMode = 0;
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

@interrupt void iTim2_overflow(void){
	//_asm("SIM");
	TIM2_SR1 &= 254;

	//_asm("RIM");
}

@interrupt void iTim2_compare(void){
	//_asm("SIM");
	TIM2_SR1 &= 254;
	
	//_asm("RIM");
}

@interrupt void iTim4_OVF(void){
	unsigned char i;
	//_asm("SIM");
	TIM4_SR &= 254;
	ticksByte++;		
	if (ticksByte == 255){
		ticksByte = 0;
		ticks++;
		if (workMode > 0){
			for (i = 0; i < portCount; i++){
				if (PWM[i].value > 0 && PWM[i].value < 255){
					ports[i]->Out |= bits[i];
				}
			}
		}
	}
	if (workMode > 0){
		for (i = 0; i < portCount; i++){
			if (PWM[i].value > 0 && PWM[i].value < 255){
				if (ticksByte == PWM[i].value){
					ports[i]->Out &= ~bits[i];
				}
			}
		}
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