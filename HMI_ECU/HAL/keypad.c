#include "../CORE/tm4c123gh6pm.h"
#include "keypad.h"

void Keypad_Init(void)
{
    // Enable clocks for Port D and Port E
    SYSCTL_RCGCGPIO_R |= 0x18;   // Bit 3 = Port D, Bit 4 = Port E

    // Rows: PD0-PD3 ? Output
    GPIO_PORTD_DIR_R |= 0x0F;
    GPIO_PORTD_DEN_R |= 0x0F;

    // Columns: PE0-PE3 ? Input with pull-up
    GPIO_PORTE_DIR_R &= ~0x0F;
    GPIO_PORTE_DEN_R |= 0x0F;
    GPIO_PORTE_PUR_R |= 0x0F;  // Enable pull-ups
}
char Keypad_GetKey(void)
{
    const char keys[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };

    for (int row = 0; row < 4; row++)
    {
        // Drive current row LOW, others HIGH
        GPIO_PORTD_DATA_R = (~(1 << row)) & 0x0F;  // drive one row LOW
        // Small delay for signal to settle
        for (volatile int d = 0; d < 5000; d++);
        for (int col = 0; col < 4; col++)
        {
            if ((GPIO_PORTE_DATA_R & (1 << col)) == 0)  // column goes LOW?
            {
                // Debounce: wait a short time and confirm still pressed
                for (volatile int dd = 0; dd < 20000; dd++);
                if ((GPIO_PORTE_DATA_R & (1 << col)) != 0) // bounced
                    continue;

                // Wait for key release (with small delays to avoid tight spin)
                while ((GPIO_PORTE_DATA_R & (1 << col)) == 0)
                {
                    for (volatile int w = 0; w < 5000; w++);
                }

                // Small post-release delay
                for (volatile int pd = 0; pd < 5000; pd++);
                return keys[row][col];
            }
        }
    }

    return 0; // no key pressed
}
