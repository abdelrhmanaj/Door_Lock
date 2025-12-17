#ifndef PASSWORD_H_
#define PASSWORD_H_

#include <stdint.h>

#define PASSWORD_LENGTH 5

void get_password_input_line(char *password_buffer);
uint8_t setup_password(char *out_password);
uint8_t verify_password(const char *stored_password);

#endif
