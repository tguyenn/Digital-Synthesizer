#ifndef ENCODER_H 
#define ENCODER_H 
#include <stdint.h> 
// Use 'extern' so other files can see these without recreating them 

void encoderInit(void); 

extern volatile int32_t menu_pos; 
extern int8_t menu_screen; 
extern uint8_t volume; 
extern volatile uint8_t update_menu; 
extern volatile uint8_t update_screen_type; 



#endif