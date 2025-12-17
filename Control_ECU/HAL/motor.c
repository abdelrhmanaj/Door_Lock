#include "motor.h"
#include "../CORE/tm4c123gh6pm.h"

#define IN1 (1 << 1)   // PF1
#define IN2 (1 << 2)   // PF2

void Motor_Init(void)
{
    SYSCTL_RCGCGPIO_R |= (1 << 5);   // Port F
    while ((SYSCTL_PRGPIO_R & (1 << 5)) == 0);

    GPIO_PORTF_DIR_R |= (IN1 | IN2);
    GPIO_PORTF_DEN_R |= (IN1 | IN2);

    GPIO_PORTF_DATA_R &= ~(IN1 | IN2); // Stop motor
}

void Motor_Right(void)
{
    GPIO_PORTF_DATA_R |= IN1;
    GPIO_PORTF_DATA_R &= ~IN2;
}

void Motor_Left(void)
{
    GPIO_PORTF_DATA_R |= IN2;
    GPIO_PORTF_DATA_R &= ~IN1;
}

void Motor_Stop(void)
{
    GPIO_PORTF_DATA_R &= ~(IN1 | IN2);
}
