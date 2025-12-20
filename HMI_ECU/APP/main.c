
#include "../CORE/tm4c123gh6pm.h"
#include "../HAL/keypad.h"
#include "../HAl/i2c_lcd.h"
#include "../CORE/password.h"
#include "../MCAL/uart1.h"
#include "../HAL/leds.h"
#include "../MCAL/systick.h"
#include "../HAL/adc.h"
#include <stdint.h>
#include <stdio.h>

/* =====================================================
 * Show Main Menu
 * ===================================================== */
static char show_main_menu(void)
{
    lcd_clear();
    display_message("F1 Open Door", 0, 0);
    display_message("F2 Change PW", 1, 0);
    DelayMs(1200);

    lcd_clear();
    display_message("F3 Set Timeout", 0, 0);
    display_message("F1 F2 F3 Select", 1, 0);

    while (1)
    {
        char k = Keypad_GetKey();
        if (k == 'A' || k == 'B' || k == 'C')
            return k;
    }
}

/* =====================================================
 * OPEN DOOR
 * ===================================================== */
static void open_door(void)
{
    char pw[PASSWORD_LENGTH + 1];
    uint8_t attempts = 0;
    uint16_t adc; 
        uint8_t sec; 
        adc= ADC_Read(); 
        sec = ADC_ToSeconds(adc);

    while (attempts < 3)
    {
        lcd_clear();
        display_message("Enter Password:", 0, 0);
        clear_line(1);
        get_password_input_line(pw);

        UART1_sendByte(CMD_VERIFY_PW);
        for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
            UART1_sendByte(pw[i]);
         lcd_clear();
         display_message("Checking", 0, 0);
         DelayMs(2000);

        if (UART1_receiveByte() == RESP_OK)
        {
            lcd_clear();
            display_message("Door Opening", 0, 0);
            UART1_sendByte(CMD_OPEN_DOOR);
            led_green_flash(7);
            DelayMs(1000*(sec-5));
            lcd_clear();
            return;
        }

        attempts++;
        lcd_clear();
        display_message("Wrong Password", 0, 0);
        led_red_flash(1);
        DelayMs(800);
    }

    UART1_sendByte(CMD_BUZZER);
    DelayMs(1000);
    lcd_clear();
    display_message("LOCKED!", 0, 0);
    led_red_flash(5);
    DelayMs(500);
}

/* =====================================================
 * CHANGE PASSWORD
 * ===================================================== */
static void change_password(void)
{
    char old_pw[PASSWORD_LENGTH + 1];

    lcd_clear();
    display_message("Old Password:", 0, 0);
    clear_line(1);
    get_password_input_line(old_pw);

    UART1_sendByte(CMD_VERIFY_PW);
    DelayMs(100);
    for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
        UART1_sendByte(old_pw[i]);
    
      DelayMs(200);
    if (UART1_receiveByte() != RESP_OK)
    {
        lcd_clear();
        display_message("Wrong Password", 0, 0);
        led_red_flash(2);
        DelayMs(1500);
        return;
    }

    char new_pw[PASSWORD_LENGTH + 1];
    while (!setup_password(new_pw));

    UART1_sendByte(CMD_SAVE_PW);
    DelayMs(100);
    for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
        UART1_sendByte(new_pw[i]);
       DelayMs(500);
    if (UART1_receiveByte() == RESP_OK)
    {
        lcd_clear();
        display_message("Password Updated", 0, 0);
        led_green_flash(2);
    }
    else
    {
        lcd_clear();
        display_message("Save Failed", 0, 0);
        led_red_flash(3);
    }

    DelayMs(1500);
}

/* =====================================================
 * SET AUTO LOCK TIME
 * ===================================================== */
static void set_timeout(void)
{
    lcd_clear();
    display_message("Adjust Timeout", 0, 0);

    while (1)
    {
        uint16_t adc; 
        uint8_t sec; 
        adc= ADC_Read(); 
        sec = ADC_ToSeconds(adc);
        char buf[16];

        lcd_set_cursor(1, 0);
        snprintf(buf, sizeof(buf), "Time: %2us", sec);
        lcd_print_str(buf);

        char k = Keypad_GetKey();
        if (k == 'D')
        {
            UART1_sendByte(CMD_SET_TIMEOUT);
            DelayMs(100);
            UART1_sendByte(sec);
            DelayMs(300);
            if (UART1_receiveByte() == RESP_OK)
            {
                lcd_clear();
                display_message("Saved!", 0, 0);
                led_green_flash(1);
            }
            else
            {
                lcd_clear();
                display_message("Save Failed", 0, 0);
                led_red_flash(2);
            }

            DelayMs(1200);
            return;
        }

        DelayMs(150);
    }
}

/* =====================================================
 * MAIN
 * ===================================================== */
int main(void)
{
    /* -------- Init hardware -------- */
    SysTick_Init(16000, SYSTICK_NOINT);
    Keypad_Init();
    I2C0_Init();
    UART1_init();      // UART WITHOUT FIFO
    ADC_Init();
    init_leds();

    lcd_init(0x27);
    lcd_clear();

    /* -------- Check first use -------- */
    UART1_sendByte(CMD_REQ_STATUS);
    DelayMs(500);
    if (UART1_receiveByte() == 'N')
    {
        char pw[PASSWORD_LENGTH + 1];
        while (!setup_password(pw));

        UART1_sendByte(CMD_SAVE_PW);
        for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
            UART1_sendByte(pw[i]);
         DelayMs(500);
        if (UART1_receiveByte() == RESP_OK)
        {
            lcd_clear();
            display_message("Password Saved", 0, 0);
            led_green_flash(2);
        }
        else
        {
            lcd_clear();
            display_message("Save Failed", 0, 0);
            led_red_flash(3);
        }

        DelayMs(1500);
    }

    /* -------- Login -------- */
    else if(UART1_receiveByte() != 'Y')
    {
      display_message("Control_ECU Not", 0, 0);
      display_message("response", 1, 0);
      while(1);
    }
    lcd_clear();
    display_message("System Ready", 0, 0);
    DelayMs(1200);

    /* -------- Main Loop -------- */
    while (1)
    {
        char option = show_main_menu();

        if (option == 'A')
            open_door();
        else if (option == 'B')
            change_password();
        else if (option == 'C')
            set_timeout();
    }
    
}
