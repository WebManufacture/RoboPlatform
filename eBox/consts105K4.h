#ifndef _Hardware_Ext
#define _Hardware_Ext

#define taskHigh 255
#define portCount 24
//15 ports - MAX!

#include "STM8S105C6.h"
#include "Consts.h"

#define UART_CR1 UART2_CR1
#define UART_CR2 UART2_CR2
#define UART_CR3 UART2_CR3
#define UART_CR4 UART2_CR4
#define UART_CR5 UART2_CR5
#define UART_BRR2 UART2_BRR2
#define UART_BRR1 UART2_BRR1
#define UART_SR UART2_SR
#define UART_DR UART2_DR

#define SysR1 PA.Out |= bit2
#define SysR0 PA.Out &= ~bit2
#define SysG1 PB.Out |= bit5
#define SysG0 PB.Out &= ~bit5
#define SysB1 PA.Out |= bit1
#define SysB0 PA.Out &= ~bit1
                
         
#endif