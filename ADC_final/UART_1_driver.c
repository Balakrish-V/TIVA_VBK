#include "tm4c123xx.h"
#include "uart_header.h"
void init_config(void)
{
    //CLOCK CONFIG
    SYSCTL_RCGCUART_R |= (1 << 0);     //UART0 PORT CLOCK
    SYSCTL_RCGCGPIO_R |= (1<<0);     //CLOCK FOR PORTA

    //UART0 CONFIG
    UART0_CTL_R &= (~(1 << 0)) & (~(1 << 8)) & (~(1 << 9)); //UART0 DISABLING BEFORE CONFIG
    UART0_IBRD_R = 104;                               //BAUD RATE INTERGER PART
    UART0_FBRD_R = 11;                              //BAUD RATE FRACCTIONAL PART
    UART0_LCRH_R |= (0x3 << 5);                          //WORD LENGTH - 8 BITS
    UART0_CC_R = 0x05;                              //SELECTING POSC
    UART0_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9); //0-UART ENABLE,8-TRANSMIT ENABLE,9-RECEIVE ENABLE

    //CONFIG PA0,PA1
    GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1);  //TO SELECT ALTERNATE FUNCTIONS
    GPIO_PORTA_PCTL_R |= (1 << 0) | (1 << 4);  //SELECTING UART FUNCTION
    GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);  //DIGITIALIZING PA0,PA1
}
unsigned char uart1_char_rx(void)
{
    static unsigned char ch;
    while (rx_flag_1 == 0);
    ch = UART1_DR_R;
    uart1_char_tx(ch);
    return ch;
}
void uart1_char_tx(unsigned char data)
{
    UART1_DR_R = data;
    while (tx_flag_1 == 0);
}
void uart1_string_tx(unsigned char *p)
{
    while(*p!='\0')
    {
        UART1_DR_R = *p;
        while(tx_flag_1 == 0);
        p++;
    }
}
