#include <stdint.h>

#include "../lib/ti_msp_dl_config.h"
#include "Audio_DAC_DMA.h"

extern volatile int pingorpong;

// Unified array holding both Ping (first half) and Pong (second half)
volatile uint16_t waveBuffer[AUDIO_BUF_SIZE * 2] = {
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

void setupPingPongDMA(void) {
  // 1. Configure Source and Destination Addresses
  // Source is our unified buffer, Destination is the DAC data register
  DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&waveBuffer[0]);
  DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&DAC0->DATA0);

  // 2. Set Transfer Size to the FULL buffer
  DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID,
                         sizeof(waveBuffer) / sizeof(uint16_t));

  // 3. Set Addressing Modes
  // Source increments to walk through the array.
  // Destination is fixed because the DAC register doesn't move[cite: 108].
  DL_DMA_configTransfer(
      DMA, DMA_CH0_CHAN_ID, DL_DMA_FULL_CH_REPEAT_SINGLE_TRANSFER_MODE,
      DL_DMA_NORMAL_MODE, DL_DMA_WIDTH_HALF_WORD, DL_DMA_WIDTH_HALF_WORD,
      DL_DMA_ADDR_INCREMENT, DL_DMA_ADDR_UNCHANGED);

  // 4. Set Transfer Mode to Repeated Single Transfer
  // The DMA remains enabled; size and addresses auto-reload when size hits
  // zero[cite: 202, 204, 208].
  DL_DMA_setTransferMode(DMA, DMA_CH0_CHAN_ID,
                         DL_DMA_FULL_CH_REPEAT_SINGLE_TRANSFER_MODE);

  // 5. Configure the Trigger (e.g., a Timer to control sample rate)
  DL_DMA_setSubscriberChanID(DMA, DL_DMA_SUBSCRIBER_INDEX_0, 0x0F); //Event channel 15

  // 6. Enable Interrupts (The Ping-Pong Secret Sauce)
  // Enable the Early IRQ at exactly half the transfer size to notify us the
  // "Ping" half is done[cite: 398, 405].
  DL_DMA_Full_Ch_setEarlyInterruptThreshold(
      DMA, DMA_CH0_CHAN_ID, DL_DMA_EARLY_INTERRUPT_THRESHOLD_HALF);
  DL_DMA_enableInterrupt(DMA, DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL0);

  // Enable the standard channel interrupt for when the "Pong" half is done
  // (size decrements to zero)[cite: 379, 380].
  DL_DMA_enableInterrupt(DMA, DL_DMA_INTERRUPT_CHANNEL0);

  // Enable the DMA channel
  DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

  NVIC_EnableIRQ(DMA_INT_IRQn);
}

void DMA_IRQHandler(void) {
  // IIDX is automatically cleared when read, removing the pending flag[cite:
  // 384].
  switch (DL_DMA_getPendingInterrupt(DMA)) {

  case DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH0:
    // The DMA just finished the first half (Ping).
    // It is currently reading from the second half (Pong).
    // It is safe to synthesize new data into the first half.
    pingorpong = 1;
    break;

  case DL_DMA_EVENT_IIDX_DMACH0:
    // The DMA just finished the second half (Pong).
    // The DMA has auto-reloaded and is now reading from the first half (Ping)
    // again. It is safe to synthesize new data into the second half.
    pingorpong = 0;
    break;

  default:
    break;
  }
}
