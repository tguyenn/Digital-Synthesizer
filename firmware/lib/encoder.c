#include "encoder.h"
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "display.h"

volatile int32_t menu_pos = 0;
int8_t menu_screen = 0;
uint8_t volume = 0;

volatile uint8_t update_menu = 0;
volatile uint8_t update_screen_type = 0;

void encoderInit(void){
    //ENC 1
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00060081; // DT (PB16)
    IOMUX->SECCFG.PINCM[PB15INDEX] = 0x00060081; // CLK (PB15)
    IOMUX->SECCFG.PINCM[PB13INDEX] = 0x00060081; // SW (PB13)

    //ENC 2
    IOMUX->SECCFG.PINCM[PB2INDEX] = 0x00060081; // DT (PB2)
    IOMUX->SECCFG.PINCM[PB19INDEX] = 0x00060081; // CLK (PB19)
    IOMUX->SECCFG.PINCM[PB18INDEX] = 0x00060081; // SW (PB18) 

    GPIOB->DOE31_0 &= ~((1 << 19) | (1 << 18) | (1 << 16) | (1 << 15) | (1 << 13) | (1 << 2));

    GPIOB->POLARITY15_0 = 0x88000000; 
    GPIOB->POLARITY31_16 = 0x00000080;

    GPIOB->CPU_INT.ICLR = (1 << 19) | (1 << 15) | (1 << 13);  
    GPIOB->CPU_INT.IMASK = (1 << 19) | (1 << 15) | (1 << 13);

    NVIC->IP[0] = (NVIC->IP[0] & (~0x0000FF00)) | (2 << 14); 
    NVIC->ISER[0] = (1 << 1);
}

void GROUP1_IRQHandler(void) {
    uint32_t status = GPIOB->CPU_INT.MIS;

    //ENC 1 CLK triggered interrupt
    if (status & (1 << 15)) { 
        //delay for debounce
        for(volatile int i=0; i<2000; i++); 
        //recheck CLK
        if (!(GPIOB->DIN31_0 & (1 << 15))) {
        if (menu_screen == 0){
            if (GPIOB->DIN31_0 & (1 << 16)) {
                menu_pos--;
            } else {
                menu_pos++;
            }
            update_menu = 1;
        }
        }
        GPIOB->CPU_INT.ICLR = (1 << 15);
    }

    //ENC 2 CLK triggered interrupt
    if (status & (1 << 19)) {
        //delay for debounce
        for(volatile int i=0; i<2000; i++); 
        if (!(GPIOB->DIN31_0 & (1 << 19))) {
            if (GPIOB->DIN31_0 & (1 << 2)) {
                volume--;
            } else {
                volume++;
            }
            if (volume > 20){
                volume = 20;
            }
            if (volume < 0){
                volume = 0;
            }
        }
        GPIOB->CPU_INT.ICLR = (1 << 19);
    }
    
    //ENC 1 SW triggered interrupt
    if (status & (1 << 13)) {
        //delay for debounce
        for(volatile int i=0; i<2000; i++); 
        //recheck SW
        if (!(GPIOB->DIN31_0 & (1 << 13))) {
            if (menu_screen == 1){
                menu_screen = 0;
            }
            else {
            menu_screen = 1;
            }
            update_screen_type = 1;
        }
        GPIOB->CPU_INT.ICLR = (1 << 13);
    }
}
