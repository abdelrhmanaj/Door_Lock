#include "../CORE/tm4c123gh6pm.h"
#include "../MCAL/uart1.h"
#include "../MCAL/eeprom.h"
#include "../HAL/motor.h"
#include "../HAL/buzzer.h"
#include "../MCAL/systick.h"
#include <stdint.h>

/* =====================================================
 * EEPROM layout
 * ===================================================== */
#define EEPROM_FLAG_BLOCK      0
#define EEPROM_FLAG_OFFSET     0

#define EEPROM_PW_BLOCK        0
#define EEPROM_PW_OFFSET       1

#define EEPROM_TIMEOUT_BLOCK   0
#define EEPROM_TIMEOUT_OFFSET  3

#define PASSWORD_FLAG          0xA5
#define DEFAULT_TIMEOUT        10

#define MAX_PW_LEN             5

/* ===================================================== */
static uint8_t auto_lock_time = DEFAULT_TIMEOUT;


/* =====================================================
 * Helpers
 * ===================================================== */
static uint8_t password_exists(void)
{
    uint32_t flag;
    EEPROM_ReadWord(EEPROM_FLAG_BLOCK, EEPROM_FLAG_OFFSET, &flag);
    return (flag == PASSWORD_FLAG);
}

static void mark_password_saved(void)
{
    EEPROM_WriteWord(EEPROM_FLAG_BLOCK, EEPROM_FLAG_OFFSET, PASSWORD_FLAG);
}

static uint8_t check_password(uint8_t *pw)
{
    uint8_t stored_pw[8];
    EEPROM_ReadBuffer(EEPROM_PW_BLOCK, EEPROM_PW_OFFSET, stored_pw, 8);

    for (uint8_t i = 0; i < MAX_PW_LEN; i++)
    {
        if (pw[i] != stored_pw[i])
            return 0;
    }
    return 1;
}

/* =====================================================
 * MAIN
 * ===================================================== */
int main(void)
{
    uint8_t cmd;
    uint8_t pw_buf[8] = {0};
    uint8_t i;
    uint32_t tmp;

    /* -------- Init hardware -------- */
    SysTick_Init(16000, SYSTICK_NOINT);
    UART1_init();
    EEPROM_Init();
    Motor_Init();
    Buzzer_Init();

    /* -------- Load auto-lock timeout -------- */
    if (EEPROM_ReadWord(EEPROM_TIMEOUT_BLOCK,
                         EEPROM_TIMEOUT_OFFSET,
                         &tmp) == EEPROM_SUCCESS)
    {
        auto_lock_time = (uint8_t)tmp;
        if (auto_lock_time < 5 || auto_lock_time > 30)
            auto_lock_time = DEFAULT_TIMEOUT;
    }

    /* -------- Main loop -------- */
    while (1)
    {
        /* Wait for command */
        if (!UART1_dataAvailable())
            continue;

        cmd = UART1_receiveByte();

        /* ===== REQUEST STATUS ===== */
        if (cmd == CMD_REQ_STATUS)
        {
            UART1_sendByte(password_exists() ? 'Y' : 'N');
        }

        /* ===== SAVE PASSWORD ===== */
        else if (cmd == CMD_SAVE_PW)
        {
            for (i = 0; i < MAX_PW_LEN; i++)
                pw_buf[i] = UART1_receiveByte();

            pw_buf[5] = pw_buf[6] = pw_buf[7] = 0;

            EEPROM_WriteBuffer(EEPROM_PW_BLOCK,
                               EEPROM_PW_OFFSET,
                               pw_buf,
                               8);

            mark_password_saved();
           
            UART1_sendByte(RESP_OK);
        }

        /* ===== VERIFY PASSWORD ===== */
        else if (cmd == CMD_VERIFY_PW)
        {
            for (i = 0; i < MAX_PW_LEN; i++)
                pw_buf[i] = UART1_receiveByte();

            if (check_password(pw_buf))
            {
                
                UART1_sendByte(RESP_OK);
            }
            else
            {
               
                UART1_sendByte(RESP_FAIL);

               
            }
        }

        /* ===== OPEN DOOR ===== */
        else if (cmd == CMD_OPEN_DOOR)
        {
            Motor_Right();
            DelayMs(2000);
            Motor_Stop();

            DelayMs(auto_lock_time * 1000);

            Motor_Left();
            DelayMs(2000);
            Motor_Stop();
        }

        /* ===== SET AUTO-LOCK TIME ===== */
        else if (cmd == CMD_SET_TIMEOUT)
        {
            auto_lock_time = UART1_receiveByte();

            if (auto_lock_time < 5) auto_lock_time = 5;
            else if (auto_lock_time > 30) auto_lock_time = 30;

            EEPROM_WriteWord(EEPROM_TIMEOUT_BLOCK,
                             EEPROM_TIMEOUT_OFFSET,
                             auto_lock_time);

            UART1_sendByte(RESP_OK);
        }

        /* ===== BUZZER ===== */
        else if (cmd == CMD_BUZZER)
        {
            Buzzer_On();
            DelayMs(8000);
            Buzzer_Off();
        }
    }
}
