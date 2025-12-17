#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void ADC_Init(void);
uint16_t ADC_Read(void);

/* convert ADC value to seconds (5–30) */
uint8_t ADC_ToSeconds(uint16_t raw);

#endif /* ADC_H_ */
