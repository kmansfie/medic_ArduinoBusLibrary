
#include <string.h>
#include <stdio.h>
#include <hardwareSerial.h>

#include "rs485.h"
#include "command.h"
#include "packetformatter.h"
#include "modsys.h"
#include "protocol.h"

#define PRINTPACKETDEBUG

#ifdef __cplusplus
extern "C" {
#endif

  //unsigned char dbuf[256];
  //unsigned char dptr = 0;

  unsigned int buffPtr;
  unsigned char buffer[128+1];
  unsigned char receiveData[128];
  int checksum;
  extern int message;
  int pState = START;
  int returnState;
  unsigned int plength;
  int command;

  //void handleChar(unsigned char c);
  //void sendChar1(unsigned char c);

  extern void setup();

  extern unsigned char packetBuffer[128];
  extern unsigned int packetBufferSize;

  unsigned int on = 0;

  //unsigned char debbuf[256];
  //unsigned int debptr = 0;

  extern const unsigned short crc;
  extern const char modName[];
  extern const char version[];
  extern const char date[];
  extern const char time[];
  extern void sendChar(unsigned char c);
  unsigned char debbuf[256];
  unsigned int debptr = 0;

  static void
  debug(unsigned char c, int state)
  {  
    debbuf[debptr++] = 'P';
    debptr &= 255;
    debbuf[debptr++] = c;
    debptr &= 255;
    debbuf[debptr++] = state & 0xff;
    debptr &= 255;
  }

  static void
  printDebug()
  {
    Serial.println("Packetbuffer debug:");
    
    int counter = 0;
    while(counter < debptr)
    {
      if(debbuf[counter] == 'P')
      Serial.print("-");
      else
      Serial.print(debbuf[counter], HEX);
      
      counter++;
      Serial.print(" ");
    }
    Serial.println("");
  }

  void
  setResponse(int deviceID, int command, int status)
  {
    Serial.print("Set Response: DevID: ");
    Serial.print(deviceID, HEX);
    Serial.print(", Command: ");
    Serial.print(command, HEX);
    Serial.print(",Status: ");
    Serial.println(status, HEX);
    
    int length;
    Packet packet;
    
    length = 0;
    packet.deviceID = deviceID;
    packet.sourceID = DEVADDRESS;
    packet.command = command;
    packet.packetData[length++] = status;
    packet.packetLength = length;
    packetBufferSize = getFormattedPacket(&packet, packetBuffer);
    message = 1;
  }

  /******************************************************************************
* Function void sendChar(unsigned char)
*
* This function is called to send a character to serial line.
*
* PreCondition:    None
*
* Input:           'c' - character to send
*
* Output:          None
*
* Side Effects:    None
*
*****************************************************************************/
  void 
  sendChar(unsigned char c)
  { 
    //Serial.print("Send Char: ");
    //Serial.println(c, HEX);
    
    //Serial.print("Write ret: ");
    //Serial.println(Serial1.write(c));
    Serial1.write(c);
  }

  /******************************************************************************
* Function void sendPacket(unsigned char *, int)
*
* This function is called to send a packet to serial line.
*
* PreCondition:    None
*
* Input:           'packet' - packet to send
*                  'packetSize' - total size of packet to send
* 
* Output:          None
*
* Side Effects:    None
*
*****************************************************************************/
  void 
  sendPacket(unsigned char * packet, int packetSize)
  {
    int count;
    for(count = 0; count < packetSize; count++)
    {
      sendChar(*packet++);
    }  
  }

  //
  /******************************************************************************
* Function void blink(void)
*
* This function is called by the RTOS when the delay has expired.  Interesting
* to note that you have to call the delay routine again to get another delay.
* The LED is flashed in this routine.
*
* PreCondition:    None
*
* Input:           None
*
* Output:          None
*
* Side Effects:    blinks the LED and also toggles the on variable between 1 and
*                  0.
*
*****************************************************************************/
  static void
  blink(void)
  {

  }

  void
  serial1Handler(unsigned char RXByte)
  {
    //dbuf[dptr++] = RXByte;
    switch(pState)
    {
    case START:
      debug(RXByte,pState);
      if (RXByte == STARTPACK)
      {
        //dbuf[dptr++] = 'S';
        pState = PACKET;
        buffPtr = 0;
        buffer[buffPtr++] = STARTPACK;
        checksum = STARTPACK;
      }
      else
      if (RXByte == STARTPOLL)
      {
        pState = POLL;
      }
      else
      if (RXByte == ACK)
      {
        message = 0;
      }
      else
      if (RXByte == NAK)
      {
        if (message)
        {
          int j;
          j = 0;
          sendChar(packetBuffer[j++]);
          while (j < packetBufferSize)
          {
            if (packetBuffer[j] == 0x7F)
            {
              sendChar(0x8E);
              sendChar(0x01);
              j++;
            }
            else if (packetBuffer[j] == 0x8E)
            {
              sendChar(0x8E);
              sendChar(0x8E);
              j++;
            }
            else if (packetBuffer[j] == 0x8F)
            {
              sendChar(0x8E);
              sendChar(0x02);
              j++;
            }
            else
            {
              sendChar(packetBuffer[j++]);
            }
          }
          
          message = 0;
        }
        // else
        // {
        // sendChar(ACK);
        // } 
      }
      else
      {
        pState = START;
      }
      break;
    case PACKET:
      debug(RXByte,pState);
      if (RXByte == STARTPACK)
      {
        pState = PACKET;
        buffPtr = 0;
        buffer[buffPtr++] = STARTPACK;
        checksum = STARTPACK;
      }
      else
      if (RXByte == STARTPOLL)
      {
        pState = POLL;
      }
      else
      if (RXByte == DEVADDRESS)
      {                    
        //dbuf[dptr++] = 'A';
        Serial.println((String)"Address Match:" + DEVADDRESS);
        
        pState = SOURCEADD;
        buffer[buffPtr++] = RXByte;
        checksum += RXByte;
      }
      else
      if (RXByte == ESCAPE)
      {
        returnState = PACKET;
        pState = HANDLEESCAPE;
      }
      else
      {
        pState = START;
      }
      
      break;
    case SOURCEADD:
      debug(RXByte, pState);
      //dbuf[dptr++] = 'R';
      if (RXByte == STARTPACK)
      {
        pState = PACKET;                    
        buffPtr = 0;
        buffer[buffPtr++] = STARTPACK;
        checksum = STARTPACK;
      }  
      else
      if (RXByte == STARTPOLL)
      {
        pState = POLL;
      }
      else
      if (RXByte == ESCAPE)
      {
        returnState = SOURCEADD;
        pState = HANDLEESCAPE;
      }
      else
      {
        pState = COLLECTCOMMAND;
        buffer[buffPtr++] = RXByte;
        checksum += RXByte;
      }
      break;
    case POLL:
      debug(RXByte,pState);
      if (RXByte == STARTPACK)
      {
        pState = PACKET;
        buffPtr = 0;
        buffer[buffPtr++] = STARTPACK;
        checksum = STARTPACK;
      }
      else
      if (RXByte == STARTPOLL)
      {
        pState = POLL;
      }
      else
      if (RXByte == DEVADDRESS)
      {
        Serial.println((String)"Address Match:" + DEVADDRESS);
        if (message)
        {
          int j;
          j = 0;
          debug(packetBuffer[j],'S');
          sendChar(packetBuffer[j++]);
          while (j < packetBufferSize)
          {
            if (packetBuffer[j] == 0x7F)
            {
              sendChar(0x8E);
              sendChar(0x01);
              j++;
            }
            else if (packetBuffer[j] == 0x8E)
            {
              sendChar(0x8E);
              sendChar(0x8E);
              j++;
            }
            else if (packetBuffer[j] == 0x8F)
            {
              sendChar(0x8E);
              sendChar(0x02);
              j++;
            }
            else
            {
              debug(packetBuffer[j],'S');
              sendChar(packetBuffer[j++]);
            }
          }
          
          //message = 0;
        }
        else
        {
          sendChar(ACK);
        } 
        pState = START;
      }
      else 
      if(RXByte == ESCAPE)
      {
        returnState = POLL;
        pState = HANDLEESCAPE;
      }
      else
      {
        pState = START;
      }
      
      break;
    case COLLECTCOMMAND:
      //dbuf[dptr++] = 'C';
      debug(RXByte,pState);
      if (RXByte == STARTPACK)
      {
        pState = PACKET;
        buffPtr = 0;
        buffer[buffPtr++] = STARTPACK;
        checksum = STARTPACK;
      }
      else
      if (RXByte == STARTPOLL)
      {
        pState = POLL;
      }
      else
      if(RXByte == ESCAPE)
      {
        returnState = COLLECTCOMMAND;
        pState = HANDLEESCAPE;
      }
      else
      {
        command = RXByte;
        pState = COLLECTCOUNT1;
        buffer[buffPtr++] = RXByte;
        checksum += RXByte;
      }
      
      break;
    case COLLECTCOUNT1:
      debug(RXByte,pState);
      //dbuf[dptr++] = '1';
      if (RXByte == STARTPACK)
      {
        pState = PACKET;
        buffPtr = 0;
        buffer[buffPtr++] = STARTPACK;
        checksum = STARTPACK;
      }
      else
      if (RXByte == STARTPOLL)
      {
        pState = POLL;
      }
      else
      {
        if (RXByte == ESCAPE)
        {
          returnState = COLLECTCOUNT1;
          pState = HANDLEESCAPE;                        
        }
        else
        {
          pState = COLLECTCOUNT2;
          checksum += RXByte;
          plength = RXByte << 8;
          buffer[buffPtr++] = RXByte;
          checksum += RXByte;
        }
      }
      
      break;
    case COLLECTCOUNT2:
      debug(RXByte,pState);
      //dbuf[dptr++] = '2';
      if (RXByte == STARTPACK)
      {
        pState = PACKET;
        buffPtr = 0;
        buffer[buffPtr++] = STARTPACK;
        checksum = STARTPACK;
      }
      else
      if (RXByte == STARTPOLL)
      {
        pState = POLL;
      }
      else
      {
        if (RXByte == ESCAPE)
        {
          returnState = COLLECTCOUNT2;
          pState = HANDLEESCAPE;
        }
        else
        {
          pState = COLLECTPACKET;
          plength += RXByte;
          buffer[buffPtr++] = RXByte;
          checksum += RXByte;
        }
      }  
      
      break;
    case COLLECTPACKET:
      debug(RXByte,pState);
      if (RXByte == STARTPACK)
      {
        pState = PACKET;
        buffPtr = 0;
        buffer[buffPtr++] = STARTPACK;
        checksum = STARTPACK;
      }
      else
      if (RXByte == STARTPOLL)
      {
        pState = POLL;
      }
      else
      {
        if (RXByte == ESCAPE)
        {
          returnState = COLLECTPACKET;
          pState = HANDLEESCAPE;
        }
        else
        {
          plength--;
          buffer[buffPtr++] = RXByte;
          checksum += RXByte;
          
          if (plength == 0)
          {
            pState = COLLECTCHECKSUM;
          }
        }
      }
      
      break;
    case COLLECTCHECKSUM:
      debug(RXByte,pState);
      if (RXByte == ESCAPE)
      {
        returnState = COLLECTCHECKSUM;
        pState = HANDLEESCAPE;
      }
      else
      {
        if (RXByte == ((checksum*-1) & 0xFF))
        {
          //dbuf[dptr++] = 'G';
          message = 0;
          memset(packetBuffer, '\0', 256);
          
          #ifdef PRINTPACKETDEBUG
          printDebug();
          #endif
          
          memcpy(receiveData,&buffer[6],128);
          char errorResult = processCommand(command,receiveData);
          
          if(errorResult == -1)
          {
            setResponse(RPIADDRESS, command, ERRORCOMMAND);
          }  
          debug(ACK, pState);
          sendChar(ACK);
        }
        else
        {
          //dbuf[dptr++] = 'B';
          sendChar(NAK);
        }
        
        pState = START;                    
      }
      break;
    case HANDLEESCAPE:
      debug(RXByte,pState);
      if (RXByte == 1)
      {
        RXByte = 0x7F;
      }
      else
      if (RXByte == 2)
      {
        RXByte = 0x8F;
      }
      
      switch (returnState)
      {
      case COLLECTCOUNT1:
        pState = COLLECTCOUNT2;
        plength = RXByte << 8;
        buffer[buffPtr++] = RXByte;
        checksum += RXByte;
        break;
      case COLLECTCOUNT2:
        pState = COLLECTPACKET;
        plength += RXByte;
        buffer[buffPtr++] = RXByte;
        checksum += RXByte;
        break;
      case COLLECTPACKET:
        plength--;
        buffer[buffPtr++] = RXByte;
        checksum += RXByte;
        pState = COLLECTPACKET;
        if (plength == 0)
        {
          pState = COLLECTCHECKSUM;
          // message done
        }
        
        break;
      case COLLECTCHECKSUM:
        if (RXByte == ((checksum*-1) & 0xFF))
        {
          #ifdef PRINTPACKETDEBUG
          printDebug();
          #endif
          
          char errorResult = processCommand(command,&buffer[6]);
          if(errorResult == -1)
          {
            setResponse(RPIADDRESS, command, ERRORCOMMAND);
          }
          sendChar(ACK);
        }
        else
        {
          sendChar(NAK);
        }
        
        pState = START;
        break;
      case PACKET:
        if (RXByte == DEVADDRESS)
        {
          Serial.println((String)"Address Match:" + DEVADDRESS);
          pState = COLLECTCOMMAND;
          buffer[buffPtr++] = RXByte;
          checksum += RXByte;
        }
        else
        {
          pState = START;
          sendChar(NAK);
        }
        
        break;
      case COLLECTCOMMAND:
        command = RXByte;
        pState = COLLECTCOUNT1;
        buffer[buffPtr++] = RXByte;
        checksum += RXByte;
        
        break;
      case POLL:
        pState = START;
        if (RXByte == DEVADDRESS)
        {
          if (message)
          {
            int j;
            j = 0;
            debug(packetBuffer[j],'S');
            sendChar(packetBuffer[j++]);
            while (j < packetBufferSize)
            {
              if (packetBuffer[j] == 0x7F)
              {
                sendChar(0x8E);
                sendChar(0x01);
                j++;
              }
              else if (packetBuffer[j] == 0x8E)
              {
                sendChar(0x8E);
                sendChar(0x8E);
                j++;
              }
              else if (packetBuffer[j] == 0x8F)
              {
                sendChar(0x8E);
                sendChar(0x02);
                j++;
              }
              else
              {
                debug(packetBuffer[j],'S');
                sendChar(packetBuffer[j++]);
              }
            }
            
            //message = 0;
          }
          else
          {
            sendChar(ACK);
          }
        }

        break;
        
      case SOURCEADD:
        pState = COLLECTCOMMAND;
        buffer[buffPtr++] = RXByte;
        checksum += RXByte;
        break;
        
      }
      break;
    }
  }

#ifdef __cplusplus
}
#endif

