#include "I2CADC.h"
#include "../inc/I2C.h"
#include "../inc/LaunchPad.h"
#include "../inc/Timer.h"
#include "../lib/synth.h"
#include <ti/devices/msp/msp.h>

#define SEL0 20
#define SEL1 24
#define SEL2 3
#define SEL3 4

static uint16_t prev_val[TOTAL_KEYS] = {0};
uint8_t addresses[5] = {0x48, 0x49, 0x4A, 0x4C, 0x4D};

void initMux(void) {
  // LP PB24, PB20, PB4, PB3 -> S0, S1, S2, S3
  GPIOB->DOE31_0 |= (1 << SEL0) | (1 << SEL1) | (1 << SEL2) | (1 << SEL3);
  IOMUX->SECCFG.PINCM[PB24INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB20INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB4INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB3INDEX] = 0x00000081;
}

void initADC(void) {
  initMux();
  I2C_Init();
  TimerA1_IntArm(80000000 / 1800, 1, 4);
}

// gets msb and lsb via I2C from whatever data passed by muxes
uint16_t readADC(uint8_t address) {
  uint16_t raw_data = I2C_Recv2(address);

  return (raw_data & 0x0FFF);
}

void incrementMux(void) {
  // cycle through hall sensor with mux selection lines
  static uint8_t count = 0;
  if (count > 11) { // we only need 12 out of the 16 mux inputs
    count = 0;
  }

  uint8_t count0 = count & 1;
  uint8_t count1 = (count & (1 << 1)) >> 1;
  uint8_t count2 = (count & (1 << 2)) >> 2;
  uint8_t count3 = (count & (1 << 3)) >> 3;

  GPIOB->DOUTCLR31_0 = (1 << SEL0) | (1 << SEL1) | (1 << SEL2) | (1 << SEL3);
  GPIOB->DOUTSET31_0 =
      (count0 << SEL0) | (count1 << SEL1) | (count2 << SEL2) | (count3 << SEL3);
  count++;
}

void setMux(uint8_t channel) {
  if (channel > 11) { 
    channel = 0;
  }
  uint8_t bit0 = channel & 1;
  uint8_t bit1 = (channel & (1 << 1)) >> 1;
  uint8_t bit2 = (channel & (1 << 2)) >> 2;
  uint8_t bit3 = (channel & (1 << 3)) >> 3;
  GPIOB->DOUTCLR31_0 = (1 << SEL0) | (1 << SEL1) | (1 << SEL2) | (1 << SEL3);
  GPIOB->DOUTSET31_0 =
      (bit0 << SEL0) | (bit1 << SEL1) | (bit2 << SEL2) | (bit3 << SEL3);
}

// ============================================================================
// CALIBRATION CONSTANTS
// ============================================================================
// Assuming 12-bit ADC (0-4095). 1.7V / 3.3V * 4095 = ~2110
#define RESTING_ADC_VAL 2110

// hysteresis thresholds to prevent bouncing/stuttering
#define POS_ATTACK_THRESH 240  // How far down the key must be to trigger
#define POS_RELEASE_THRESH 240 // Must lift up past this point to release

// Minimum speed required to trigger a note (rejects slow drifts/noise)
#define MIN_ATTACK_VEL_THRESH 20
// ============================================================================

int16_t prev_displacement[NUM_KEYS] = {0};
static bool key_is_pressed[NUM_KEYS] = {false};

inline void updateSingleKeyVal(void) {
  static uint8_t current_key = 0;
  uint8_t octave = current_key / 12;
  int16_t raw_adc = (int16_t)readADC(addresses[octave]);
  int16_t displacement = 0;
  if (current_key % 2 == 0) {
    // even keys - voltage up
    displacement = raw_adc - RESTING_ADC_VAL;
  } else {
    // odd keys - voltage down
    displacement = RESTING_ADC_VAL - raw_adc;
  }
  int16_t velocity = displacement - prev_displacement[current_key];
  prev_displacement[current_key] = displacement;
  if (!key_is_pressed[current_key]) {
    if (displacement > POS_ATTACK_THRESH && velocity >= MIN_ATTACK_VEL_THRESH) {
      key_is_pressed[current_key] = true;
      Attack_Key(current_key, velocity);
    }
  } else {
    if (displacement < POS_RELEASE_THRESH) {
      key_is_pressed[current_key] = false;
      Release_Key(current_key);
    }
  }
  setMux((current_key + 1) % 12);
  current_key++;
  if (current_key >= NUM_KEYS) {
    current_key = 0;
  }
}
void TIMA1_IRQHandler() {
  updateSingleKeyVal();
}