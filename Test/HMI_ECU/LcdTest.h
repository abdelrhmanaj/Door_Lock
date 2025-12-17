#include "tm4c123gh6pm.h"
#include "HMI_ECU/../HMI_ECU/keypad.h"
#include "HMI_ECU/i2c_lcd.h"
#include "systick.h"

int main(void)
{
    char key;

    SysTick_Init(16000, SYSTICK_NOINT);      // ?? systick.h
    I2C0_Init();         // I2C for LCD
    lcd_init(0x27);      // LCD I2C address
    Keypad_Init();       // Keypad init

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print_str("Keypad Test");

    lcd_set_cursor(1, 0);
    lcd_print_str("Key: ");

    while (1)
    {
        key = Keypad_GetKey();

        if (key != 0)
        {
            lcd_set_cursor(1, 5);   // ??? "Key: "
            lcd_print_char(key);
        }
    }
}
