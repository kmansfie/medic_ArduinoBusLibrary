#include <stdio.h>
#include "dataConverter.h"
#include "charactercode.h"

static unsigned char isEscaped = FALSE;

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



