
#include "password.h"
#include "../HAL/keypad.h"
#include "../HAL/i2c_lcd.h"
#include "../HAL/leds.h"
#include "../MCAL/uart1.h"
#include "../MCAL/systick.h"
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
uint8_t verify_password(void)
{
    char entered_password[PASSWORD_LENGTH + 1];

    lcd_clear();
    display_message("Enter Password:", 0, 0);
    clear_line(1);

    get_password_input_line(entered_password);

    /* Send password to Control ECU */
    UART1_sendByte(CMD_VERIFY_PW);
    UART1_sendString(entered_password);

    DelayMs(200);   // wait response
    char resp = UART1_receiveByte();

    if (resp == RESP_OK)
    {
        lcd_clear();
        display_message("Password OK!", 0, 0);
        led_green_flash(2);
        DelayMs(1000);
        return 1;
    }
    else
    {  
      DelayMs(1000);
      resp = UART1_receiveByte();
      if (resp == 'H')
      {
        lcd_clear();
        display_message("You are 7aramyyyyy", 0, 0);
        led_red_flash(7);
        DelayMs(7000);
      }
      else{
        lcd_clear();
        display_message("Wrong Password", 0, 0);
        led_red_flash(2);
        DelayMs(1000);
       
      }
       return 0;
    }
}
