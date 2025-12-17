#include "eeprom.h"
#include "../tm4c123gh6pm.h"

/* ================= DEFINES ================= */

#define EEPROM_PASSWORD_BASE   0x000   // word address
#define EEPROM_AUTOLOCK_ADDR   0x020   // word address

/* ================= BASIC FUNCTIONS ================= */

void EEPROM_Init(void)
{
    SYSCTL_RCGCEEPROM_R = 1;   // Enable EEPROM clock

    // Wait until EEPROM is ready
    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);

    // Optional: check for errors
    if (EEPROM_EESUPP_R & 0x08)
    {
        // EEPROM error (rare)
    }
}

void EEPROM_Write(uint16_t address, uint32_t data)
{
    EEPROM_EEBLOCK_R  = address >> 4;   // Block number
    EEPROM_EEOFFSET_R = address & 0x0F; // Offset in block
    EEPROM_EERDWR_R   = data;

    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);
}

uint32_t EEPROM_Read(uint16_t address)
{
    EEPROM_EEBLOCK_R  = address >> 4;
    EEPROM_EEOFFSET_R = address & 0x0F;
    return EEPROM_EERDWR_R;
}

void EEPROM_MassErase(void)
{
    EEPROM_EEDONE_R = EEPROM_EEDONE_WORKING;
    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);
}

/* ================= APPLICATION LEVEL ================= */

void EEPROM_SavePassword(char *password, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        EEPROM_Write(EEPROM_PASSWORD_BASE + i, (uint32_t)password[i]);
    }
}

void EEPROM_LoadPassword(char *password, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        password[i] = (char)EEPROM_Read(EEPROM_PASSWORD_BASE + i);
    }
    password[length] = '\0';
}

void EEPROM_SaveAutoLock(uint8_t seconds)
{
    EEPROM_Write(EEPROM_AUTOLOCK_ADDR, seconds);
}

uint8_t EEPROM_LoadAutoLock(void)
{
    return (uint8_t)EEPROM_Read(EEPROM_AUTOLOCK_ADDR);
}
