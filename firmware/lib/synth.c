#include "synth.h"
#include "Audio_DAC_DMA.h"
#include "LookupTable.h"
#include "Piano.h"
#include "Envelope.h"

const uint32_t Phase_Incs[NUM_KEYS] = {
    PHASE_INC(FREQ_C2),  PHASE_INC(FREQ_CS2), PHASE_INC(FREQ_D2),
    PHASE_INC(FREQ_DS2), PHASE_INC(FREQ_E2),  PHASE_INC(FREQ_F2),
    PHASE_INC(FREQ_FS2), PHASE_INC(FREQ_G2),  PHASE_INC(FREQ_GS2),
    PHASE_INC(FREQ_A2),  PHASE_INC(FREQ_AS2), PHASE_INC(FREQ_B2),

    PHASE_INC(FREQ_C3),  PHASE_INC(FREQ_CS3), PHASE_INC(FREQ_D3),
    PHASE_INC(FREQ_DS3), PHASE_INC(FREQ_E3),  PHASE_INC(FREQ_F3),
    PHASE_INC(FREQ_FS3), PHASE_INC(FREQ_G3),  PHASE_INC(FREQ_GS3),
    PHASE_INC(FREQ_A3),  PHASE_INC(FREQ_AS3), PHASE_INC(FREQ_B3),

    PHASE_INC(FREQ_C4),  PHASE_INC(FREQ_CS4), PHASE_INC(FREQ_D4),
    PHASE_INC(FREQ_DS4), PHASE_INC(FREQ_E4),  PHASE_INC(FREQ_F4),
    PHASE_INC(FREQ_FS4), PHASE_INC(FREQ_G4),  PHASE_INC(FREQ_GS4),
    PHASE_INC(FREQ_A4),  PHASE_INC(FREQ_AS4), PHASE_INC(FREQ_B4),

    PHASE_INC(FREQ_C5),  PHASE_INC(FREQ_CS5), PHASE_INC(FREQ_D5),
    PHASE_INC(FREQ_DS5), PHASE_INC(FREQ_E5),  PHASE_INC(FREQ_F5),
    PHASE_INC(FREQ_FS5), PHASE_INC(FREQ_G5),  PHASE_INC(FREQ_GS5),
    PHASE_INC(FREQ_A5),  PHASE_INC(FREQ_AS5), PHASE_INC(FREQ_B5),

    PHASE_INC(FREQ_C6),  PHASE_INC(FREQ_CS6), PHASE_INC(FREQ_D6),
    PHASE_INC(FREQ_DS6), PHASE_INC(FREQ_E6),  PHASE_INC(FREQ_F6),
    PHASE_INC(FREQ_FS6), PHASE_INC(FREQ_G6),  PHASE_INC(FREQ_GS6),
    PHASE_INC(FREQ_A6),  PHASE_INC(FREQ_AS6), PHASE_INC(FREQ_B6)};

typedef struct {
  int16_t init_vel;
  uint32_t time;
  uint32_t phase;
} Key_State;

typedef struct {
  bool update_me;
  bool attack_release;
  uint16_t init_vel;
} Key_Action;

Key_State Key_States[NUM_KEYS] = {0};
bool Active_Keys[NUM_KEYS] = {false}; //shitty hash table
Key_Action Key_Actions[NUM_KEYS] = {0};

void Attack_Key(uint8_t key_idx, int16_t velocity) {
  Key_Actions[key_idx].attack_release = true;
  Key_Actions[key_idx].init_vel = velocity;
  Key_Actions[key_idx].update_me = true; 
}

void Release_Key(uint8_t key_idx) {
  Key_Actions[key_idx].attack_release = false;
  Key_Actions[key_idx].init_vel = 0;
  Key_Actions[key_idx].update_me = true;
}

int32_t Inc_Key_State(uint8_t key_idx, Key_State *key_state) {
  uint32_t env_idx = key_state->time >> 10;

  if (env_idx >= ENV_SIZE) {
    Active_Keys[key_idx] = false;
    return 0;
  }
  int32_t val = PianoLUT[key_state->phase >> 22];
  val = (val * Note_Gain[key_idx]) >> 15;
  val = (val * Envelope_LUT[env_idx]) >> 15;
  key_state->time++;
  key_state->phase += Phase_Incs[key_idx];

  return val;
}

void Synth_Write_Buf() {
  uint32_t offset = 0;
  if (refillPing) {
    offset = 0;
    refillPing = false;
  } else if (refillPong) {
    offset = AUDIO_BUF_SIZE;
    refillPong = false;
  } else {
    return;
  }

  uint8_t active_indices[NUM_KEYS];
  uint8_t local_num_active = 0;
  for (int j = 0; j < NUM_KEYS; j++) {
    
    if (Key_Actions[j].update_me) {
      Key_Actions[j].update_me = false;
      if (Key_Actions[j].attack_release == true) {
        Active_Keys[j] = true;
        Key_States[j].phase = 0; 
        Key_States[j].time = 0;
        Key_States[j].init_vel = Key_Actions[j].init_vel;
      } else {
        Active_Keys[j] = false;
      }
    }
    if (Active_Keys[j]) {
      active_indices[local_num_active++] = j;
    }
  }
  for (int i = 0; i < AUDIO_BUF_SIZE; i++) {
    int32_t sample = 0;
    for (int k = 0; k < local_num_active; k++) {
      uint8_t key_idx = active_indices[k];
      sample += Inc_Key_State(key_idx, &Key_States[key_idx]);
    }

    sample = sample / 4;

    if (sample > 2047) {
      sample = 2047;
    } else if (sample < -2048) {
      sample = -2048;
    }

    audioBuffer[i + offset] = (int16_t)sample;
  }
}
