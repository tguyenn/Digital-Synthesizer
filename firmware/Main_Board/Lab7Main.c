// Lab7Main.c
// Runs on MSPM0G3507
// Lab 7 
// Last Modified: January 12, 2026

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"

#include "../lib/myUART.h"

#define sel0 20 // PB20
#define sel1 24 // PB24
#define sel2 3 // PB3
#define sel3 4 // PB4

void PLL_Init(void) { // set phase lock loop (PLL)
  Clock_Init80MHz(0);   // run this line for 80MHz
}

void TIMG7_IRQHandler() {
  GPIOA->DOUTTGL31_0 ^= (1 << 16);
  static uint8_t count = 0;
  if(count > 16) {
    count = 0;
  }
  uint8_t count0 = count & 1;
  uint8_t count1 = (count & (1 << 1)) >> 1;
  uint8_t count2 = (count & (1 << 2)) >> 2;
  uint8_t count3 = (count & (1 << 3)) >> 3;
  GPIOB->DOUTCLR31_0 = (1 << sel0) | (1 << sel1) | (1 << sel2) | (1 << sel3);
  GPIOB->DOUTSET31_0 = (count0 << sel0) | (count1 << sel1) | (count2 << sel2) | (count3 << sel3);
  count++;
}

void initLED(void){
  // PA16, LP RUN LED
  IOMUX->SECCFG.PINCM[PA16INDEX] = 0x00000081;
  GPIOA->DOE31_0 |= 0x10000;

  // init mux selection lines
  // PB24
  // PB20
  // PB4
  // PB3
  GPIOB->DOE31_0 |= (1 << sel0) | (1 << sel1) | (1 << sel2) | (1 << sel3);
  IOMUX->SECCFG.PINCM[PB24INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB20INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB4INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB3INDEX] = 0x00000081;

  TimerG7_IntArm(UINT16_MAX, 0, 0);
}


int main(void){ // main1
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  initLED();
  initUART();
  __enable_irq();
  while(1){
    readUART0();
  }
}