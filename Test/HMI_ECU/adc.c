#include "../tm4c123gh6pm.h"
#include "adc.h"

void ADC_Init(void)
{
    /***********************
     * Enable clocks
     ***********************/
    SYSCTL_RCGCGPIO_R |= 0x02;    // Enable Port B clock
    SYSCTL_RCGCADC_R  |= 0x01;    // Enable ADC0 clock

    /***********************
     * Configure PB4 as AIN10
     ***********************/
    GPIO_PORTB_AFSEL_R |= (1 << 4);   // Enable alternate function
    GPIO_PORTB_DEN_R   &= ~(1 << 4);  // Disable digital
    GPIO_PORTB_AMSEL_R |= (1 << 4);   // Enable analog mode

    /***********************
     * Configure ADC
     ***********************/
    ADC0_ACTSS_R &= ~0x08;        // Disable SS3 for configuration
    ADC0_EMUX_R  &= ~0xF000;      // SS3 trigger = software
    ADC0_SSMUX3_R = 10;           // Use AIN10 (PB4)
    ADC0_SSCTL3_R = 0x06;         // IE0 = 1, END0 = 1
    ADC0_ACTSS_R |= 0x08;         // Enable sequencer 3
}

uint16_t ADC_Read(void)
{
    ADC0_PSSI_R |= 0x08;              // Start conversion SS3
    while ((ADC0_RIS_R & 0x08) == 0); // Wait until done
    uint16_t value = ADC0_SSFIFO3_R & 0xFFF;
    ADC0_ISC_R = 0x08;                // Clear flag
    return value;
}

/***************************************
 * Convert ADC (0–4095) to time 5–30 sec
 * Step = 1 second
 ***************************************/
uint8_t ADC_ReadSeconds(void)
{
    uint16_t raw = ADC_Read();

    // Scale to 5–30 seconds
    uint8_t seconds = (raw * 25 / 4095) + 5;

    return seconds;
}
