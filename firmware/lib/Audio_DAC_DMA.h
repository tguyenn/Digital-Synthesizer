#pragma once

#define AUDIO_BUF_SIZE 256
#define AUDIO_TOTAL_SIZE (AUDIO_BUF_SIZE * 2)

void TimerG0_32kHz_EventArm(void);

void DAC0_Init(void);

void DMA_Init_CircularPingPong();

void DMA_IRQHandler(void);