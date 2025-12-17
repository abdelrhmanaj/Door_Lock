#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "Control_ECU/uart1.h"
#include "Control_ECU/eeprom.h"

#define PASSWORD_LENGTH 5

int main(void)
{
    char password[PASSWORD_LENGTH + 1];
    uint8_t autoLock;

    UART1_Init();          // UART1 for printf
    EEPROM_Init();         // EEPROM init

    printf("\r\n=== EEPROM READ TEST ===\r\n");

    /* Read from EEPROM */
    EEPROM_LoadPassword(password, PASSWORD_LENGTH);
    autoLock = EEPROM_LoadAutoLock();

    /* Print EEPROM content */
    printf("Stored Password: ");
    for (int i = 0; i < PASSWORD_LENGTH; i++)
    {
        printf("%c", password[i]);
    }
    printf("\r\n");

    printf("Auto-Lock Time: %d seconds\r\n", autoLock);

    printf("========================\r\n");

    while (1);
}
