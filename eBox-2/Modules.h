#define RESTART_INIT_MODE 1
#define RESET_INIT_MODE 0

struct SystemTask{	
	unsigned char type : 3;
	unsigned char func : 5;
	unsigned char Param0;	
	unsigned int start;
};

struct Settings{
	unsigned int DeviceAddr;
	unsigned char Version;	
	unsigned char TimeBase : 2;
	unsigned char BeginMode : 3;
	unsigned char EndMode : 3;
};

struct WorkSettings{	
	unsigned int DeviceAddr;
	unsigned char Version;	
	unsigned char TimeBase;
	unsigned int TimeDiv;
	unsigned int Reserved;
	unsigned char ReservedInt;
	unsigned char BeginMode;
	unsigned char EndMode;
	unsigned char WorkMode;
};

union TaskData{
	struct SystemTask sysTask;
	unsigned long data;
	struct Settings settings;
	unsigned char type : 3;
};

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

typedef unsigned char (*TaskHandler)(union TaskData task, long ticks);

extern void Reset(void);
extern void Restart(void);
extern void RegisterTaskHandler(unsigned char index, TaskHandler handler);


