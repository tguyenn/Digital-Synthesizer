#include <stdint.h>
#include <ti/devices/msp/msp.h>

#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "Audio_DAC_DMA.h"

static volatile int refillPing;
static volatile int refillPong;

volatile uint16_t audioBuffer[AUDIO_TOTAL_SIZE] = {
    2047, 2097, 2147, 2198, 2248, 2298, 2347, 2397, 2446, 2496, 2544, 2593,
    2641, 2689, 2737, 2784, 2830, 2877, 2922, 2967, 3012, 3056, 3099, 3142,
    3184, 3226, 3266, 3306, 3346, 3384, 3422, 3458, 3494, 3530, 3564, 3597,
    3629, 3661, 3691, 3721, 3749, 3776, 3803, 3828, 3852, 3875, 3897, 3918,
    3938, 3957, 3974, 3991, 4006, 4020, 4033, 4044, 4055, 4064, 4072, 4079,
    4084, 4088, 4092, 4093, 4094, 4093, 4092, 4088, 4084, 4079, 4072, 4064,
    4055, 4044, 4033, 4020, 4006, 3991, 3974, 3957, 3938, 3918, 3897, 3875,
    3852, 3828, 3803, 3776, 3749, 3721, 3691, 3661, 3629, 3597, 3564, 3530,
    3494, 3458, 3422, 3384, 3346, 3306, 3266, 3226, 3184, 3142, 3099, 3056,
    3012, 2967, 2922, 2877, 2830, 2784, 2737, 2689, 2641, 2593, 2544, 2496,
    2446, 2397, 2347, 2298, 2248, 2198, 2147, 2097, 2047, 1997, 1947, 1896,
    1846, 1796, 1747, 1697, 1648, 1598, 1550, 1501, 1453, 1405, 1357, 1310,
    1264, 1217, 1172, 1127, 1082, 1038, 995,  952,  910,  868,  828,  788,
    748,  710,  672,  636,  600,  564,  530,  497,  465,  433,  403,  373,
    345,  318,  291,  266,  242,  219,  197,  176,  156,  137,  120,  103,
    88,   74,   61,   50,   39,   30,   22,   15,   10,   6,    2,    1,
    0,    1,    2,    6,    10,   15,   22,   30,   39,   50,   61,   74,
    88,   103,  120,  137,  156,  176,  197,  219,  242,  266,  291,  318,
    345,  373,  403,  433,  465,  497,  530,  564,  600,  636,  672,  710,
    748,  788,  828,  868,  910,  952,  995,  1038, 1082, 1127, 1172, 1217,
    1264, 1310, 1357, 1405, 1453, 1501, 1550, 1598, 1648, 1697, 1747, 1796,
    1846, 1896, 1947, 1997, 2047, 2097, 2147, 2198, 2248, 2298, 2347, 2397,
    2446, 2496, 2544, 2593, 2641, 2689, 2737, 2784, 2830, 2877, 2922, 2967,
    3012, 3056, 3099, 3142, 3184, 3226, 3266, 3306, 3346, 3384, 3422, 3458,
    3494, 3530, 3564, 3597, 3629, 3661, 3691, 3721, 3749, 3776, 3803, 3828,
    3852, 3875, 3897, 3918, 3938, 3957, 3974, 3991, 4006, 4020, 4033, 4044,
    4055, 4064, 4072, 4079, 4084, 4088, 4092, 4093, 4094, 4093, 4092, 4088,
    4084, 4079, 4072, 4064, 4055, 4044, 4033, 4020, 4006, 3991, 3974, 3957,
    3938, 3918, 3897, 3875, 3852, 3828, 3803, 3776, 3749, 3721, 3691, 3661,
    3629, 3597, 3564, 3530, 3494, 3458, 3422, 3384, 3346, 3306, 3266, 3226,
    3184, 3142, 3099, 3056, 3012, 2967, 2922, 2877, 2830, 2784, 2737, 2689,
    2641, 2593, 2544, 2496, 2446, 2397, 2347, 2298, 2248, 2198, 2147, 2097,
    2047, 1997, 1947, 1896, 1846, 1796, 1747, 1697, 1648, 1598, 1550, 1501,
    1453, 1405, 1357, 1310, 1264, 1217, 1172, 1127, 1082, 1038, 995,  952,
    910,  868,  828,  788,  748,  710,  672,  636,  600,  564,  530,  497,
    465,  433,  403,  373,  345,  318,  291,  266,  242,  219,  197,  176,
    156,  137,  120,  103,  88,   74,   61,   50,   39,   30,   22,   15,
    10,   6,    2,    1,    0,    1,    2,    6,    10,   15,   22,   30,
    39,   50,   61,   74,   88,   103,  120,  137,  156,  176,  197,  219,
    242,  266,  291,  318,  345,  373,  403,  433,  465,  497,  530,  564,
    600,  636,  672,  710,  748,  788,  828,  868,  910,  952,  995,  1038,
    1082, 1127, 1172, 1217, 1264, 1310, 1357, 1405, 1453, 1501, 1550, 1598,
    1648, 1697, 1747, 1796, 1846, 1896, 1947, 1997};

#define EVENT_ROUTE_1 1 // We will use Event Route 1 for the hardware trigger

