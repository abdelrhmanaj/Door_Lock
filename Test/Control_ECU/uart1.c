#include "../tm4c123gh6pm.h"
#include "uart1.h"

void UART1_Init(void)
{
    SYSCTL_RCGCUART_R |= (1 << 1);
    SYSCTL_RCGCGPIO_R |= (1 << 1);

    UART1_CTL_R &= ~UART_CTL_UARTEN;

    UART1_IBRD_R = 8;
    UART1_FBRD_R = 44;

    UART1_LCRH_R = UART_LCRH_WLEN_8;

    UART1_ICR_R = 0x7FF;

    UART1_CTL_R = UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE;

    GPIO_PORTB_AFSEL_R |= 0x03;
    GPIO_PORTB_PCTL_R &= ~0xFF;
    GPIO_PORTB_PCTL_R |=  0x11;
    GPIO_PORTB_DEN_R  |= 0x03;
}

void UART1_EnableRxInterrupt(void)
{
    UART1_IM_R = 0;
    UART1_ICR_R = UART_ICR_RXIC;

    UART1_IFLS_R &= ~UART_IFLS_RX_M;
    UART1_IFLS_R |=  UART_IFLS_RX1_8;

    UART1_IM_R |= UART_IM_RXIM;

    NVIC_PRI1_R &= ~(0x7 << 21);   // highest priority
    NVIC_PRI1_R |=  (0x0 << 21);

    NVIC_EN0_R |= (1 << 6);        // UART1 IRQ
}

char UART1_ReadChar(void)
{
    return (char)(UART1_DR_R & 0xFF);
}
