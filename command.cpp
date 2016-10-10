#include "command.h"

#include <stdio.h>
#include <string.h>
#include <hardwareSerial.h>



#ifdef __cplusplus
extern "C" {
#endif

static commandFuncPtr commandFuncPtrHolder[COMMANDQUEUEMAX];


void commandInit()
{
  memset(commandFuncPtrHolder, 0, COMMANDQUEUEMAX);
}

char registerCommand(int command, commandFuncPtr commandFunc)
{
  char errorResult;
  
  if(command > COMMANDQUEUEMAX)
  {
    errorResult = COMMANDGTMAX;
  }
  else
  if(commandFuncPtrHolder[command] != 0)
  {
    errorResult = COMMANDEXIST;
  }
  else
  if(commandFunc == 0)
  {
    errorResult = COMMANDNULL;
  }
  else
  {
    commandFuncPtrHolder[command] = commandFunc;
	Serial.print("Register Command: 0x");
	Serial.println(command, HEX);
  
    Serial.print("Command is ");
	Serial.println(( commandFuncPtrHolder[command]  == NULL )? "NULL":"OK");
	
	
    errorResult = COMMANDOK;
  }
  return errorResult;  
}

char processCommand(int command, unsigned char *receivedData)
{
  char errorResult = -1;

  //kputString("processCommand: 0x");
  //putInt(command);
  //kputString(" 0x");
  //putInt(commandFuncPtrHolder[command]);
  //kputString("\n\r");
  
  #ifdef UNITTEST
    //printf("Process Command:0x%02X\n", command);
  #endif
  
  if(commandFuncPtrHolder[command] != 0)
  {
	Serial.print("Com: ");
    Serial.println(command, HEX);
  
    //kputString("processCommand: Call command\r\n");
    (*commandFuncPtrHolder[command])(receivedData);
    
    errorResult = 0;
  }
  
  if(commandFuncPtrHolder[command] == NULL)
  {
	  Serial.print("Command ");
	  Serial.print(command, HEX);
	  Serial.println(" is NULL.");
  }
  return errorResult;
}

#ifdef __cplusplus
}
#endif