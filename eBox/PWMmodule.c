#include "DeviceConfig.h"
#include "Modules.h"

unsigned char PinTaskHandler(union TaskData task, long ticks);
unsigned char GradTaskHandler(union TaskData task, long ticks);
unsigned char PwmTaskHandler(union TaskData task, long ticks);
unsigned char BeeperTaskHandler(union TaskData task, long ticks);
unsigned char SysTaskHandler(union TaskData task, long ticks);

struct PinTask{
	unsigned char type : 3;
	unsigned char port : 5;
	unsigned char value;		
	unsigned int start;
};

struct PwmTask{
	unsigned char type : 3;
	unsigned char port : 5;	
	unsigned char increment;
	unsigned int start;
};

union PwmTaskData{
	long data;
	struct PwmTask pwmTask;
	struct PinTask pinTask;
};

struct PWMdata{
	unsigned char value;
	signed short increment;
	unsigned long startTicks;
};

extern struct WorkSettings settings;

@near struct PWMdata PWM[portCount];
unsigned char ticksByte;
unsigned char PWMcursor;

void OnInit(struct WorkSettings settings){
	unsigned char i;
	
	TIM2_CR1 = bit7;    //Разрешаем буферизацию ARR
	TIM2_ARRH = 0;      //2000000 / 100 ~ 20000Hz;
	TIM2_ARRL = 100;    //2000000 / 100 ~ 20000Hz;
	TIM2_IER =  1;      //Update overflow and Capture/Compare 1
	TIM2_PSCR = 3;      //Предделитель 16000000 /8 = 2000000 Hz
	TIM2_CR1 |= 1;      //En
	
	ticksByte = 0;	
	
	for (i = 0; i< portCount; i++){
		PWM[i].value = 0;
		PWM[i].increment = 0;
		PWM[i].startTicks = 0;
	}	
	
	RegisterTaskHandler(1, &PinTaskHandler);
	RegisterTaskHandler(2, &PinTaskHandler);
	RegisterTaskHandler(3, &PinTaskHandler);
	RegisterTaskHandler(4, &PwmTaskHandler);
}

unsigned char PinTaskHandler(union TaskData data, long ticks){
	union PwmTaskData task;
	unsigned char pNum;
	struct Port *port;
	unsigned char bitNum;
	unsigned char value;
	task.data = data.data;
	pNum = task.pinTask.port;
	port = ports[pNum];
	bitNum = bits[pNum];
	port->DDR |= bitNum; //DDR
	port->CR1 |= bitNum; //СR1
	port->CR2 |= bitNum; //СR2
	if (task.pinTask.type == 1){
		PWM[pNum].value = task.pinTask.value;
		PWM[task.pwmTask.port].increment = 0;	
	}
	else{
		if ( task.pwmTask.type == 4){
			PWM[task.pwmTask.port].value = task.pwmTask.increment;	
			PWM[task.pwmTask.port].increment = 0;	
		}
		else{
			if ( task.pwmTask.type == 2){
				PWM[task.pwmTask.port].increment = task.pwmTask.increment;
			}			
			if ( task.pwmTask.type == 3){
				PWM[task.pwmTask.port].increment = -task.pwmTask.increment;
			}			
		}
		PWM[task.pwmTask.port].startTicks = ticks;
	}
	value = PWM[pNum].value;
	if (value > 0){
		port->Out |= bitNum;
	}
	else{		
		port->Out &= ~bitNum;
	}
	return 1;
}

unsigned char GradTaskHandler(union TaskData data, long ticks){
	
	PinTaskHandler(data, ticks);
	return 1;
}

unsigned char PwmTaskHandler(union TaskData data, long ticks){
	union PwmTaskData task;
	unsigned char pNum;
		
	PinTaskHandler(data, ticks);
		/*
	task.data = data.data;
	pNum = task.pinTask.port;
	if (task.pinTask.type == 1){
		PWM[pNum].value = task.pinTask.value;
	}
	else{
		PWM[task.pwmTask.port].increment = task.pwmTask.type == 2 ? task.pwmTask.increment : -task.pwmTask.increment;
		PWM[task.pwmTask.port].startTicks = ticks;
	}
	value = PWM[pNum].value;
	if (value > 0){
		port->Out |= bitNum;
	}
	else{		
		port->Out &= ~bitNum;
	}
	return 1;
	*/
	return 1;
}

unsigned char BeeperTaskHandler(union TaskData task, long ticks){
	BEEP_CSR = task.sysTask.Param0;
	return 1;
}

void OnCicle(unsigned long ticks){
	struct PWMdata* pwm;	
	unsigned int overflows = 0;
	signed short value;
	signed char sgn;
	unsigned int absss;
	if (PWM[PWMcursor].increment != 0 && PWM[PWMcursor].startTicks > 0){
		pwm = &PWM[PWMcursor];
		sgn = pwm->increment > 0 ? 1 : -1;
		absss = sgn > 0 ? pwm->increment : -pwm->increment;
		absss *= 4;
		value = ticks - pwm->startTicks;
		if (ticks - pwm->startTicks >= absss){
			pwm->value = pwm->value + sgn;
			pwm->startTicks = ticks;
			if (pwm->value == 255 || pwm->value == 0){
				pwm->increment = 0;
				pwm->startTicks = 0;
				if (pwm->value == 255){
					ports[PWMcursor]->Out |= bits[PWMcursor];
				}
				if (pwm->value == 0){
					ports[PWMcursor]->Out &= ~bits[PWMcursor];
				}
			}
		}
	}
	PWMcursor++;
	if (PWMcursor == portCount){
		PWMcursor = 0;
		overflows++;
	}
}

void OnTick(unsigned long ticks){

}

@interrupt void iTim2_overflow(void){
	//_asm("SIM");
	unsigned char i;
	TIM2_SR1 &= 254;
	ticksByte++;	
	if (ticksByte == 255){
		ticksByte = 0;
		if (settings.WorkMode > 0){
			for (i = 0; i < portCount; i++){
				if (PWM[i].value > 0 && PWM[i].value < 255){
					ports[i]->Out |= bits[i];
				}
			}
		}
	}
	if (settings.WorkMode > 0){
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

@interrupt void iTim2_compare(void){
	//_asm("SIM");
	TIM2_SR1 &= 254;
	
	//_asm("RIM");
}