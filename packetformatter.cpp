
#include <string.h>
#include "packetformatter.h"
#include "charactercode.h"
#include "command.h"

unsigned short specialToEscapedChar(unsigned char c);

static unsigned int checkSum;
static unsigned char packetSize;
static unsigned char buffer[128];

static void 
insertValueToBuffer(unsigned char value)
{
  buffer[packetSize] = value;
  packetSize++;
}


#if 0
static void
processPacketData(unsigned char packetData)
{

  unsigned short packetDataResult = specialToEscapedChar(packetData);
  
  if( (packetDataResult >> 8) != 0)
  {
    insertValueToBuffer((unsigned char)(packetDataResult>> 8));
  }
  
  insertValueToBuffer((unsigned char) packetDataResult);
  
}
#endif

int getFormattedPacket(Packet * packet, unsigned char * packetBuffer)
{
  
  checkSum = 0;
  packetSize = 0;
  
  insertValueToBuffer(STARTPACKET);
  checkSum += STARTPACKET;
  
  #ifdef DEBUG
  printf("Start Packet: %x\n", STARTPACKET);
  #endif
  
  insertValueToBuffer(packet->deviceID);
  checkSum += packet->deviceID;
  
  #ifdef DEBUG
  printf("Device ID: %x\n", packet->deviceID);
  #endif

  insertValueToBuffer(packet->sourceID);
  checkSum += packet->sourceID;
  
  #ifdef DEBUG
  printf("Device ID: %x\n", packet->sourceID);
  #endif
  
  insertValueToBuffer(packet->command);
  checkSum += packet->command;
  
  #ifdef DEBUG
  printf("Command: %x\n", packet->command);
  #endif
  
  unsigned char mostSignificantBytePacketLength = (unsigned char) packet->packetLength >> 8;
  unsigned char leastSignificantBytePacketLength = (unsigned char) packet->packetLength;
  insertValueToBuffer(mostSignificantBytePacketLength);
  checkSum += mostSignificantBytePacketLength;
  insertValueToBuffer(leastSignificantBytePacketLength);
  checkSum += leastSignificantBytePacketLength;
  
  #ifdef DEBUG
  printf("Packet length MSB: %x\n", mostSignificantBytePacketLength);
  printf("Packet length LSB: %x\n", leastSignificantBytePacketLength);
  #endif
  
  int i;
  for(i = 0; i < packet->packetLength; i++)
  {
    insertValueToBuffer(packet->packetData[i]);
    checkSum += packet->packetData[i];
	
    #ifdef DEBUG
      printf("Data %d: %x\n", i, packet->packetData[i]);
    #endif
  }
  
  unsigned char checksumLSB = (unsigned char)(checkSum & 0xFF);
  unsigned char twosComplementCheckSum = ~checksumLSB + 1;
  insertValueToBuffer(twosComplementCheckSum);
  
  #ifdef DEBUG
  printf("Checksum: %x\n", twosComplementCheckSum);
  #endif
  
  memcpy(packetBuffer, buffer, packetSize);
  
  return packetSize;
}

