#ifndef UART1_H_
#define UART1_H_

#include <stdint.h>

void UART1_Init(void);
void UART1_EnableRxInterrupt(void);
char UART1_ReadChar(void);

#endif
