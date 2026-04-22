#include "encoder.h"
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"

volatile int32_t menu_pos = 0;
volatile uint8_t switch_pressed = 0;

void encoderInit(void){
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00060081; // DT (PB16)
    IOMUX->SECCFG.PINCM[PB15INDEX] = 0x00060081; // CLK (PB15)
    IOMUX->SECCFG.PINCM[PB13INDEX] = 0x00060081; // SW (PB13)

    GPIOB->DOE31_0 &= ~((1 << 16) | (1 << 15) | (1 << 13));

    GPIOB->POLARITY15_0 = 0x88000000; 


    GPIOB->CPU_INT.ICLR = (1 << 15) | (1 << 13);  
    GPIOB->CPU_INT.IMASK = (1 << 15) | (1 << 13); 

    NVIC->IP[0] = (NVIC->IP[0] & (~0x0000FF00)) | (2 << 14); 
    NVIC->ISER[0] = (1 << 1);
}

void GROUP1_IRQHandler(void) {
    uint32_t status = GPIOB->CPU_INT.MIS;

    if (status & (1 << 15)) { 
        //delay for debounce
        for(volatile int i=0; i<2000; i++); 
        //recheck CLK
        if (!(GPIOB->DIN31_0 & (1 << 15))) {
            if (GPIOB->DIN31_0 & (1 << 16)) {
                menu_pos++;
            } else {
                menu_pos--;
            }
        }
        GPIOB->CPU_INT.ICLR = (1 << 15);
    }
    
    if (status & (1 << 13)) {
        //delay for debounce
        for(volatile int i=0; i<2000; i++); 
        //recheck SW
        if (!(GPIOB->DIN31_0 & (1 << 13))) {
            switch_pressed = 1;
        }
        GPIOB->CPU_INT.ICLR = (1 << 13);
    }
}