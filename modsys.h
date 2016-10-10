// modsys.h
//
// This contains system defines like the device number of this module
//

//#ifndef UNITTEST
  #define DEVADDRESS 1
//#else
//  #define DEVADDRESS 0x7f
//#endif

enum module_t {TYPE_EXHAUST = 1, TYPE_BIDET, TYPE_SUPERMODULE, TYPE_MISC,
               TYPE_PPG, TYPE_GUI, TYPE_NEOPIXEL, TYPE_ARDUINO_MEGA};
typedef enum module_t Module;

#define RPIADDRESS 0xFE
