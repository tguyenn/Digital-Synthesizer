
#include "../lib/ledStrip.h"
#include "../inc/PWM.h"
#include "../lib/DMA.h"

// run through velocity array
// if velocity > some threshold, then trigger led pulse
// TODO write function that scans 


// PWM output pin is PA13

#define BUS_CLK 8
#define DMA_TRANSFER_SIZE_WORDS (1)
#define LED_BUF_SIZE (NUM_LEDS*24) // 60 LEDs * 24 bits each = 1440 bits


stripLed ledBuf[60];
uint16_t serialLEDBuf[LED_BUF_SIZE];

void initLedStrip() {
    initPWM_PA13(BUS_CLK, 0, 40, WS_0); // 32 MHz / 800 kHz = 40
    initDMA();
    initDMALED(serialLEDBuf, LED_BUF_SIZE);
}

// modifies internal buffer of LED state
void modifyLEDBuf(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    ledArr[index].r = r;
    ledArr[index].g = g;
    ledArr[index].b = b;
}

// takes array of stripLed and writes appropriate duty cycle value (CC01 register) for every bit in a uint16_t array
void serializeLEDBuf() {
    // for every LED
        // serialize LED data (8 red, 8 green, 8 blue bits)
    for(int ledIdx = 0; ledIdx < NUM_LEDS; ledIdx++) {
        for(int bitOffset = 0; bitOffset < 8; bitOffset++) { 
        if(ledArr[ledIdx].g & (1 << (7 - bitOffset)))
            serialLEDBuf[ledIdx*24 + bitOffset] = WS_1;
        else 
            serialLEDBuf[ledIdx*24 + bitOffset] = WS_0; 
        }
        for(int bitOffset = 8; bitOffset < 16; bitOffset++) { 
        if(ledArr[ledIdx].r & (1 << (15 - bitOffset)))
            serialLEDBuf[ledIdx*24 + bitOffset] = WS_1;
        else 
            serialLEDBuf[ledIdx*24 + bitOffset] = WS_0;   
        }
        for(int bitOffset = 16; bitOffset < 24; bitOffset++) { 
        if(ledArr[ledIdx].b & (1 << (23 - bitOffset)))
            serialLEDBuf[ledIdx*24 + bitOffset] = WS_1;
        else 
            serialLEDBuf[ledIdx*24 + bitOffset] = WS_0;   
        }
    }
}


// uint16_t ledArr[1440]; // each element represents one bit, which will be output by a DMA timer to bit bang the WS2812B protocol

// dma flow:
// timer with period of 1.25 us counting down
// this timer is used to generate PWM symbols that represents 1's and 0's in the funny serial LED protocol
// to create the 1's and 0's, we must change the duty cycle of this PWM for each bit.
    // need to reload the timer capture compare (CC01) register to change the duty cycle 
    // we can reload this register through a DMA transfer that occurs at the reload of the tiemr
    // we can configure the timer to publish an event to a DMA trigger channel on reset
    // upon trigger, the DMA will move one uint16_t from serialLEDArr to CC01 register and then increment its pointer to the next symbol

void pushColors() {
    transferDMALED(); // todo: change this to just enable DMA
    // todo: hold data line low
    // todo: disable DMA
}

// blocking and bad 😔
// void WS2812B_SendByte(uint8_t byte) {
//     for (int i = 7; i >= 0; i--) {
//         if (byte & (1 << i)) {
//             setPWM_PA13(WS_1); // Using CCP1 (PA13)
//         } else {
//             setPWM_PA13(WS_0); // Using CCP1 (PA13)
//         }
//         // IMPORTANT: You need a way to wait for the timer to finish 
//         // one full cycle before changing the duty cycle again!
//         while(!(TIMG0->CPU_INT.RIS & 0x1)); // Wait for timer zero/load event
//         TIMG0->CPU_INT.ICLR = 0x1;           // Clear flag
//     }
// }

void testLEDStrip(void) {
    for(int i = 0; i < NUM_LEDS; i++) {
        modifyLEDBuf(i, 0x70, 0, 0); // kinda red
    }
    pushColors();
    for(int i = 0; i < NUM_LEDS; i++) {
        modifyLEDBuf(i, 0, 0x70, 0); // kinda blue
    }
    pushColors();
    for(int i = 0; i < NUM_LEDS; i++) {
        modifyLEDBuf(i, 0, 0, 0x70); // kinda green
    }
    pushColors();
}