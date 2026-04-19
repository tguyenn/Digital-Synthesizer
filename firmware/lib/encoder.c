#include "encoder.h"
#include <ti/devices/msp/msp.h>

// IOMUX Indices for MSPM0G3507
#define PB13INDEX 27
#define PB15INDEX 31
#define PB16INDEX 32

void encoderInit(void){
    // 1. Configure Pin Muxing (PINCM)
    // 0x00060081: Internal Pull-up, Input Buffer Enabled, GPIO Mode
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00060081; // DT (PB16)
    IOMUX->SECCFG.PINCM[PB15INDEX] = 0x00060081; // CLK (PB15)
    IOMUX->SECCFG.PINCM[PB13INDEX] = 0x00060081; // SW (PB13)

    // 2. Set Directions to Input
    GPIOB->DOE31_0 &= ~((1 << 16) | (1 << 15) | (1 << 13));

    // 3. Configure Interrupts (MSPM0 SDK 2.09 CMSIS Naming)
    // IEL (Interrupt Event Level): 0 for level, 1 for edge
    // IPOL (Interrupt Polarity): 0 for Falling/Low, 1 for Rising/High
    
    // Set PB15 (CLK) to Falling Edge
    GPIOB->CPU_INT.IEL |= (1 << 15);   // 1 = Edge triggered
    GPIOB->CPU_INT.IPOL &= ~(1 << 15); // 0 = Falling edge

    // Set PB13 (SW) to Falling Edge
    GPIOB->CPU_INT.IEL |= (1 << 13);   // 1 = Edge triggered
    GPIOB->CPU_INT.IPOL &= ~(1 << 13); // 0 = Falling edge

    // 4. Clear Flags and Unmask
    GPIOB->CPU_INT.ICLR = (1 << 15) | (1 << 13);  
    GPIOB->CPU_INT.IMASK = (1 << 15) | (1 << 13); 

    // 5. Enable Port B Interrupt in NVIC
    // GROUP1 (Port B) is IRQ 1
    NVIC->ISER[0] = (1 << 1); 
}