#include "tm4c123gh6pm.h"
#include "Control_ECU/motor.h"
#include "systick.h"

int main(void)
{
    SysTick_Init(16000, SYSTICK_NOINT);  // 1 ms tick
    Motor_Init();

    /* Rotate Right */
    Motor_Right();
    DelayMs(8000);      // ? ?????? SysTick ?????

    Motor_Stop();
    DelayMs(500);

    /* Rotate Left */
    Motor_Left();

    while (1);
}
