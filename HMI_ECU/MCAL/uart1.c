#include "uart1.h"
#include "../CORE/tm4c123gh6pm.h"
#include <stdint.h>


/* =====================================================
 * UART1 Init
 * PB0 -> RX
 * PB1 -> TX
 * Baud = 9600 @ 16 MHz
 * 8N1 – FIFO ENABLED
 * ===================================================== */
void UART1_init(void)
{
    /* Enable clocks */
    SYSCTL_RCGCUART_R |= (1 << 1);   // UART1
    SYSCTL_RCGCGPIO_R |= (1 << 1);   // GPIOB
    while ((SYSCTL_PRGPIO_R & (1 << 1)) == 0);

    /* Disable UART */
    UART1_CTL_R &= ~UART_CTL_UARTEN;

    /* Baud rate: 9600
     * BRD = 16e6 / (16 * 9600) = 104.166
     */
    UART1_IBRD_R = 104;
    UART1_FBRD_R = 11;

    /* 8-bit, no parity, 1 stop bit, FIFO Off */
    UART1_LCRH_R = UART_LCRH_WLEN_8 ;

    /* Enable UART, TX, RX */
    UART1_CTL_R = UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE;

    /* GPIO configuration PB0, PB1 */
    GPIO_PORTB_AFSEL_R |= 0x03;
    GPIO_PORTB_PCTL_R  = (GPIO_PORTB_PCTL_R & ~0xFF) | 0x11;
    GPIO_PORTB_DEN_R  |= 0x03;
    GPIO_PORTB_AMSEL_R &= ~0x03;
}

/* =====================================================
 * Send single byte (blocking)
 * ===================================================== */
void UART1_sendByte(uint8_t data)
{
    while (UART1_FR_R & UART_FR_TXFF);   // TX FIFO full
    UART1_DR_R = data;
}

/* =====================================================
 * Receive single byte (blocking)
 * ===================================================== */
uint8_t UART1_receiveByte(void)
{
    //while (UART1_FR_R & UART_FR_RXFE);   // RX FIFO empty
    return (uint8_t)(UART1_DR_R & 0xFF);
}

/* =====================================================
 * Send null-terminated string
 * Appends '\0' for sync
 * ===================================================== */
void UART1_sendString(const char *str)
{
    while (*str)
        UART1_sendByte((uint8_t)*str++);

    UART1_sendByte('\0');
}

/* =====================================================
 * Receive string until '\0'
 * ===================================================== */
void UART1_receiveString(char *buffer, uint8_t maxLen)
{
    uint8_t i = 0;
    uint8_t c;

    do
    {
        c = UART1_receiveByte();

        if (c != '\0' && i < (maxLen - 1))
            buffer[i++] = (char)c;

    } while (c != '\0');

    buffer[i] = '\0';
}

/* =====================================================
 * Non-blocking data check
 * ===================================================== */
uint8_t UART1_dataAvailable(void)
{
    return ((UART1_FR_R & UART_FR_RXFE) == 0);
}

