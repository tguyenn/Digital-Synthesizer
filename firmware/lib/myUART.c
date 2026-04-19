#include "../inc/UART.h"
#include "../inc/UART1.h"
#include "../lib/myUART.h"

#define NUM_KEYS 60

void initUART() {
    UART_Init(); // UART0: chip -> LP comms
    UART1_Init(); // UART1: to laptop via USB to UART bridge
}

void testUART1() {
    UART1_OutChar('L');
    UART1_OutChar('b');
    UART1_OutChar('o');
    UART1_OutChar('z');
    UART1_OutChar('o');
}

void testUART0() {
    UART_OutChar('L');
    UART_OutChar('b');
    UART_OutChar('o');
    UART_OutChar('z');
    UART_OutChar('o');
}

int16_t keyVel[NUM_KEYS];
void pushVelData() {
    for(int i = 0; i < NUM_KEYS; i++) {
        UART_OutChar(keyVel[i]);
    }
}

int16_t keyVel[NUM_KEYS]; 
int16_t oldPos[NUM_KEYS]; 
int16_t newPos[NUM_KEYS]; 

void calcVel() {
    for(int i = 0; i < NUM_KEYS; i++) {
        keyVel[i] = (oldPos[i] - newPos[i]);
        // if we assume a constant sampling rate of each hall sensor, then we do not need to divide by time. 
        // we can use the raw magnitude and direction of keyVel[i] to determine how to play sound 
        // maybe slave chip can use history of keyVel ?

        // we should prob do some sort of filtering tbh, but we might need to collect more position data

        // how do we encode the velocity data in the most compact way possible? UART frames are 8 bits wide, but the "velocity"
        // will be 16 bits wide -> 
    }
}

// todo move this to main??
int16_t newPos[NUM_KEYS];
int16_t oldPos[NUM_KEYS];

void readADCData() {
    for(int i = 0; i < NUM_KEYS; i++) {
        oldPos[i] = newPos[i]; // is there a better way to implement w/o so many memory accesses?
        // newPos[i] = readADC();
    }
}

// main loop / ISR(?) logic
// while(ADCCount < 60) {
//     readADCData();
//     ADCCount++;
// }
// calcVel();
// pushVelData();
// ADCCount = 0;

// filtering???
