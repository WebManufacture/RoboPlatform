#ifdef STM8S003F3

#define MESSAGE_QUEUE_LENGTH 2
#define MAX_KNOWN_DEVICES 1
#include "consts103F3.h"

#endif

#ifdef STM8S103F3 

#define MAX_KNOWN_DEVICES 1
#define MESSAGE_QUEUE_LENGTH 4
#include "consts103F3.h"

#endif

#if defined STM8S105K4

#include "consts105K4.h"

#endif

#ifdef STM8S105C6
#include "consts105C6.h"
#endif

#ifndef MAX_MESSAGE_SIZE
#define MAX_MESSAGE_SIZE 10
#endif

#define UART_RAW_MODE 1
