#include "tm4c123gh6pm.h"
#include "HMI_ECU/keypad.h"


void main(void){
char key;

   Keypad_Init();

   printf("Keypad test started\n");

   while (1)
   {
       key = Keypad_GetKey();
       if (key != 0)
       {
           printf("Pressed key: %c\n", key);
       }
   }
}