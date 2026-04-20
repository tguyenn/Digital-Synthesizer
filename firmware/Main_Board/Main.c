// Main.c
#include <stdio.h>
#include <stdint.h>

#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/ST7735.h"
#include "../inc/UART.h"
#include "../lib/myUART.h"
#include "../inc/DAC.h"
#include "../lib/ledStrip.h"
#include "../inc/I2C.h"
#include "../lib/I2CADC.h"

// Include your new abstraction layer
#include "BoardConfig.h"

 ADCoutput_t adc_vals;

const uint16_t Wave16[16] = {
  2048, 2441, 2772, 3004, 3072, 3004, 2772, 2441, 
  2048, 1655, 1324, 1092, 1024, 1092, 1324, 1655
};

void PLL_Init(void) { 
  // Calls the correct clock function based on BoardConfig.h
  INIT_SYS_CLOCK(); 
}

void initHeart(void){
  IOMUX->SECCFG.PINCM[LED_IOMUX] = 0x00000081;
  LED_PORT->DOE31_0 |= (1 << LED_PIN);
  TimerG7_IntArm(UINT16_MAX, 0, 0);
}

// void initMux(void) {
//   MUX_PORT->DOE31_0 |= (1 << MUX_SEL0_PIN) | (1 << MUX_SEL1_PIN) | 
//                        (1 << MUX_SEL2_PIN) | (1 << MUX_SEL3_PIN);
                       
//   IOMUX->SECCFG.PINCM[MUX_SEL0_IOMUX] = 0x00000081;
//   IOMUX->SECCFG.PINCM[MUX_SEL1_IOMUX] = 0x00000081;
//   IOMUX->SECCFG.PINCM[MUX_SEL2_IOMUX] = 0x00000081;
//   IOMUX->SECCFG.PINCM[MUX_SEL3_IOMUX] = 0x00000081;
// }

void initLCD(void) {
  ST7735_InitR(INITR_REDTAB);
}

// void incrementMux(void) {
//   static uint8_t count = 0;
//   if(count > 12) { 
//     count = 0;
//   }
  
//   uint8_t count0 = count & 1;
//   uint8_t count1 = (count & (1 << 1)) >> 1;
//   uint8_t count2 = (count & (1 << 2)) >> 2;
//   uint8_t count3 = (count & (1 << 3)) >> 3;

//   // Clear all 4 mux pins, then set the new state
//   MUX_PORT->DOUTCLR31_0 = (1 << MUX_SEL0_PIN) | (1 << MUX_SEL1_PIN) | 
//                           (1 << MUX_SEL2_PIN) | (1 << MUX_SEL3_PIN);
                          
//   MUX_PORT->DOUTSET31_0 = (count0 << MUX_SEL0_PIN) | (count1 << MUX_SEL1_PIN) | 
//                           (count2 << MUX_SEL2_PIN) | (count3 << MUX_SEL3_PIN);
//   count++;
// }

void toggleHeart(void) {
  LED_PORT->DOUTTGL31_0 = (1 << LED_PIN); 
}

void TIMG6_IRQHandler() {
  static uint16_t i = 0;
  DAC0->DATA0 = Wave16[i++ % 16];
} 

void TIMA1_IRQHandler(){
  adc_vals = updateKeyVals();
}

// void TIMG7_IRQHandler() {
//   incrementMux();
//   toggleHeart();
// }

int main(void){ 
  __disable_irq();
  PLL_Init(); 
  LaunchPad_Init(); 
 // initHeart();
  I2C_Init();
  initADC();
  // initLCD();
  // initUART();
  // DAC_Init();
  // TimerG6_IntArm(10, 0, 0); 
  TimerA1_IntArm(1800, 0, 1);
  __enable_irq();
  while(1){
   // I2C_Recv2(0x48);
  }
}