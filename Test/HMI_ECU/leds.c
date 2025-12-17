#include "leds.h"
#include "../tm4c123gh6pm.h"

#define LED_RED   (1 << 1)
#define LED_GREEN (1 << 3)

void init_leds(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20; // Port F
    for(volatile int i = 0; i < 100000; i++);
    GPIO_PORTF_DIR_R |= (LED_RED | LED_GREEN);
    GPIO_PORTF_DEN_R |= (LED_RED | LED_GREEN);
    GPIO_PORTF_DATA_R = 0;
}

void led_red_flash(uint8_t times)
{
    for(uint8_t i = 0; i < times; i++)
    {
        GPIO_PORTF_DATA_R = LED_RED;
        for(volatile int j = 0; j < 1000000; j++);
        GPIO_PORTF_DATA_R = 0;
        for(volatile int j = 0; j < 1000000; j++);
    }
}

void led_green_flash(uint8_t times)
{
    for(uint8_t i = 0; i < times; i++)
    {
        GPIO_PORTF_DATA_R = LED_GREEN;
        for(volatile int j = 0; j < 1000000; j++);
        GPIO_PORTF_DATA_R = 0;
        for(volatile int j = 0; j < 1000000; j++);
    }
}
