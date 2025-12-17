#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "HMI_ECU/adc.h"

int main(void)
{
    uint16_t adc_value;
    uint8_t seconds;

    ADC_Init();

    printf("ADC TEST STARTED\n");

    while (1)
    {
        adc_value = ADC_Read();
        seconds   = ADC_ToSeconds(adc_value);

        printf("ADC = %4d | Time = %2d sec\n", adc_value, seconds);

        // simple delay (software)
        for (volatile int i = 0; i < 2000000; i++);
    }
}
