#include "../tm4c123gh6pm.h"
#include "HMI_ECU/keypad.h"
#include "HMI_ECU/i2c_lcd.h"
#include "HMI_ECU/../HMI_ECU/../HMI_ECU/adc.h"
#include "HMI_ECU/../HMI_ECU/password.h"
#include "HMI_ECU/leds.h"
#include "../systick.h"

static void show_main_menu(void)
{
    lcd_clear();
    display_message("+ Open Door", 0, 0);
    display_message("- Change Pass", 1, 0);
}

int main(void)
{
    char key;
    uint16_t adc;
    uint8_t seconds;
    char password[PASSWORD_LENGTH + 1];

    SysTick_Init(16000, SYSTICK_NOINT);
    Keypad_Init();
    I2C0_Init();
    lcd_init(0x27);
    ADC_Init();
    init_leds();          // ? ??

    /* ===== Initial Password Setup ===== */
    while (!setup_password(password))
    {
        // retry until password is saved
    }

    /* ===== Main Menu ===== */
    while (1)
    {
        show_main_menu();

        key = 0;
        while (!key)
        {
            key = Keypad_GetKey();
        }

        /* ===== + Open Door ===== */
        if (key == '+')
        {
            lcd_clear();
            display_message("Door Opened", 0, 0);
            led_green_flash(3);
            DelayMs(1000);
        }

        /* ===== - Change Password ===== */
        else if (key == '-')
        {
            lcd_clear();
            display_message("Verify Pass", 0, 0);

            if (verify_password(password))   // ? ??????? ???
            {
                led_red_flash(1);

                while (!setup_password(password))
                {
                    // retry
                }
            }
            else
            {
                lcd_clear();
                display_message("Wrong Password", 0, 0);
                led_red_flash(2);
                DelayMs(1000);
            }
        }

        /* ===== * Auto-Lock Timeout ===== */
        else if (key == '*')
        {
            lcd_clear();
            display_message("Auto Lock Time", 0, 0);

            adc     = ADC_Read();
            seconds = ADC_ToSeconds(adc);

            lcd_set_cursor(1, 0);
            lcd_print_str("Time = ");
            lcd_print_char((seconds / 10) + '0');
            lcd_print_char((seconds % 10) + '0');
            lcd_print_str(" sec");

            DelayMs(5000);   // stay 5 seconds
        }

        // automatically return to main menu
    }
}
