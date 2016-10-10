#ifndef COMMAND
#define COMMAND

#define COMMANDOK     0
#define COMMANDEXIST -1
#define COMMANDGTMAX -2
#define COMMANDNULL  -3

#define ERRORCOMMAND 0xEE
#define COMMANDQUEUEMAX 100

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*commandFuncPtr) (unsigned char*);


extern void commandInit();
extern char registerCommand(int command, commandFuncPtr commandFunc);
extern char processCommand(int command,unsigned char *receivedData);

#ifdef __cplusplus
}
#endif

#endif
