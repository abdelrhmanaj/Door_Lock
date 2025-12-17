#include "../HAL/i2c_lcd.h"
#include "../MCAL/systick.h"
#include "../CORE/tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>

// I2C address of LCD
#define LCD_ADDR        0x27
#define LCD_RS          0x01
#define LCD_EN          0x04
#define LCD_BACKLIGHT   0x08

static uint8_t lcd_backlight = LCD_BACKLIGHT;
// runtime I2C address (can be changed at runtime)
static uint8_t lcd_i2c_addr = LCD_ADDR;

// Initialize I2C0 on PB2 (SCL) PB3 (SDA)
void I2C0_Init(void)
{
    // Enable I2C0 and GPIOB clocks
    SYSCTL_RCGCI2C_R |= 0x01;   // Enable I2C0 clock
    SYSCTL_RCGCGPIO_R |= 0x02;  // Port B
    
    // Wait for clocks to be ready
    for(volatile int i = 0; i < 100000; i++);
    while (!(SYSCTL_PRGPIO_R & 0x02));

    // Configure PB2 (SCL) and PB3 (SDA)
    GPIO_PORTB_AFSEL_R |= (1<<2) | (1<<3);  // Enable alternate function
    GPIO_PORTB_ODR_R |= (1<<3);              // SDA open-drain
    GPIO_PORTB_PUR_R |= (1<<2) | (1<<3);     // Enable pull-ups
    GPIO_PORTB_DEN_R |= (1<<2) | (1<<3);     // Digital enable
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~((0xF<<8) | (0xF<<12))) | (0x3<<8) | (0x3<<12);

    // Enable master function and set baud rate
    I2C0_MCR_R = 0x10;  // Enable I2C master
    I2C0_MTPR_R = 39;   // Set speed to ~50kHz
    
    // Wait for module to stabilize
    for(volatile int i = 0; i < 100000; i++);
}

// Write single byte to I2C slave
static void I2C_WriteByte(uint8_t addr, uint8_t data)
{
    I2C0_MSA_R = (addr << 1) & 0xFE;
    for(volatile int i = 0; i < 100; i++);
    I2C0_MDR_R = data;
    for(volatile int i = 0; i < 100; i++);
    I2C0_MCS_R = 0x07; // START | RUN | STOP
    while (I2C0_MCS_R & 0x01); // wait while BUSY
    for(volatile int i = 0; i < 1000; i++);
}

// Delay helper - now uses SysTick from systick.h

// LCD private helper functions
static void LCD_PulseEnable(uint8_t data)
{
    I2C_WriteByte(lcd_i2c_addr, data | LCD_EN | lcd_backlight);
    DelayMs(1);
    I2C_WriteByte(lcd_i2c_addr, data & ~LCD_EN | lcd_backlight);
    DelayMs(1);
}

static void LCD_Write4Bits(uint8_t data)
{
    I2C_WriteByte(lcd_i2c_addr, data | lcd_backlight);
    LCD_PulseEnable(data);
}

static void LCD_WriteNibble(uint8_t nibble, uint8_t is_data)
{
    uint8_t data = (is_data ? LCD_RS : 0) | (nibble & 0xF0);
    LCD_Write4Bits(data);
}

static void LCD_Command(uint8_t cmd)
{
    LCD_WriteNibble(cmd & 0xF0, 0);         // High nibble
    DelayMs(1);
    LCD_WriteNibble((cmd << 4) & 0xF0, 0);  // Low nibble
    DelayMs(1);
}

static void LCD_Data(uint8_t data)
{
    LCD_WriteNibble(data & 0xF0, 1);        // High nibble
    DelayMs(1);
    LCD_WriteNibble((data << 4) & 0xF0, 1); // Low nibble
    DelayMs(1);
}


void lcd_init(uint8_t addr)
{
    DelayMs(50);
    
    // Special initialization sequence (4-bit mode)
    LCD_WriteNibble(0x03 << 4, 0);
    DelayMs(5);
    LCD_WriteNibble(0x03 << 4, 0);
    DelayMs(5);
    LCD_WriteNibble(0x03 << 4, 0);
    DelayMs(1);
    LCD_WriteNibble(0x02 << 4, 0);
    
    DelayMs(1);
    
    // Configure LCD settings
    LCD_Command(0x28);  // 4-bit mode, 2 lines, 5x8 font
    LCD_Command(0x0C);  // Display on, cursor off, blink off
    LCD_Command(0x06);  // Increment cursor, no display shift
    LCD_Command(0x01);  // Clear display
    DelayMs(2);
}

void lcd_send_cmd(uint8_t cmd)
{
    LCD_Command(cmd);
}

void lcd_send_data(uint8_t data)
{
    LCD_Data(data);
}

void lcd_print_char(char c)
{
    LCD_Data((uint8_t)c);
    DelayMs(5);  // Wait for LCD to process character
}

void lcd_print_str(const char *s)
{
    while (*s)
    {
        LCD_Data(*s++);
        DelayMs(5);  // Wait for LCD to process each character
    }
}

void lcd_clear(void)
{
    LCD_Command(0x01);
    DelayMs(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t row_offsets[] = {0x00, 0x40};
    LCD_Command(0x80 | (col + row_offsets[row]));
}

// lcd_set_addr/lcd_get_addr implemented at end of file (runtime address)

void lcd_set_backlight(uint8_t on)
{
    lcd_backlight = on ? 0x08 : 0x00;
}

uint8_t i2c_scan_first(void)
{
    return LCD_ADDR;
}

uint8_t i2c_find_device(void)
{
    return LCD_ADDR;
}

// Probe a single I2C 7-bit address. Returns 1 if ACK received, 0 otherwise.
int i2c_probe_addr(uint8_t addr)
{
    // Perform a zero-length write to check for ACK
    I2C0_MSA_R = (addr << 1) & 0xFE; // write address
    I2C0_MDR_R = 0x00;
    I2C0_MCS_R = 0x07; // START | RUN | STOP
    while (I2C0_MCS_R & 0x01); // wait while BUSY
    // If ERROR bit (bit 1) is clear -> ACK received
    if ((I2C0_MCS_R & 0x02) == 0)
        return 1;
    return 0;
}

// Utility functions moved from util.c
void display_message(const char *msg, uint8_t row, uint8_t col)
{
    lcd_set_cursor(row, col);
    lcd_print_str(msg);
}

void clear_line(uint8_t row)
{
    lcd_set_cursor(row, 0);
    lcd_print_str("                ");
    lcd_set_cursor(row, 0);
}


void lcd_set_addr(uint8_t addr)
{
    lcd_i2c_addr = addr;
}

uint8_t lcd_get_addr(void)
{
    return lcd_i2c_addr;
}
