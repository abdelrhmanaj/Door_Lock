#include "buzzer.h"
#include "../tm4c123gh6pm.h"

#define BUZZER_PIN (1 << 3)   // PF3

void Buzzer_Init(void)
{
    SYSCTL_RCGCGPIO_R |= (1 << 5);   // Port F
    while ((SYSCTL_PRGPIO_R & (1 << 5)) == 0);

    GPIO_PORTF_DIR_R |= BUZZER_PIN;  // Output
    GPIO_PORTF_DEN_R |= BUZZER_PIN;

    GPIO_PORTF_DATA_R &= ~BUZZER_PIN; // OFF
}

void Buzzer_On(void)
{
    GPIO_PORTF_DATA_R |= BUZZER_PIN;
}

void Buzzer_Off(void)
{
    GPIO_PORTF_DATA_R &= ~BUZZER_PIN;
}
