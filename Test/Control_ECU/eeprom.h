#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>

/* ================= EEPROM API ================= */

/* Initialize EEPROM module */
void EEPROM_Init(void);

/* Write 32-bit word to EEPROM (word address) */
void EEPROM_Write(uint16_t address, uint32_t data);

/* Read 32-bit word from EEPROM (word address) */
uint32_t EEPROM_Read(uint16_t address);

/* Erase whole EEPROM */
void EEPROM_MassErase(void);

/* ================= Application Helpers ================= */

/* Password handling */
void EEPROM_SavePassword(char *password, uint8_t length);
void EEPROM_LoadPassword(char *password, uint8_t length);

/* Auto-lock time */
void EEPROM_SaveAutoLock(uint8_t seconds);
uint8_t EEPROM_LoadAutoLock(void);

#endif /* EEPROM_H_ */
