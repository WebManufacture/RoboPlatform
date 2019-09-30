#include "Macro.h"

#ifndef UTILS_MODULE 

#define UTILS_MODULE 1

signed char __evt_init_clock(char event, char* data);

signed char sign (signed long x, signed long y);
signed long abs(signed long x);
void Delay(long waitCalc);
void UnblockEEPROM(void);
void UnblockFLASH(void);
void Reset(void);

#endif