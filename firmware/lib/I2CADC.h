#ifndef I2CADC_H
#define I2CADC_H

#include <stdint.h>

void initADC(void);
void initMux(void);
uint16_t readADC(int8_t address);
 



#endif