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


void PLL_Init(void) { // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

void TIMG7_IRQHandler() {
  GPIOA->DOUTTGL31_0 ^= (1 << 16);
}


void initLED(void){
  // PA16, LP
  IOMUX->SECCFG.PINCM[PA16INDEX] = 0x00000081;
  GPIOA->DOE31_0 |= 0x10000;
  initUART();

  TimerG7_IntArm(UINT16_MAX, 0, 0);
}


int main(void){ // main1
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  initLED();
  __enable_irq();
  while(1){
    
  }
}