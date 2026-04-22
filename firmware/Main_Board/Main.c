// Main.c
#include <stdint.h>
#include <stdio.h>


#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/Timer.h"
// #include "../inc/ADC1.h"
// #include "../inc/ST7735.h"
// #include "../inc/UART.h"
// #include "../lib/myUART.h"
// #include "../lib/ledStrip.h"
// #include "../inc/I2C.h"
// #include "../lib/I2CADC.h"
#include "../inc/DAC.h"
#include "../inc/ST7735.h"
#include "../lib/encoder.h"


#include "../lib/Audio_DAC_DMA.h"
#include "../lib/ti_msp_dl_config.h"


#include "BoardConfig.h"

// ADCoutput_t adc_vals;

void PLL_Init(void) { 
  // Calls the correct clock function based on BoardConfig.h
  INIT_SYS_CLOCK();
}

void initHeart(void) {
  IOMUX->SECCFG.PINCM[LED_IOMUX] = 0x00000081;
  LED_PORT->DOE31_0 |= (1 << LED_PIN);
  TimerG0_IntArm(UINT16_MAX, 0, 0);
}

void toggleHeart(void) { LED_PORT->DOUTTGL31_0 = (1 << LED_PIN); }

void TIMG0_IRQHandler() {
  toggleHeart();
} 

void TIMA1_IRQHandler() {
  // adc_vals = updateKeyVals();
}

volatile int pingorpong = -1;

/*// pls stop merge conflict 😔
// int main(void) {
//   __disable_irq();
//   PLL_Init();
//   LaunchPad_Init();
//   SYSCFG_DL_init();
//   // DAC_Init();
//   encoderInit();
//   // SYSCFG_DL_DAC12_init();
//   // SYSCFG_DL_DMA_init();
//   // initHeart();
//   // initADC();
//   // initLCD();
//   // initUART();
//   // TimerG6_IntArm(10, 0, 0);  
//   // TimerA1_IntArm(1800, 0, 1);
//   // I2C_Init();
//   // initLedStrip();
//   setupPingPongDMA();
//   TimerG0_IntArm(UINT16_MAX, 0, 0);
//   __enable_irq();
//   while (1) {

//     // Audio_DAC_DMA_Init();
//     __enable_irq();
//     while (1) {
//       // ST7735_FillScreen(ST7735_BLUE);
//       // for (int i = 0; i < 800000; i++) {

//       // }
//       // Audio_DAC_DMA_swap();
//     }
//   }
// }
*/

int main(void) {
  
  
  while(1) {
  __disable_irq();
  PLL_Init();
  LaunchPad_Init();
    
  __enable_irq();
  }
}

int main2(void) {
  
  
  while(1) {
  __disable_irq();
  PLL_Init();
  LaunchPad_Init();
    
  __enable_irq();
  }
}