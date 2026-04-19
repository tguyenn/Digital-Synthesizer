#include "../inc/ti_msp_dl_config.h" // dma

void initDMA() {
    SYSCFG_DL_init();
    DL_SYSCTL_disableSleepOnExit();
    NVIC_EnableIRQ(DMA_INT_IRQn);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);


}

void initDMALED(&uint16_t serialLEDArr) {
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gSrcData[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gDstData[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, sizeof(gSrcData) / sizeof(uint32_t));
}

void transferDMALED(void) {
    
    DL_DMA_startTransfer(DMA, DMA_CH0_CHAN_ID);
}
