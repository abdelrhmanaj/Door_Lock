#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

// SysTick modes
#define SYSTICK_INT     1  // Interrupt mode
#define SYSTICK_NOINT   0  // Polling mode (no interrupt)

// Initialize SysTick timer
// reload: reload value for timer (affects timing)
// mode: SYSTICK_INT for interrupt mode, SYSTICK_NOINT for polling mode
void SysTick_Init(uint32_t reload, uint8_t mode);

// Delay in milliseconds using SysTick
// Works by polling COUNT flag or waiting for ticks in interrupt mode
void DelayMs(uint32_t ms);

#endif // SYSTICK_H
