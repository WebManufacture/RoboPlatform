#include "Consts.h"
#include "Utils.h"
#include "DeviceConfig.h"

volatile struct Settings settings @0x4000;
unsigned long ticks = 0;
@near struct PWMdata PWM[portCount];
unsigned char ticksByte;

void Reset(void){
	WWDG_CR = bit7;
}

void ClearPWM(char index){
	PWM[index].value = 0;
	PWM[index].lowValue = 0;
	PWM[index].highValue = 0;
	PWM[index].currentPos = 2;
	PWM[index].reserved1 = 0;
	PWM[index].reserved2 = 0;
	PWM[index].reserved3 = 0;
}

void ClearAll(void){	
	unsigned char i;
	for (i = 0; i< portCount; i++){
		ClearPWM(i);
	}	
}

void StopTimer(void){
	TIM2_CR1 &= 254;
}

void StartTimer(void){
	//TIM2_CR1 |= 1;
}


void SetTimer(int value, char div){
	char h, l; 	
	if (value){
		l = value;
		h = value >> 8;
	}
	else{
		l = 160;
		h = 1;
	}	
	if (div < 2){
		div = 2;		
	}
	if (l + h < 80/div){
		l = 80;
		h = 0;
	}
	TIM2_ARRH = h;      //1000000 / 100 ~ 10000Hz;
	TIM2_ARRL = l;    //1000000 / 100 ~ 10000Hz;
	TIM2_PSCR = div;      //Предделитель 16000000 /16 = 1000000 Hz
}


void Init(void){
	// 2MHz
	CLK_CKDIVR = 0; //SET 16 MHZ
	
	PA_DDR = PB_DDR = PC_DDR = PD_DDR = 0;
	PA_ODR = PB_ODR = PC_ODR = PD_ODR = 0;
	PA_CR1 = PB_CR1 = PC_CR1 = PD_CR1 = 0;
  PA_CR2 = PB_CR2 = PC_CR2 = PD_CR2 = 0;
	
	
  PA_DDR = PC_DDR = PD_DDR = 255;
	PA_CR1 = PC_CR1 = PD_CR1 = 255;
  PA_CR2 = PC_CR2 = PD_CR2 = 0;
	
	
	#ifdef PE_ODR
	PE_DDR = 0
	PE_ODR = 0
	PE_CR1 = 0
	PE_CR2 = 0
	#endif
	
	#ifdef PF_ODR
	PF_DDR = 0
	PF_ODR = 0
	PF_CR1 = 0
	PF_CR2 = 0
	#endif
		
	TIM2_CR1 = bit7;    //Разрешаем буферизацию ARR
	TIM2_IER =  1;      //Update overflow and Capture/Compare 1
	SetTimer(settings.TimerValue, settings.TimerDivision);
	//TIM2_CR1 |= 1;      //En
		
	
	UART_CR1 = 0; //PARITY ODD + 9Bit (8d+1p)
	UART_CR2 = bit3 + bit2 + bit5; // RX + TX
	UART_CR3 = 0; //1 STOP
	UART_CR4 = 0;
	UART_CR5 = 0;
	if (settings.BRR1 + settings.BRR2 == 0){
		UART_BRR2 = 0x6B; //600 16
		UART_BRR1 = 0x82; //600 16
		
		//UART_BRR2 = 0x03; //9600 16 MHz
		//UART_BRR1 = 0x68; //9600 16 MHz
	}
	else{
		UART_BRR2 = settings.BRR2; //9600 16 MHz
		UART_BRR1 = settings.BRR1; //9600 16 MHz
	}
	//UART_BRR2 = 0x0B; //115200 16 MHz
	//UART_BRR1 = 0x08; //115200 16 MHz
	
	//UART_BRR2 = 0x0D; //128000 16 MHz
	//UART_BRR1 = 0x07; //128000 16 MHz
	//UART_CR1 |= bit1; //EN
	
	//UART_BRR2 = 0x00; //9600 2 MHz
	//UART_BRR1 = 0x0D; //9600 2 MHz
	//UART_CR1 |= 1;
				
	
	ClearAll();
		
	ticks = 0;
	
	StartTimer();

	#asm
	rim
	#endasm
}

