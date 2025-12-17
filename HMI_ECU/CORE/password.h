#ifndef PASSWORD_H
#define PASSWORD_H

#include <stdint.h>

// Number of digits in the system password. Change here to adjust UI and protocol.
#ifndef PASSWORD_LENGTH
#define PASSWORD_LENGTH 5
#endif

void get_password_input_line(char *password_buffer);
uint8_t setup_password(char *out_password);
uint8_t verify_password();

#endif // PASSWORD_H
