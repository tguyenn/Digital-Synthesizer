#include "../inc/UART.h"
#include "../inc/UART1.h"
#include "../lib/myUART.h"

void initUART() {
    UART_Init(); // chip -> LP comms
    UART1_Init(); // to laptop via USB to UART bridge
}


void readUART0() {
    // char data = UART_InChar();
    UART1_OutChar('L');
    UART1_OutChar('b');
    UART1_OutChar('o');
    UART1_OutChar('z');
    UART1_OutChar('o');
}