unsigned char ProcessInTask(struct UartData* data){
	
}

unsigned char ProcessOutTask(struct UartData* data){
	char pNum = data->port;
	struct Port * port;
	unsigned char bitNum;
	port = ports[pNum];
	bitNum = bits[pNum];
	port->DDR |= bitNum; //DDR
	port->CR1 |= bitNum; //СR1
	port->CR2 |= bitNum; //СR2
	PWM[pNum].value = data->highValue;
	PWM[pNum].lowValue = data->lowValue;
	PWM[pNum].highValue = data->highValue;
	PWM[pNum].currentPos = 1;
	if (data->highValue == 0){
		port->Out &= ~bitNum;
		PWM[pNum].currentPos = 2;
	}
	else{		
		port->Out |= bitNum;
		if (data->lowValue == 0){
			PWM[pNum].currentPos = 2;
		}
	}
	return 1;
}

unsigned char ProcessUartPacket(struct UartData* data){
	char dataState;
	if (data->command == 1){ //get
		uartDataBuf.port = TIM2_PSCR;
		uartDataBuf.lowValue = TIM2_ARRL;	
		uartDataBuf.highValue = TIM2_ARRH;	
		StopTimer();
		uartSend();
		StartTimer();
	}
	if (data->command == 2){ //set
		dataState = FLASH_IAPSR;
		dataState &= bit3;
		if (dataState == 0){
			FLASH_DUKR = 0xAE;
			FLASH_DUKR = 0x56;
		}
		settings.TimerValue = uartDataBuf.lowValue;
		settings.TimerDivision = uartDataBuf.port;
		SetTimer(uartDataBuf.lowValue, uartDataBuf.port);
	}
	if (data->command == 3){ //get
		uartSendByte(data->port);	
		//uartDataBuf.data = EEPROM[uartDataBuf.address];
	}
	if (data->command == 4){ //get
		ProcessInTask(data);
		//uartDataBuf.data = EEPROM[uartDataBuf.address];
	}
	if (data->command == 5){ //get
		ProcessOutTask(data);
		//uartDataBuf.data = EEPROM[uartDataBuf.address];
	}
	if (uartDataBuf.command == 6){ //Stop
		StopTimer();
	}
	if (uartDataBuf.command == 7){ //Stop
		StartTimer();
	}
	if (uartDataBuf.command == 9){ //Stop
		ClearAll();
	}
	if (uartDataBuf.command == 10){ //Reset
		Reset();
	}
}


void processTask(char i){
	if (PWM[i].currentPos < 2){
			if (PWM[i].value == 0){
				if (PWM[i].currentPos){
					PWM[i].currentPos = 0;
					PWM[i].value = PWM[i].lowValue;
					ports[i]->Out &= ~bits[i];
				}
				else{
					PWM[i].currentPos = 1;
					PWM[i].value = PWM[i].highValue;
					ports[i]->Out |= bits[i];
				}
			}
			else{
				PWM[i].value--;
			}	
		}
}

main()
{
	char i;
	Init();
	uartDataBuf.command = 0;
	uartDataBuf.port = TIM2_PSCR;
	uartDataBuf.lowValue = TIM2_ARRL;	
	uartDataBuf.highValue = TIM2_ARRH;	
	StopTimer();
	uartSend();
	StartTimer();
	while (1){
		for (i = 0; i < portCount; i++){
			processTask(i);
		}		
		if (processUart()){
			ProcessUartPacket(&uartDataBuf);
			resetUartState();
		}
	}
}


@interrupt void iTim2_overflow(void){
	//_asm("SIM");
	unsigned char i;
	TIM2_SR1 &= 254;
	for (i = 0; i < portCount; i++){
		processTask(i);
	}
	//_asm("RIM");
}

@interrupt void iTim2_compare(void){
	//_asm("SIM");
	TIM2_SR1 &= 254;
	
	//_asm("RIM");
}
