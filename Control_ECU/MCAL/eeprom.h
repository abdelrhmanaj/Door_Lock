
#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>

/******************************************************************************
 *                              Definitions                                    *
 ******************************************************************************/

/* EEPROM Return Codes */
#define EEPROM_SUCCESS          0
#define EEPROM_ERROR            1
#define EEPROM_TIMEOUT          2

/* EEPROM Configuration */
#define EEPROM_BLOCK_SIZE       16      /* 16 words per block */
#define EEPROM_WORD_SIZE        4       /* 4 bytes per word */
#define EEPROM_TOTAL_BLOCKS     32      /* 32 blocks total */
#define EEPROM_TOTAL_SIZE       2048    /* 2KB total */

/* EEPROM EEDONE Register Bits */
#define EEPROM_EEDONE_WORKING   0x00000001  /* EEPROM Working */
#define EEPROM_EEDONE_INVPL     0x00000100  /* Invalid Program/Erase Voltage Level */
#define EEPROM_EEDONE_WRBUSY    0x00000020  /* Write Busy */
#define EEPROM_EEDONE_NOPERM    0x00000010  /* Write Without Permission */
#define EEPROM_EEDONE_WKCOPY    0x00000008  /* Working on a Copy */
#define EEPROM_EEDONE_WKERASE   0x00000004  /* Working on an Erase */

uint8_t EEPROM_Init(void);
uint8_t EEPROM_WriteWord(uint32_t block, uint32_t offset, uint32_t data);
uint8_t EEPROM_ReadWord(uint32_t block, uint32_t offset, uint32_t *data);
uint8_t EEPROM_WriteBuffer(uint32_t block, uint32_t offset, const uint8_t *buffer, uint32_t length);
uint8_t EEPROM_ReadBuffer(uint32_t block, uint32_t offset, uint8_t *buffer, uint32_t length);
uint8_t EEPROM_MassErase(void);

#endif 