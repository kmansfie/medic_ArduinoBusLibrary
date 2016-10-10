#ifndef RS485
#define RS485

#ifdef __cplusplus
extern "C" {
#endif

void sendChar(unsigned char c);
void serial1Handler(unsigned char RXByte);
void setResponse(int deviceID, int command, int status);

#ifdef __cplusplus
}
#endif

#endif
