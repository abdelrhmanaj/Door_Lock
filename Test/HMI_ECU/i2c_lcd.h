// Simple I2C LCD driver for PCF8574-style I2C backpacks
#ifndef I2C_LCD_H
#define I2C_LCD_H

#include <stdint.h>

// Default I2C address (common values: 0x27 or 0x3F)
#ifndef LCD_I2C_ADDR
#define LCD_I2C_ADDR 0x27
#endif

// PCF8574 bit mapping (correct wiring)
#define LCD_RS        0x01  // P0 - Register Select
#define LCD_EN        0x04  // P2 - Enable
#define LCD_BACKLIGHT 0x08  // P3 - Backlight

// Initialize I2C peripheral and LCD controller on given I2C address
void I2C0_Init(void);
void lcd_init(uint8_t addr);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_print_char(char c);
void lcd_print_str(const char *s);
void lcd_clear(void);
// Detect first responding I2C address in common range (0x20-0x7F)
uint8_t i2c_scan_first(void);
// Probe a single 7-bit I2C address (returns 1 if ACK)
int i2c_probe_addr(uint8_t addr);
// Set/get runtime LCD I2C address
void lcd_set_addr(uint8_t addr);
uint8_t lcd_get_addr(void);
void lcd_set_backlight(uint8_t on);
uint8_t i2c_find_device(void);

// Utility helpers (moved from util.h/c)
void display_message(const char *msg, uint8_t row, uint8_t col);
void clear_line(uint8_t row);

#endif // I2C_LCD_H
