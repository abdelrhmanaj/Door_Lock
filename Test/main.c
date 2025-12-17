#include "tm4c123gh6pm.h"
#include "Control_ECU/buzzer.h"
#include "systick.h"

int main(void)
{
    SysTick_Init(16000, SYSTICK_NOINT);  // 1ms tick
    Buzzer_Init();

    Buzzer_On();
    DelayMs(1000);    // 5 seconds

    Buzzer_Off();

    while (1);
}
