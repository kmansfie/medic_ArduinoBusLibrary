#include "registerDefaultCommand.h"
#include "command.h"
#include "packetformatter.h"
#include "modsys.h"

#include <hardwareSerial.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif



int message;
unsigned char packetBuffer[128];
unsigned int packetBufferSize;

static void 
getModuleName(unsigned char *receivedData)
{

  Serial.println("Get Module Type");
  
  int length;
  Packet packet;
  packet.deviceID = RPIADDRESS;
  packet.sourceID = DEVADDRESS;
  packet.command = 1;
  length = 0;

  packet.packetData[length++] = (mod_type_ << 8) & 0xFF;
  packet.packetData[length++] = mod_type_ & 0xFF;

  packet.packetData[length++] = 0;
  packet.packetLength = length;
  packetBufferSize = getFormattedPacket(&packet, packetBuffer);
  message = 1;
}

static void 
getVersion(unsigned char *receivedData)
{
  int length;
  Packet packet;
  packet.deviceID = RPIADDRESS;
  packet.sourceID = DEVADDRESS;
  packet.command = 2;
  length = 0;
  
  packet.packetData[length++] = (version_ << 8) & 0xFF;
  packet.packetData[length++] = version_ & 0xFF;

  packet.packetData[length++] = 0;
  packet.packetLength = length;
  packetBufferSize = getFormattedPacket(&packet, packetBuffer);
  message = 1;
}

typedef struct { char *key; int val;} map_t;

static map_t month_lookup[] = { {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},
                                {"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Aug", 8},
                                {"Sep", 9}, {"Oct", 10},{"Nov", 11}, {"Dec",12}};
                                
#define MONTHCOUNT (sizeof(month_lookup)/sizeof(map_t))

static void 
getDate(unsigned char *receivedData)
{
  int length;
  Packet packet;
  packet.deviceID = RPIADDRESS;
  packet.sourceID = DEVADDRESS;
  packet.command = 3;
  length = 0;
  
  char month[4];
  sprintf(month, "%c%c%c", date_[0], date_[1], date_[2]);
  unsigned char index;
  for(index = 0; index < MONTHCOUNT; index++)
  {
    if(strcmp(month_lookup[index].key, month) == 0)
    {
      packet.packetData[length++] = month_lookup[index].val;
      break;
    }
  }
  
  char day[3];
  sprintf(day, "%c%c", date_[4], date_[5]);
  unsigned char day_int = (unsigned char) atoi(day);
  
  packet.packetData[length++] = day_int;
      
  char year[5];
  sprintf(year, "%c%c%c%c", date_[7], date_[8], date_[9], date_[10]);
  unsigned short year_int = (unsigned short) atoi(year);
      
  packet.packetData[length++] = (year_int >> 8) & 0xFF;
  packet.packetData[length++] = year_int & 0xFF;
 
  packet.packetData[length++] = 0;
  packet.packetLength = length;
  packetBufferSize = getFormattedPacket(&packet, packetBuffer);
  message = 1;
}

static void 
getTime(unsigned char *receivedData)
{
  int length;
  Packet packet;
  packet.deviceID = RPIADDRESS;
  packet.sourceID = DEVADDRESS;
  packet.command = 4;
  length = 0;

  while (time_[length])
  {
    packet.packetData[length] = time_[length];
    length++;
  }

  packet.packetData[length++] = 0;
  packet.packetLength = length;
  packetBufferSize = getFormattedPacket(&packet, packetBuffer);
  message = 1;
}

void registerDefaultCommand()
{
  registerCommand(0x01, getModuleName);
  registerCommand(2, getVersion);
  registerCommand(3, getDate);
  registerCommand(4, getTime);
}

#ifdef __cplusplus
}
#endif

