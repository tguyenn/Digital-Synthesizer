// BoardConfig.h
#ifndef BOARDCONFIG_H
#define BOARDCONFIG_H

#include <ti/devices/msp/msp.h>

// =========================================================
// MASTER SWITCH: Uncomment the target you are compiling for
// =========================================================
#define TARGET_LAUNCHPAD
// #define TARGET_CUSTOM_CHIP

// =========================================================
// CONFIGURATION PROFILES
// =========================================================

#ifdef TARGET_LAUNCHPAD

    // --- Clock Setup ---
    #define INIT_SYS_CLOCK()    Clock_Init80MHz(0)

    // --- Mux Pins (GPIOB) ---
    #define MUX_PORT            GPIOB
    #define MUX_SEL0_PIN        20   // PB20
    #define MUX_SEL1_PIN        24   // PB24
    #define MUX_SEL2_PIN        3    // PB3
    #define MUX_SEL3_PIN        4    // PB4

    #define MUX_SEL0_IOMUX      PB20INDEX
    #define MUX_SEL1_IOMUX      PB24INDEX
    #define MUX_SEL2_IOMUX      PB3INDEX
    #define MUX_SEL3_IOMUX      PB4INDEX

    // --- Heartbeat LED (GPIOA) ---
    #define LED_PORT            GPIOA
    #define LED_PIN             16   // PA16
    #define LED_IOMUX           PA16INDEX

#elif defined(TARGET_CUSTOM_CHIP)

    // --- Clock Setup ---
    #define INIT_SYS_CLOCK()    Clock_Init80MHz_Internal()

    // --- Mux Pins (GPIOA) ---
    #define MUX_PORT            GPIOA
    #define MUX_SEL0_PIN        12   // PA12
    #define MUX_SEL1_PIN        13   // PA13
    #define MUX_SEL2_PIN        22   // PA22
    #define MUX_SEL3_PIN        16   // PA16

    #define MUX_SEL0_IOMUX      PA12INDEX
    #define MUX_SEL1_IOMUX      PA13INDEX
    #define MUX_SEL2_IOMUX      PA22INDEX
    #define MUX_SEL3_IOMUX      PA16INDEX

    // --- Heartbeat LED (GPIOA) ---
    #define LED_PORT            GPIOA
    #define LED_PIN             18   // PA18
    #define LED_IOMUX           PA18INDEX

#else
    #error "Please define a target board (TARGET_LAUNCHPAD or TARGET_CUSTOM_CHIP) in BoardConfig.h"
#endif

#endif // BOARDCONFIG_H