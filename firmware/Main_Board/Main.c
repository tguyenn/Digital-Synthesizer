// Main.c
#include <stdio.h>
#include <stdint.h>

#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/Timer.h"
// #include "../inc/ADC1.h"
// #include "../inc/ST7735.h"
// #include "../inc/UART.h"
// #include "../lib/myUART.h"
// #include "../inc/DAC.h"
// #include "../lib/ledStrip.h"
// #include "../inc/I2C.h"
// #include "../lib/I2CADC.h"
#include "../inc/ST7735.h"

// Include your new abstraction layer
#include "../lib/ti_msp_dl_config.h"
#include "../lib/Audio_DAC_DMA.h"
#include "BoardConfig.h"

// ADCoutput_t adc_vals;

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

void initLCD(void) {
  // ST7735_InitR(INITR_REDTAB);
}

void toggleHeart(void) {
  LED_PORT->DOUTTGL31_0 = (1 << LED_PIN); 
}

void TIMG7_IRQHandler() {
  toggleHeart();
} 


void TIMA1_IRQHandler(){
  // adc_vals = updateKeyVals();
}

// void TIMG7_IRQHandler() {
//   incrementMux();
//   toggleHeart();
// }


int main(void){ 
  __disable_irq();
  // PLL_Init(); 
  // LaunchPad_Init();
  SYSCFG_DL_init();
  // SYSCFG_DL_DAC12_init();
  // SYSCFG_DL_DMA_init();
 // initHeart();
  // I2C_Init();
  // initADC();
  // initLCD();
  // initUART();
  // DAC_Init();
  // TimerG6_IntArm(10, 0, 0); 
  // TimerA1_IntArm(1800, 0, 1);
  // initHeart();
  // initADC();
  // initLCD();
  // initUART();
  // I2C_Init();
  // DAC_Init();
  // initLedStrip();
  // lmfao();
    // TimerG0_IntArm(UINT16_MAX, 0, 0);
    // ST7735_InitR(INITR_REDTAB);
   // I2C_Recv2(0x48);
  Audio_DAC_DMA_Init();
  __enable_irq();
  while(1){
    // ST7735_FillScreen(ST7735_BLUE);
    for (int i = 0; i < 800000; i++) {
      
    }
    Audio_DAC_DMA_swap();
  }
}