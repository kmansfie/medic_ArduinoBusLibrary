#ifndef COMMAND
#define COMMAND

#define COMMANDOK     0
#define COMMANDEXIST -1
#define COMMANDGTMAX -2
#define COMMANDNULL  -3

#define ERRORCOMMAND 0xEE
#define COMMANDQUEUEMAX 100

#define FAN_CONTROL 0x10
#define VALVE_CONTROL 0x12
#define EM_GET_STATUS 0x14

#define FAN_CONTROL_ACK 0x90
#define VALVE_CONTROL_ACK 0x92
#define EM_GET_STATUS_ACK 0x94

#define GM_SET_CONFIG 0x17
#define GM_GET_CONFIG 0x18
#define GM_GET_STATUS 0x19

#define GM_SET_CONFIG_ACK 0x97
#define GM_GET_CONFIG_ACK 0x98
#define GM_GET_STATUS_ACK 0x99

#define FS_GET_STATUS_1 0x1A
#define FS_SET_CONFIG_1 0x1B
#define FS_GET_CONFIG_1 0x1C
#define FS_GET_MISC 0x1D
#define FS_SPRAYER_CONTROL 0x1F

#define FS_GET_STATUS_2 0x20
#define FS_SET_CONFIG_2 0x21
#define FS_GET_CONFIG_2 0x22

#define STATUS_LED 0x23
#define LED_1 0x24
#define LED_2 0x25

#define FS_SET_CONFIG_DEFAULT_1 0x26
#define FS_SET_CONFIG_DEFAULT_2 0x27
#define FS_GET_CONFIG_DEFAULT_1 0x28
#define FS_GET_CONFIG_DEFAULT_2 0x29

#define FS_GET_STATUS_ACK_1 0x9A
#define FS_SET_CONFIG_ACK_1 0x9B
#define FS_GET_CONFIG_ACK_1 0x9C
#define FS_GET_MISC_ACK 0x9D
#define FS_SPRAYER_ACK 0X9F
#define FS_GET_STATUS_ACK_2 0xA0
#define FS_SET_CONFIG_ACK_2 0xA1
#define FS_GET_CONFIG_ACK_2 0xA2

#define STATUS_LED_ACK 0xA3
#define LED_1_ACK 0xA4
#define LED_2_ACK 0xA5

#define FS_SET_CONFIG_DEFAULT_ACK_1 0xA6
#define FS_SET_CONFIG_DEFAULT_ACK_2 0XA7
#define FS_GET_CONFIG_DEFAULT_ACK_1 0xA8
#define FS_GET_CONFIG_DEFAULT_ACK_2 0XA9

#define FLM_SET_NEOPIXEL_COLOR 0x2A
#define FLM_SET_NEOPIXEL_PATTERN_1 0x2B

#define FLM_SET_NEOPIXEL_COLOR_ACK 0xAA
#define FLM_SET_NEOPIXEL_PATTERN_1_ACK 0xAB

#define ARDUINO_LED_CONTROL 0x2C

#define ARDUINO_LED_CONTROL_ACK 0xAC


#ifdef __cplusplus
extern "C" {
#endif

typedef void (*commandFuncPtr) (unsigned char*);


extern void commandInit();
extern char registerCommand(int command, commandFuncPtr commandFunc);
extern char processCommand(int command,unsigned char *receivedData);

#ifdef __cplusplus
}
#endif

#endif
