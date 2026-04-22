

#include "dl_config_led_strip.h"

// // stupid ass chat
// /*
//  *  ======== SYSCFG_DL_init ========
//  *  Perform any initialization needed before using any board APIs
//  */
// SYSCONFIG_WEAK void SYSCFG_DL_init(void)
// {
//     SYSCFG_DL_initPower();
//     SYSCFG_DL_GPIO_init();
//     /* Module-Specific Initializations*/
//     SYSCFG_DL_SYSCTL_init();
//     SYSCFG_DL_DMA_init();
// }

// SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
// {
//     DL_GPIO_reset(GPIOA);
//     DL_GPIO_reset(GPIOB);


//     DL_GPIO_enablePower(GPIOA);
//     DL_GPIO_enablePower(GPIOB);

//     delay_cycles(POWER_STARTUP_DELAY);
// }

// SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
// {

//     DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_1_IOMUX);

//     DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_TEST_IOMUX);

//     DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN |
// 		GPIO_LEDS_USER_TEST_PIN);
//     DL_GPIO_enableOutput(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN |
// 		GPIO_LEDS_USER_TEST_PIN);

// }



// SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
// {

// 	//Low Power Mode is configured to be SLEEP0
//     DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    
// 	DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
// 	/* Set default configuration */
// 	DL_SYSCTL_disableHFXT();
// 	DL_SYSCTL_disableSYSPLL();

// }


// static const DL_DMA_Config gDMA_CH0Config = {
//     .transferMode   = DL_DMA_SINGLE_BLOCK_TRANSFER_MODE,
//     .extendedMode   = DL_DMA_NORMAL_MODE,
//     .destIncrement  = DL_DMA_ADDR_INCREMENT,
//     .srcIncrement   = DL_DMA_ADDR_INCREMENT,
//     .destWidth      = DL_DMA_WIDTH_WORD,
//     .srcWidth       = DL_DMA_WIDTH_WORD,
//     .trigger        = DMA_CH0_TRIGGER_SEL_SW,
//     .triggerType    = DL_DMA_TRIGGER_TYPE_EXTERNAL,
// };

// SYSCONFIG_WEAK void SYSCFG_DL_DMA_CH0_init(void)
// {
//     DL_DMA_clearInterruptStatus(DMA, DL_DMA_INTERRUPT_CHANNEL0);
//     DL_DMA_enableInterrupt(DMA, DL_DMA_INTERRUPT_CHANNEL0);
//     DL_DMA_initChannel(DMA, DMA_CH0_CHAN_ID , (DL_DMA_Config *) &gDMA_CH0Config);
// }
// SYSCONFIG_WEAK void SYSCFG_DL_DMA_init(void){
//     SYSCFG_DL_DMA_CH0_init();
// }

// syscfg