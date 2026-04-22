#include "../inc/dl_config_led_strip.h"
// #include "../inc/ti_msp_dl_config.h" // dma
#include "../lib/DMA.h"

#define CC01_REG 0x00001810 // address for capture compare register for PWM
// ?

void initDMA() {
    SYSCFG_DL_init();
    DL_SYSCTL_disableSleepOnExit();
    NVIC_EnableIRQ(DMA_INT_IRQn);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
}

void initDMALED(uint16_t* serialLEDArr, uint32_t transferSize) {
    // 1. Set Source to incrementing RAM address
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) serialLEDArr);
    
    // 2. Set Destination to the fixed Timer CC register
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &TIMG0->COUNTERREGS.CC_01[1]);

    // 3. Configure Transfer size and non-repeating mode
    // transferSize should be (Number of LEDs * 24)
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, transferSize);
    
    // Each trigger moves one 16-bit PWM value, stops after transferSize moves
    DL_DMA_setTransferMode(DMA, DMA_CH0_CHAN_ID, DL_DMA_SINGLE_TRANSFER_MODE);

    // 4. Set Trigger to listen to Subscriber Port 1 (Event Channel 1)
    // Using the DriverLib define for Subscriber Channels is safer
    // DL_DMA_setTrigger(DMA, DMA_CH0_CHAN_ID, DL_DMA_TRIG_SUB_CH_1, DMA_DMATCTL_DMATINT_EXTERNAL);

    // 5. Enable the DMA Channel
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
}

void transferDMALED() {
    DL_DMA_startTransfer(DMA, DMA_CH0_CHAN_ID);
}
