#include "../inc/UART.h"
// #include "../inc/UART1.h"
#include "../lib/myUART.h"

void initUART() {
    UART_Init(); // chip -> LP comms
    // UART1_Init(); // to laptop via USB to UART bridge

}


void readUART0() {
    // char data = UART_InChar();
    
}
