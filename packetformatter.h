#ifndef PACKETFORMATTER
#define PACKETFORMATTER

typedef struct
{
	int deviceID;
  int sourceID;
	unsigned char command;
	unsigned short packetLength;
	unsigned char packetData[1024];

} Packet;

extern int getFormattedPacket(Packet *packet, unsigned char * packetBuffer);
extern int getArraySize(unsigned char* sampleData);

#endif