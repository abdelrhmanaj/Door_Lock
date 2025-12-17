#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "Control_ECU/uart1.h"
#include "Control_ECU/motor.h"
#include "Control_ECU/buzzer.h"
#include "systick.h"

/* Flags set by ISR */
volatile uint8_t open_door_req = 0;
volatile uint8_t buzzer_req   = 0;

int main(void)
{
    SysTick_Init(16000, SYSTICK_NOINT); // 1 ms
    UART1_Init();
    UART1_EnableRxInterrupt();

    Motor_Init();
    Buzzer_Init();

    printf("Control ECU Ready\r\n");

    while (1)
    {
        if (open_door_req)
        {
            open_door_req = 0;
            printf("Command received: O (Open Door)\r\n");

            Motor_Right();
            DelayMs(500);
            Motor_Stop();
        }

        if (buzzer_req)
        {
            buzzer_req = 0;
            printf("Command received: B (Buzzer)\r\n");

            Buzzer_On();
            DelayMs(500);
            Buzzer_Off();
        }
    }
}

void UART1_Handler(void)
{
    if (UART1_MIS_R & UART_MIS_RXMIS)
    {
        while (!(UART1_FR_R & UART_FR_RXFE))
        {
            char c = UART1_DR_R & 0xFF;

            if (c == '\r' || c == '\n')
                continue;

            if (c == 'O')
                open_door_req = 1;
            else if (c == 'B')
                buzzer_req = 1;
        }
        UART1_ICR_R = UART_ICR_RXIC;
    }
}
