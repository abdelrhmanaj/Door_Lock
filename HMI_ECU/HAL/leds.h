#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

void init_leds(void);
void led_red_flash(uint8_t times);
void led_green_flash(uint8_t times);

#endif // LEDS_H
