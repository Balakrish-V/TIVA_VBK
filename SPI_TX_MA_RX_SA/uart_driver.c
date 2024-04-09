/*
 * uart_driver.c
 *
 *  Created on: 09-Feb-2024
 *      Author: TECQNIO
 */
#include "tm4c123xx.h"

#include "delay.h"

#include "uart_header.h"

void uart0_config(void)
{
    SYSCTL_RCGCUART_R |= 1<<0;  // Enable clock for UART0 Module - USB cable itself supports UART Communication

    SYSCTL_RCGCGPIO_R |= 1<<0;  // Enable clock for GPIO Module also - since uart in port A

    GPIO_PORTA_AFSEL_R |= (1<<0) | (1<<1); //(Port A corresponding pin 0 and 1)

    // Mode can be set in PCTL(port Control) register once the AFSEL corresponding bit is high

    GPIO_PORTA_PCTL_R |= (1<<0) | (1<<4); // Selecting pin 0 and 1 for func. as UART and selecting functionality also

    GPIO_PORTA_DEN_R |= (1<<0) | (1<<1); // Digitilalizing the pins 0 and 1

    // Disable uart before making the configurations
    UART0_CTL_R &= (~(1<<0)) & (~(1<<8)) & (~(1<<9));

    UART0_IBRD_R = 104; // baud rate 104
    UART0_FBRD_R = 11;

    UART0_LCRH_R |= (0x3<<5); //line control -> word length

    UART0_CC_R = 0x5; //clock select -> source of clock is precision internal oscillator

    // next is enable UART, UART Tx Enable and UART Rx Enable
    UART0_CTL_R|=(1<<0)|(1<<8)|(1<<9);

}

void uart1_config(void)
{
    SYSCTL_RCGCUART_R |= 1<<1;  // Enable clock for UART1 Module

    SYSCTL_RCGCGPIO_R |= 1<<2;  // Enable clock for GPIO Module also - since UART in port C

    GPIO_PORTC_AFSEL_R |= (1<<4) | (1<<5); //for pin 15 - Port C 5(TX) , pin 16 - Port C 4(RX)

    // Mode can be set in PCTL(port Control) register once the AFSEL corresponding bit is high

    GPIO_PORTC_PCTL_R |= (2<<16) | (2<<20); // Selecting PC4 and PC5(4 bits for a pin) for func. as UART and selecting functionality also

    GPIO_PORTC_DEN_R |= (1<<4) | (1<<5); // Digitilalizing the pins 0 and 1

    // Disable uart before making the configurations
    UART1_CTL_R &= (~(1<<0)) & (~(1<<8)) & (~(1<<9));

    UART1_IBRD_R = 104; // baud rate 104
    UART1_FBRD_R = 11;

    UART1_LCRH_R |=(0x3<<5) | (1<<4); //line control -> word length

    UART1_CC_R = 0x5; //clock select -> source of clock is precision internal oscillator

    // next is enable UART, UART Tx Enable and UART Rx Enable
    UART1_CTL_R|=(1<<0)|(1<<8)|(1<<9);

}

void uart3_config(void)
{
    SYSCTL_RCGCUART_R |= 1<<3;  // Enable clock for UART1 Module

    SYSCTL_RCGCGPIO_R |= 1<<2;  // Enable clock for GPIO Module also - since UART3 also in port C

    GPIO_PORTC_AFSEL_R |= (1<<7) | (1<<6); //for pin 15 - Port C 5(TX) , pin 16 - Port C 4(RX)

    // Mode can be set in PCTL(port Control) register once the AFSEL corresponding bit is high

    GPIO_PORTC_PCTL_R |= (1<<28) | (1<<24); // Selecting PC4 and PC5(4 bits for a pin) for func. as UART and selecting functionality also

    GPIO_PORTC_DEN_R |= (1<<7) | (1<<6); // Digitilalizing the pins 0 and 1

    // Disable uart before making the configurations
    UART3_CTL_R &= (~(1<<0)) & (~(1<<8)) & (~(1<<9));

    UART3_IBRD_R = 104; // baud rate 104
    UART3_FBRD_R = 11;

    UART3_LCRH_R |= (0x3<<5); //line control -> word length and enable FIFO's

    UART3_CC_R = 0x5; //clock select -> source of clock is precision internal oscillator

    // next is enable UART, UART Tx Enable and UART Rx Enable
    UART3_CTL_R|=(1<<0)|(1<<8)|(1<<9);

}

void uart0_char_tx(unsigned char data)
{
    UART0_DR_R = data;
    while(tx_flag_0==0);
}

void uart1_char_tx(unsigned char data)
{
    UART1_DR_R = data;
    while(tx_flag_1==0);
}

void uart3_char_tx(unsigned char data)
{
    UART3_DR_R = data;
    while(tx_flag_3==0);
}

void uart0_string_tx(unsigned char *p)
{
         while(*p)
         {
             while(tx_flag_0==0);
             UART0_DR_R = *p;
             p++;
         }
}
void uart1_string_tx(unsigned char *p)
{
         while(*p)
         {
             while(tx_flag_1==0);
             UART1_DR_R = *p;
             p++;
         }
}
void uart3_string_tx(unsigned char *p)
{
         while(*p)
         {
             while(tx_flag_3==0);
             UART3_DR_R = *p;
             p++;
         }
}
unsigned char uart0_char_rx(void)
{
    static unsigned char ch;
    while(rx_flag_0 == 0); // collect info once it becomes to ONE
    ch=UART0_DR_R;
    uart0_char_tx(ch);
    return ch;
}

unsigned char uart1_char_rx(void)
{
    unsigned char ch;
    while(rx_flag_1 == 0);
    ch=UART1_DR_R;
    uart1_char_tx(ch);
    return ch;
}

unsigned char uart3_char_rx(void)
{
    static unsigned char ch;
    while(rx_flag_3 == 0);
    ch=UART3_DR_R;
    uart3_char_tx(ch);
    return ch;
}