void TimerG0_32kHz_EventArm(void) {
  // 1. Release the Timer from Reset (Do NOT assert it)
  TIMG0->GPRCM.RSTCTL =
      (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETSTKYCLR_CLR);

  TIMG0->GPRCM.PWREN =
      (GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE);
  Clock_Delay(24);

  // 2. Clock Configuration (SYSCLK for TIMG0 in PD0 is 40 MHz)
  TIMG0->CLKSEL = 0x08;      // Select SYSCLK
  TIMG0->CLKDIV = 0x00;      // Divide by 1
  TIMG0->COMMONREGS.CPS = 0; // Prescale = 1 (Value + 1)

  // 3. Set the Period for 32,000 Hz using a 40 MHz clock!
  // 40,000,000 / 32,000 = 1250. LOAD = 1250 - 1 = 1249.
  TIMG0->COUNTERREGS.LOAD = 1249;

  // 4. Configure Event Publishing
  TIMG0->CPU_INT.IMASK = 0;
  TIMG0->GEN_EVENT0.IMASK = GPTIMER_GEN_EVENT0_IMASK_Z_SET;
  TIMG0->FPUB_0 = EVENT_ROUTE_1;

  // 5. Start the Timer in PERIODIC (Repeat) Mode
  TIMG0->COMMONREGS.CCLKCTL = 1;
  TIMG0->COUNTERREGS.CTRCTL =
      (GPTIMER_CTRCTL_EN_ENABLED | GPTIMER_CTRCTL_CM_DOWN |
       GPTIMER_CTRCTL_REPEAT_REPEAT_1); // Bit 4: REPEAT Enable
}

void DAC0_Init(void) {
  // 1. Release the DAC from Reset (Do NOT assert it)
  DAC0->GPRCM.RSTCTL =
      (DAC12_RSTCTL_KEY_UNLOCK_W | DAC12_RSTCTL_RESETSTKYCLR_CLR);

  DAC0->GPRCM.PWREN = (DAC12_PWREN_KEY_UNLOCK_W | DAC12_PWREN_ENABLE_ENABLE);
  Clock_Delay(24);

  // 2. Configure PA15 for analog output
  IOMUX->SECCFG.PINCM[PA15INDEX] = 0x00000080;

  // 3. Configure DAC Control Registers
  DAC0->CTL0 = (DAC12_CTL0_ENABLE_SET | DAC12_CTL0_RES__12BITS);

  DAC0->CTL1 = (DAC12_CTL1_AMPEN_ENABLE | DAC12_CTL1_OPS_OUT0);
}

void DMA_Init_CircularPingPong() {
  // Note: The MSPM0 DMA block does not have a local GPRCM power register
  // like the timers or DAC. It is managed by SYSCTL.

  // 1. Ensure Channel 0 is disabled before modifying registers
  DMA->DMACHAN[0].DMACTL &= ~DMA_DMACTL_DMAEN_MASK;

  // 2. Set Addresses and Transfer Size
  DMA->DMACHAN[0].DMASA = (uint32_t)audioBuffer;
  DMA->DMACHAN[0].DMADA =
      (uint32_t)&(DAC0->DATA0); // Destination: DAC Data register
  DMA->DMACHAN[0].DMASZ = AUDIO_TOTAL_SIZE;

  // 3. Trigger Configuration (Subscribe to TimerG0 via Event Route 1)
  // Connect Event Route 1 into DMA Subscriber Port 0
  DMA->FSUB_0 = EVENT_ROUTE_1;

  // Tell DMA Channel 0 to trigger whenever Subscriber Port 0 ticks
  DMA->DMATRIG[0].DMATCTL = DMA_GENERIC_SUB0_TRIG;

  // 4. Configure the Channel Control (DMACTL) Register
  DMA->DMACHAN[0].DMACTL =
      DMA_DMACTL_DMAPREIRQ_PREIRQ_HALF | // Automatically flag at exactly 50%
      DMA_DMACTL_DMASRCWDTH_HALF |       // 16-bit source read
      DMA_DMACTL_DMADSTWDTH_HALF |       // 16-bit destination write
      DMA_DMACTL_DMASRCINCR_INCREMENT |  // Increment source array pointer
      DMA_DMACTL_DMADSTINCR_UNCHANGED |  // DAC address never changes
      DMA_DMACTL_DMATM_RPTSNGL |         // Repeated Single Transfer
      DMA_DMACTL_DMAEN_ENABLE; // Enable Channel(Enable Full Interrupt)
  DMA_DMACTL_DMAEN_ENABLE;     // Enable Channel

  // 5. Enable DMA CPU Interrupts
  // Enable the full transfer interrupt (DMACH0) and the early halfway interrupt
  // (PREIRQ)
  DMA->CPU_INT.IMASK =
      (DMA_CPU_INT_IMASK_DMACH0_SET | DMA_CPU_INT_IMASK_PREIRQCH0_SET);

  // Enable DMA Interrupt in Cortex-M0+ NVIC
  NVIC_EnableIRQ(DMA_INT_IRQn);
}

// ISR Name must match the vector table in your TI startup file
void DMA_IRQHandler(void) {
  // Read the Interrupt Index (this automatically clears the flag)
  uint32_t pending_irq = DMA->CPU_INT.IIDX;

  if (pending_irq == DMA_CPU_INT_IIDX_STAT_PREIRQCH0) {
    // Halfway mark reached (Early Interrupt).
    // DMA is now actively reading the Pong half.
    refillPing = true;
  } else if (pending_irq == DMA_CPU_INT_IIDX_STAT_DMACH0) {
    // Transfer complete (Full Interrupt).
    // Pointer wrapped around. DMA is now actively reading the Ping half.
    refillPong = true;
  }
}