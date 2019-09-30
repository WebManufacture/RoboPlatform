#include "generalConfig.h"
#include "deviceConfig.h"
#include "uart.h"
#include "utils.h"

struct UartState{
	unsigned char phase;
	unsigned char size;
	unsigned char index;
	unsigned char crcCode;
} urState, uwState;

extern char *readDataBuf;
extern char *writeDataBuf;

char InitUART(){
	UART_CR1 = 0; //PARITY ODD + 9Bit (8d+1p)
	UART_CR2 = bit5 + bit3 + bit2; //  RXIE + RX + TX
	UART_CR3 = 0; //1 STOP
	UART_CR4 = 0;
	UART_CR5 = 0;
	
	//0x0B08 - 115200 16 MHz
	//0x0368 - 9600 16 MHz
	//0x0101 - 921600 16
	//0x024E - 9600 12 MHz
	//0x0806 - 115200 12 MHz
	//0x0D00 - 921600 12

	UART_BRR2 = 0x0B08 >> 8;
	UART_BRR1 = 0x0B08;
}

char CheckUart(void){
	return urState.phase == 06 ? urState.size : 0;
}

char* GetUartData(void){
	if (urState.phase >= 6){
		return readDataBuf;
	}
	else{
		return 0;
	}
}

void ClearUart(void){
	urState.phase = 0;
}

void WaitSend(void){
	while(uwState.phase > 0 && uwState.phase < 3){
		
	}
}

void UartSendData(uchar size){
	WaitSend();
	#ifdef UART_RAW_MODE
	uwState.phase = 2;
	#else
	uwState.phase = 0;
	#endif
	uwState.index = 0;
	uwState.size = size;
	#ifdef UART_TX_CRC_VALUE
	uwState.crcCode = UART_TX_CRC_VALUE;
	#endif	
	UART_CR2 |= bit7;//(TIEN) TXE interrupt	
}

void UART_RX_handler(void){
	unsigned char val;
	unsigned int index;
	_asm("SIM");
	res(UART_SR,5);
	val = UART_DR;
	switch (urState.phase){
		case 3: 
			urState.phase = val == 3 ? 6 : 0;
		break;
		case 2:
			if (urState.index >= urState.size){
				urState.phase = val == 3 ? 6 : (val == urState.crcCode ? 3 : 0);
			}
			else{
				readDataBuf[urState.index] = val;
				urState.crcCode ^= val;
				urState.index++;
			}
		break;
		case 1:
			if (val > 0){
				urState.phase = 2;
				urState.size = val;				
			}
			else{
				urState.phase = 0;
				urState.size = 0;
			}
			urState.index = 0;
		break;
		case 0:
			if (val == 1 || val == 2){
				#ifdef UART_RX_CRC_VALUE
				urState.crcCode = UART_RX_CRC_VALUE;
				#else
				urState.crcCode = 222;
				#endif
				urState.phase = val;
			}
			urState.index = 0;
		break;
	}
	_asm("RIM");
}

void UART_TX_handler(void){
	unsigned char val;
	unsigned char wv;
	_asm("SIM");
	if (uwState.index >= uwState.size){
		#ifdef UART_RAW_MODE
			uwState.phase = 6;
			UART_CR2 = UART_CR2 & ~bit7; //UART TX OFF
		#else
			if (uwState.phase < 6){
				#ifdef UART_TX_CRC_VALUE
					if (uwState.phase < 3){
						UART_DR = uwState.crcCode;
						uwState.phase = 3;
					}
					else{
						UART_DR = 03;//EOT
						uwState.phase = 6;
					}
				#else
					UART_DR = 03;//EOT
					uwState.phase = 6;
				#endif
			}
			else{
				UART_CR2 = UART_CR2 & ~bit7; //UART TX OFF
			}		
		#endif
	}
	else{
		if (uwState.phase == 2){
			val = writeDataBuf[uwState.index];
			UART_DR = val;
			uwState.index++;			
			uwState.crcCode ^= val;
		}		
		if (uwState.phase == 1){
			UART_DR = uwState.size;
			uwState.phase = 2;
		}
		if (uwState.phase == 0){
			#ifdef UART_TX_UNSIZED_PACKETS
			UART_DR = 2;//SOH
			uwState.phase = 2;
			#else
			UART_DR = 1;//SOH
			uwState.phase = 1;
			#endif
			#ifdef UART_TX_CRC_VALUE
				uwState.crcCode = UART_TX_CRC_VALUE;
			#endif
		}
	}
	_asm("RIM");
}


@interrupt void UART_RX_IRQHandler(void){
	UART_RX_handler();
}

@interrupt void UART_TX_IRQHandler(void){
	UART_TX_handler();
}
