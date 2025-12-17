#ifndef KEYPAD_H
#define KEYPAD_H
extern const char keypad_codes[4][4];
/*
 * Initializes the keypad GPIO pins.
 * Must be called before using Keypad_GetKey.
 */
void Keypad_Init(void);

/*
 * Scans the keypad and returns the character of the pressed key.
 * Returns 0 if no key is pressed.
 */
char Keypad_GetKey(void);
#endif // KEYPAD_H