
#include "../lib/ledStrip.h"
#include "../inc/PWM.c"

// run through velocity array
// if velocity > some threshold, then trigger led pulse
// TODO write function that scans 

// TODO write DMA code that outputs the data buffer ledArr

#define BUS_CLK 8
#define DMA_TRANSFER_SIZE_WORDS (1)

stripLed ledArr[60];

void initLedStrip() {
    initPWM_PA13(BUS_CLK, 0, 40, WS_0); // 32 MHz / 800 kHz = 40
}

// modifies internal buffer of LED state
void modifyLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    ledArr[index].r = r;
    ledArr[index].g = g;
    ledArr[index].b = b;
}

#define LEDBufSize (NUM_LEDS*24) // 60 LEDs * 24 bits each = 1440 bits
uint16_t serialLEDArr[LEDBufSize];
// takes array of stripLed and writes appropriate duty cycle value (CCV register) for every bit in a uint16_t array
void serializeLEDArr() {
    // for every LED
        // serialize LED data (8 red, 8 green, 8 blue bits)
    for(int ledIdx = 0; ledIdx < NUM_LEDS; ledIdx++) {
        for(int bitOffset = 0; bitOffset < 8; bitOffset++) { 
        if(ledArr[ledIdx].g & (1 << (7 - bitOffset)))
            serialLEDArr[ledIdx*24 + bitOffset] = WS_1;
        else 
            serialLEDArr[ledIdx*24 + bitOffset] = WS_0; 
        }
        for(int bitOffset = 8; bitOffset < 16; bitOffset++) { 
        if(ledArr[ledIdx].r & (1 << (15 - bitOffset)))
            serialLEDArr[ledIdx*24 + bitOffset] = WS_1;
        else 
            serialLEDArr[ledIdx*24 + bitOffset] = WS_0;   
        }
        for(int bitOffset = 16; bitOffset < 24; bitOffset++) { 
        if(ledArr[ledIdx].b & (1 << (23 - bitOffset)))
            serialLEDArr[ledIdx*24 + bitOffset] = WS_1;
        else 
            serialLEDArr[ledIdx*24 + bitOffset] = WS_0;   
        }
    }
}


// 60 LEDs, 24 bits each -> 60 * 24 = 1440 bits
// uint16_t ledArr[1440]; // each element represents one bit, which will be output by a DMA timer to bit bang the WS2812B protocol
void pushColors() {
    // todo:
    // DMA serialLEDArr
}


// blocking and bad 😔
void WS2812B_SendByte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        if (byte & (1 << i)) {
            setPWM_PA13(WS_1); // Using CCP1 (PA13)
        } else {
            setPWM_PA13(WS_0); // Using CCP1 (PA13)
        }
        // IMPORTANT: You need a way to wait for the timer to finish 
        // one full cycle before changing the duty cycle again!
        while(!(TIMG0->CPU_INT.RIS & 0x1)); // Wait for timer zero/load event
        TIMG0->CPU_INT.ICLR = 0x1;           // Clear flag
    }
}