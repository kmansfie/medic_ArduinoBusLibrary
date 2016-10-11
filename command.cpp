#include "command.h"

#include <stdio.h>
#include <string.h>
#include <hardwareSerial.h>



#ifdef __cplusplus
extern "C" {
#endif

static commandFuncPtr commandFuncPtrHolder[COMMANDQUEUEMAX];


/******************************************************************************
 * Function void commandInit(void)
 *
 * Initialize the command holder.
 *
 * PreCondition:    None
 *
 * Input:           None
 *                  
 * Output:          None
 *
 * Side Effects:    None
 *
 *****************************************************************************/
 
void 
commandInit()
{
  memset(commandFuncPtrHolder, 0, COMMANDQUEUEMAX);
}


/******************************************************************************
 * Function char registerCommand(int, commandFuncPtr)
 *
 * This function stores the pointer of the functions to be used in operation.
 *
 * PreCondition:    None
 *
 * Input:           'command' - command code or number. This is the key for calling the command function(commandFunc).
 *                  'commandFunc' - pointer of function that to be stored in an array.
 *                  
 * Output:          error result
 *                  COMMANDOK    -   0
 *                  COMMANDEXIST -  -1
 *                  COMMANDGTMAX -  -2
 *                  COMMANDNULL  -  -3 
 *
 * Side Effects:    None
 *
 *****************************************************************************/
 
char 
registerCommand(int command, commandFuncPtr commandFunc)
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

/******************************************************************************
 * Function char processCommand(int, unsigned char *)
 *
 * This function calls the function that is inside the array with the element 'command'.
 *
 * PreCondition:    None
 *
 * Input:           'command' - command code or number. This is the key for calling the command function(commandFunc).
 *                  'receivedData' - data received from the master device.
 *                  
 * Output:          error result
 *                  error    -   -1
 *                  no error -    0
 *
 * Side Effects:    None
 *
 *****************************************************************************/
char 
processCommand(int command, unsigned char *receivedData)
{
  char errorResult = -1;
  
  if(commandFuncPtrHolder[command] != 0)
  {
    Serial.print("Com: ");
    Serial.println(command, HEX);

    (*commandFuncPtrHolder[command])(receivedData);
    
    errorResult = 0;
  }
  
  return errorResult;
}

#ifdef __cplusplus
}
#endif