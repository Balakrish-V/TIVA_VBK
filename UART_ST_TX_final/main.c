#include "tm4c123xx.h"
#include "uart_header.h"
unsigned char rx_data;
void delay(long k)
{
    while (k--)
        ;
}

void uart1_char_tx(unsigned char data)
{
    UART1_DR_R = data;
    while (tx_flag_1 == 0)
        ;
}
void uart5_char_tx(unsigned char data)
{
    // Wait for the UART5 transmit FIFO to be not full
    while (UART5_FR_R & UART_FR_TXFF)
        ;
    UART5_DR_R = data;
    while (tx_flag_5 == 0);
}
unsigned char uart1_char_rx(void)
{
    static unsigned char ch;
    while (rx_flag_1 == 0)
        ;
    ch = UART1_DR_R;
    return ch;
}
void uart1_config(void)
{
    SYSCTL_RCGCUART_R |= 1 << 1;  // Enable clock for UART1 Module
    SYSCTL_RCGCGPIO_R |= 1 << 2; // Enable clock for GPIO Module also - since UART in port C
    GPIO_PORTC_AFSEL_R |= (1 << 4) | (1 << 5);
    //for pin 15 - Port C 5(TX) , pin 16 - Port C 4(RX)
    // Mode can be set in PCTL(port Control) register once the AFSEL corresponding bit is high
    GPIO_PORTC_PCTL_R |= (2 << 16) | (2 << 20);
    // Selecting PC4 and PC5(4 bits for a pin) for func. as UART and selecting functionality also
    GPIO_PORTC_DEN_R |= (1 << 4) | (1 << 5); // Digitilalizing the pins 0 and 1
    // Disable uart before making the configurations

    // Set PC4 (UART1 RX pin) as input
    GPIO_PORTC_DIR_R &= ~(1 << 4);

    UART1_CTL_R &= (~(1 << 0)) & (~(1 << 8)) & (~(1 << 9));
    UART1_IBRD_R = 104; // baud rate 104
    UART1_FBRD_R = 11;
    UART1_LCRH_R |= (0x3 << 5); //line control -> word length
    UART1_CC_R = 0x5; //clock select -> source of clock is precision internal oscillator
    // next is enable UART, UART Tx Enable and UART Rx Enable
    UART1_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9);
}
void uart5_config(void)
{
    // CLK_CONFIG
    SYSCTL_RCGCUART_R |= (1 << 5);                       //UART5 PORT CLOCK
    SYSCTL_RCGCGPIO_R |= (1 << 4);                       //GPIO PORTE

    //CONFIG PE4,PE5
    GPIO_PORTE_AFSEL_R |= (1 << 4) | (1 << 5);  //TO SELECT ALTERNATE FUNCTIONS
    GPIO_PORTE_PCTL_R |= (1 << 16) | (1 << 20);       //SELECTING UART FUNCTION
    GPIO_PORTE_DEN_R |= (1 << 4) | (1 << 5);  //DIGITIALIZING PE4,PE5

    //UART5 CONFIG
    UART5_CTL_R &= (~(1 << 0)) & (~(1 << 8)) & (~(1 << 9)); //UART5 DISABLING BEFORE CONFIG
    UART5_IBRD_R = 104;                               //BAUD RATE INTERGER PART
    UART5_FBRD_R = 11;                              //BAUD RATE FRACCTIONAL PART
    UART5_LCRH_R |= (0x3 << 5);                          //WORD LENGTH - 8 BITS
    UART5_CC_R = 0x05;                              //SELECTING POSC
    UART5_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9); //0-UART ENABLE,8-TRANSMIT ENABLE,9-RECEIVE ENABLE
}
int main()
{
    uart5_config();
    uart1_config();
    while (1)
    {
        uart1_char_tx('A');
        delay(10000); //this delay is necessary. If not, it will not give the receiver sufficient time to process the data
        rx_data = uart1_char_rx();
        uart5_char_tx(rx_data);
    }
}
