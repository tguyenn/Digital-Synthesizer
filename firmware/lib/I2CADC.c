#include "I2CADC.h"
#include <ti/devices/msp/msp.h>
#include "../inc/I2C.h"
#include "../inc/LaunchPad.h"



#define SEL0 20 
#define SEL1 24
#define SEL2 3
#define SEL3 4

static uint16_t prev_val[TOTAL_KEYS]= {0};
uint8_t addresses[5] = {0x48, 0x49, 0x4A, 0x4C,0x4D};


void initMux(void) {
  // LP PB24, PB20, PB4, PB3 -> S0, S1, S2, S3
  GPIOB->DOE31_0 |= (1 << SEL0) | (1 << SEL1) | (1 << SEL2) | (1 << SEL3);
  IOMUX->SECCFG.PINCM[PB24INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB20INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB4INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB3INDEX] = 0x00000081;
}
 
void initADC(void){
    initMux(); 
}

//gets msb and lsb via I2C from whatever data passed by muxes
uint16_t readADC(uint8_t address) {
    uint16_t raw_data = I2C_Recv2(address);
    
    return (raw_data & 0x0FFF);
}

void incrementMux(void) {
  // cycle through hall sensor with mux selection lines
  static uint8_t count = 0;
  if(count > 11) { // we only need 12 out of the 16 mux inputs
    count = 0;
  }
  
  uint8_t count0 = count & 1;
  uint8_t count1 = (count & (1 << 1)) >> 1;
  uint8_t count2 = (count & (1 << 2)) >> 2;
  uint8_t count3 = (count & (1 << 3)) >> 3;
  
  GPIOB->DOUTCLR31_0 = (1 << SEL0) | (1 << SEL1) | (1 << SEL2) | (1 << SEL3);
  GPIOB->DOUTSET31_0 = (count0 << SEL0) | (count1 << SEL1) | (count2 << SEL2) | (count3 << SEL3);
  count++;
}

ADCoutput_t updateKeyVals(void){
  ADCoutput_t current_output;
  
  for (uint8_t octave = 0; octave< 5; octave++){
    for (int key = 0; key < 12; key++){
      incrementMux();
      for(volatile int delay = 0; delay < 50; delay++);
      
      uint16_t key_val = readADC(addresses[octave]);
      int key_num = (octave * 12) + key;
      current_output.position[key_num] = key_val;
      
      int16_t velocity = key_val - prev_val[key_num];
      current_output.velocity[key_num] = velocity;

      prev_val[key_num] = key_val;     
    }
  }
  return current_output;
}


