#include "password.h"
#include "keypad.h"
#include "i2c_lcd.h"
#include "leds.h"
#include "../systick.h"
#include <string.h>

/* ===== Helper: get password input ===== */
void get_password_input_line(char *password_buffer)
{
    uint8_t index = 0;
    char key;

    while (index < PASSWORD_LENGTH)
    {
        key = Keypad_GetKey();
        if (key)
        {
            if (key >= '0' && key <= '9')
            {
                password_buffer[index++] = key;
                lcd_print_char('*');
                DelayMs(12);   // debounce
            }
            else if (key == '*' && index > 0)
            {
                index--;
                lcd_send_cmd(0x10); // cursor left
                lcd_print_char(' ');
                lcd_send_cmd(0x10);
                DelayMs(50);
            }
        }
    }
    password_buffer[PASSWORD_LENGTH] = '\0';
}

/* ===== Setup Password (LOCAL) ===== */
uint8_t setup_password(char *out_password)
{
    if (!out_password) return 0;

    char password1[PASSWORD_LENGTH + 1];
    char password2[PASSWORD_LENGTH + 1];

    lcd_clear();
    display_message("CREATE PASSWORD", 0, 0);
    display_message("Enter digits", 1, 0);
    DelayMs(1500);

    clear_line(1);
    get_password_input_line(password1);

    lcd_clear();
    display_message("CONFIRM PASSWORD", 0, 0);
    display_message("Enter digits", 1, 0);
    DelayMs(1500);

    clear_line(1);
    get_password_input_line(password2);

    if (strcmp(password1, password2) == 0)
    {
        strcpy(out_password, password1);

        lcd_clear();
        display_message("Password Saved!", 0, 0);
        display_message("Setup Complete", 1, 0);
        led_green_flash(3);
        DelayMs(2000);
        return 1;
    }
    else
    {
        lcd_clear();
        display_message("Passwords DON'T", 0, 0);
        display_message("Match! Try Again", 1, 0);
        led_red_flash(3);
        DelayMs(1500);
        return 0;
    }
}

/* ===== Verify Password (LOCAL) ===== */
uint8_t verify_password(const char *stored_password)
{
    char entered_password[PASSWORD_LENGTH + 1];

    lcd_clear();
    display_message("Enter Password:", 0, 0);
    clear_line(1);

    get_password_input_line(entered_password);

    if (strcmp(entered_password, stored_password) == 0)
    {
        lcd_clear();
        display_message("Password OK!", 0, 0);
        led_green_flash(2);
        DelayMs(1000);
        return 1;
    }
    else
    {
        lcd_clear();
        display_message("Wrong Password", 0, 0);
        led_red_flash(2);
        DelayMs(1000);
        return 0;
    }
}
