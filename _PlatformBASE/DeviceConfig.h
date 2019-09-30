#ifdef STM8S003F3

#include "consts103F3.h"

#define PROGRAM_PIN_DDR PpA.ddr1
#define PROGRAM_PIN_IN  PpA.in1
#define PROGRAM_PIN_OUT PpA.out1

#endif

#ifdef STM8S103F3 

#include "consts103F3.h"

#define PROGRAM_PIN_DDR PpA.ddr1
#define PROGRAM_PIN_IN  PpA.in1
#define PROGRAM_PIN_OUT PpA.out1

#endif

#if defined STM8S105K4

#include "consts105K4.h"

#define PROGRAM_PIN_DDR PpE.ddr5
#define PROGRAM_PIN_IN  PpE.in5
#define PROGRAM_PIN_OUT PpE.out5

#endif

#ifdef STM8S105C6

#include "consts105C6.h"

#define PROGRAM_PIN_DDR PpE.ddr5
#define PROGRAM_PIN_IN  PpE.in5
#define PROGRAM_PIN_OUT PpE.out5

#endif

#define MAX_MESSAGE_SIZE 10
