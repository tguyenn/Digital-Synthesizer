#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <stdint.h>


#define NUM_LEDS          60
#define BITS_PER_LED      24
#define DMA_BUF_SIZE      (NUM_LEDS * BITS_PER_LED)

#define WS_PERIOD         40
#define WS_0              27  
#define WS_1              14  

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} stripLed;

extern stripLed ledArr[NUM_LEDS];
extern uint16_t dmaBuffer[DMA_BUF_SIZE];

void initLedStrip(void);
void modifyLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void prepareDMAData(void);
void pushColors(void);
void pushColors_Blocking(void);
void WS2812B_SendByte(uint8_t);

#endif