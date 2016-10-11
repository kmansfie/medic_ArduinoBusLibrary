#include <stdio.h>
#include "dataConverter.h"
#include "charactercode.h"

static unsigned char isEscaped = FALSE;

/******************************************************************************
 * Function unsigned char getEquivalentChar(unsigned char)
 *
 * This function gets the special character .
 *
 * PreCondition:    None
 *
 * Input:           'c' - character or the key to get that the special character.
 *                  
 * Output:          special character (e.g. 0x7f, 0x8d, 0x8f)
 *
 * Side Effects:    None
 *
 *****************************************************************************/
static unsigned char 
getEquivalentChar(unsigned char c)
{
  unsigned char equivalentChar;
  switch(c)
  {
    case 0x01:
	  equivalentChar = STARTPACKET;
	  break;
	case 0x02:
	  equivalentChar = STARTPOLL;
	  break;
	case FLAGBYTE:
	  equivalentChar = FLAGBYTE;
	  break;
	default:
	  equivalentChar = 0;
	  break;
  }
  
  return equivalentChar;
}

/******************************************************************************
 * Function short escapedToSpecialChar(unsigned char)
 *
 * This function converts the escaped character to special character (e.g. 0x8F 0x01 -> 0x7F) .
 *
 * PreCondition:    None
 *
 * Input:           'c' - character to convert to special character.
 *                  
 * Output:          -1 - the input is a flag byte. Need to call this function again 
 *                       and input the character or key to get the special character.
 *                  0x7f, 0x8d, 0x8f - special character.
 *
 * Side Effects:    None
 *
 *****************************************************************************/
short 
escapedToSpecialChar(unsigned char c)
{
  short character = c;
  
  if(isEscaped)
  {
    isEscaped = FALSE;
	character = getEquivalentChar(c);
  }
  else if(!isEscaped && c == FLAGBYTE)
  {
    isEscaped = TRUE;
	character = -1;
  }
  
  return character;
}


/******************************************************************************
 * Function unsigned short specialToEscapedChar(unsigned char)
 *
 * This function converts the special character to escaped character (e.g. 0x7F -> 0x8F 0x01  ) .
 *
 * PreCondition:    None
 *
 * Input:           'c' - character to convert to escaped character.
 *                  
 * Output:          escaped character
 *
 * Side Effects:    None
 *
 *****************************************************************************/
unsigned short
specialToEscapedChar(unsigned char c)
{
  
  unsigned short escapedChar = 0;
  char isSpecialChar = ((STARTPOLL == c) || (STARTPACKET == c) || (FLAGBYTE == c));

  if(isSpecialChar)
  {
    escapedChar = (FLAGBYTE << 8);
   switch(c)
   {
     case STARTPACKET:
	   escapedChar |= 0x01;
       break;
     case STARTPOLL:
       escapedChar |= 0x02;
       break;
     case FLAGBYTE:
       escapedChar |= FLAGBYTE;
       break;	   
   }
  }
  else
  {
    escapedChar = (unsigned char) c;
  }
  
  return escapedChar;
}



