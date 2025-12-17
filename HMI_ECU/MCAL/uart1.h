#ifndef UART1_H_
#define UART1_H_

#include <stdint.h>

/* ===== Protocol Commands ===== */
#define CMD_REQ_STATUS   'Q'
#define CMD_SAVE_PW      'S'
#define CMD_VERIFY_PW    'V'
#define CMD_OPEN_DOOR    'O'
#define CMD_SET_TIMEOUT  'T'
#define CMD_BUZZER       'B'

#define RESP_OK          'K'
#define RESP_FAIL        'F'

void UART1_init(void);

void UART1_sendByte(uint8_t data);
uint8_t UART1_receiveByte(void);

void UART1_sendString(const char *str);
void UART1_receiveString(char *buffer, uint8_t maxLen);
uint8_t UART1_dataAvailable(void);

#endif
