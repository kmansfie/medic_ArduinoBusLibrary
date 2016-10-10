/******************************************************************
 *
 * File system for EEPROM
 *   This file defines the file system for all modules in the
 *   smart toilet application.
 *
 *
 *
 *
 ******************************************************************/

#define SECTOR_TOTAL_LENGTH 128

typedef struct {
  unsigned int sectorNum;
  unsigned int sectorLink;
  unsigned long length;
  unsigned int sectorCRC;
  unsigned char data;
} sector;
