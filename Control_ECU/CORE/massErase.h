#include "../CORE/tm4c123gh6pm.h"
#include "../MCAL/eeprom.h"
#include "../MCAL/systick.h"

int main(void)
{
    /* Init SysTick (for safety delays if needed) */
    SysTick_Init(16000, SYSTICK_NOINT);

    /* Init EEPROM module */
    EEPROM_Init();

    /* -------- MASS ERASE EEPROM -------- */
    EEPROM_MassErase();

    /* Optional small delay to ensure completion */
    DelayMs(100);

    /* -------- Done -------- */
    while (1)
    {
        /* Stop here – EEPROM is erased */
    }
}
