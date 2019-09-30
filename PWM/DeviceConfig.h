#include "STM8S103F3P.h"
#include "consts103F3.h"

#ifndef _Hardware_Ext
#define _Hardware_Ext 1
#endif

#define RESTART_INIT_MODE 1
#define RESET_INIT_MODE 0


extern struct UartData uartDataBuf;
extern char processUart(void);
extern void resetUartState(void);
extern void uartSend(void);
extern void uartSendByte(char val);

struct PWMdata{
	unsigned long value;
	unsigned long lowValue;	
	unsigned long highValue;
	unsigned char currentPos;
	unsigned char reserved1;
	unsigned char reserved2;
	unsigned char reserved3;
};

struct UartData{
	unsigned char command;
	unsigned int address;
	unsigned char port;
	unsigned long lowValue;	
	unsigned long highValue;
};

struct Settings{
	unsigned int DeviceAddr;
	unsigned char Version;	
	unsigned char TimerDivision;
	unsigned int TimerValue;	
	unsigned char BRR2;
	unsigned char BRR1;
};

