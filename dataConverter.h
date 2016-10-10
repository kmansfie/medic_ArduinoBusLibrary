#ifndef DATACONVERTER
#define DATACONVERTER

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

short escapedToSpecialChar(unsigned char c);
unsigned short specialToEscapedChar(unsigned char c);
#endif