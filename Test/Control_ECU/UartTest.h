#include <stdio.h>
#include "../tm4c123gh6pm.h"
#include "Control_ECU/uart1.h"

/* print hex number (simple, no stdlib dependency) */
void UART1_PrintHex(uint32_t num)
{
    char hex[] = "0123456789ABCDEF";
    for (int i = 28; i >= 0; i -= 4)
        UART1_SendChar(hex[(num >> i) & 0xF]);
}

int main(void)
{
    char c;

    UART1_Init();

    /* ================= Test 1: Startup ================= */
    UART1_SendString("\r\n============================\r\n");
    UART1_SendString("UART1 REGISTER LEVEL TEST\r\n");
    UART1_SendString("============================\r\n");

    /* ================= Test 2: Register Dump ================= */
    UART1_SendString("\r\n[TEST 2] UART1 REGISTERS\r\n");

    UART1_SendString("UART1_CTL  = 0x");
    UART1_PrintHex(UART1_CTL_R);
    UART1_SendString("\r\n");

    UART1_SendString("UART1_LCRH = 0x");
    UART1_PrintHex(UART1_LCRH_R);
    UART1_SendString("\r\n");

    UART1_SendString("UART1_IBRD = 0x");
    UART1_PrintHex(UART1_IBRD_R);
    UART1_SendString("\r\n");

    UART1_SendString("UART1_FBRD = 0x");
    UART1_PrintHex(UART1_FBRD_R);
    UART1_SendString("\r\n");

    UART1_SendString("UART1_FR   = 0x");
    UART1_PrintHex(UART1_FR_R);
    UART1_SendString("\r\n");

    /* ================= Test 3: TX Test ================= */
    UART1_SendString("\r\n[TEST 3] TX TEST\r\n");
    UART1_SendString("Sending numbers: 0 1 2 3 4 5\r\n");
    UART1_SendString("Sending chars: A B C * #\r\n");

    /* ================= Test 4: RX Polling ================= */
    UART1_SendString("\r\n[TEST 4] RX POLLING TEST\r\n");
    UART1_SendString("Send ONE character...\r\n");

    c = UART1_ReceiveChar();
    UART1_SendString("Received: '");
    UART1_SendChar(c);
    UART1_SendString("' ASCII=");
    UART1_PrintHex(c);
    UART1_SendString("\r\n");

    /* ================= Test 5: Echo Loop ================= */
    UART1_SendString("\r\n[TEST 5] ECHO MODE\r\n");
    UART1_SendString("Type anything, press 'q' to exit\r\n");

    while (1)
    {
        c = UART1_ReceiveChar();

        if (c == 'q')
        {
            UART1_SendString("\r\nExit echo mode\r\n");
            break;
        }

        UART1_SendString("Echo: ");
        UART1_SendChar(c);
        UART1_SendString("\r\n");
    }

    /* ================= Test 6: FIFO Status ================= */
    UART1_SendString("\r\n[TEST 6] FIFO FLAGS\r\n");

    UART1_SendString("RXFE = ");
    UART1_SendChar((UART1_FR_R & UART_FR_RXFE) ? '1' : '0');
    UART1_SendString(" (1 = empty)\r\n");

    UART1_SendString("TXFF = ");
    UART1_SendChar((UART1_FR_R & UART_FR_TXFF) ? '1' : '0');
    UART1_SendString(" (1 = full)\r\n");

    UART1_SendString("\r\nALL TESTS DONE\r\n");

    while (1); // stop here
}
