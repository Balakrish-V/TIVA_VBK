#include "tm4c123xx.h"
#include <stdio.h>
#include "uart_header.h"
#define RX_BUFFER_SIZE 1
void delay(long k)
{
    while (k--)
        ;
}
void spi1_config(void)
{
    //CLOCK CONFIG
    SYSCTL_RCGCSSI_R |= (1 << 1);    //CLOCK FOR SPI MODULE 1
    SYSCTL_RCGCGPIO_R |= (1 << 3);     //CLOCK FOR PORTD

    //PD0,PD1,PD2,PD3 CONFIG
    GPIO_PORTD_AFSEL_R |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    GPIO_PORTD_PCTL_R |= (2 << 0) | (2 << 4) | (2 << 8) | (2 << 12);
    GPIO_PORTD_DEN_R |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    /*
     SSI1_CR1_R &= ~(1 << 1);  // Clearing bit 1 to set CPOL=0 (Clock Polarity)
     SSI1_CC_R = 0x0;          // SELECTING SYSTEM CLOCK
     SSI1_CPSR_R = 40; // Setting CPSDVSR to achieve 125kbps at system clock 16MHz (You may need to adjust this value according to your system clock)
     SSI1_CR0_R = 0x07; // SELECT DATA SIZE AS 8BIT AND FRAME AS FREE SCALE FRAME AND SPO AND SPH AS 0 AND SCR AS 0 (This setting corresponds to CPOL=0 and CPHA=0)
     SSI1_CR1_R |= (1 << 1);   // ENABLE SSI1
     */
}
void spi1_slave(void)
{
    SSI1_CR1_R &= (~(1 << 1));
    SSI1_CR1_R |= (1 << 2);   // SLAVE
    SSI1_CR0_R = 0xc7;
    SSI1_CR1_R |= (1 << 1);
}
void spi1_master(void)
{
    SSI1_CR1_R &= (~(1 << 1));
    SSI1_CR1_R &= ~(1 << 2);    // MASTER
    SSI1_CC_R = 0x0;          // SELECTING SYSTEM CLOCK
    SSI1_CPSR_R = 32; // Setting CPSDVSR to achieve 125kbps at system clock 16MHz (You may need to adjust this value according to your system clock)
    SSI1_CR0_R = 0xC7; // SELECT DATA SIZE AS 8BIT AND FRAME AS FREE SCALE FRAME AND SPO AND SPH AS 0 AND SCR AS 0 (This setting corresponds to CPOL=0 and CPHA=0)
    SSI1_CR1_R |= (1 << 1);
}
void spi1_write(unsigned char data)
{
    while (((SSI1_SR_R >> 1) & 1) == 0)
        ;
    SSI1_DR_R = data;
    while (((SSI1_SR_R >> 4) & 1) == 1)
        ;
}
unsigned char spi1_receive(void)
{
    char dt;
    while (((SSI1_SR_R >> 3) & 0x1) == 0)
        ;
    dt = SSI1_DR_R;
    return dt;
}
void uart5_char_tx(unsigned char data)
{
    // Wait for the UART5 transmit FIFO to be not full
    while (UART5_FR_R & UART_FR_TXFF)
        ;
    UART5_DR_R = data;
    while (tx_flag_5 == 0)
        ;
}
void uart5_config(void)
{
    // CLK_CONFIG
    SYSCTL_RCGCUART_R |= (1 << 5);                       //UART5 PORT CLOCK
    SYSCTL_RCGCGPIO_R |= (1 << 4);                       //GPIO PORTE

    //CONFIG PE4,PE5
    GPIO_PORTE_AFSEL_R |= (1 << 4) | (1 << 5);  //TO SELECT ALTERNATE FUNCTIONS
    GPIO_PORTE_PCTL_R |= (1 << 16) | (1 << 20); //SELECTING UART FUNCTION
    GPIO_PORTE_DEN_R |= (1 << 4) | (1 << 5);  //DIGITIALIZING PE4,PE5

    //UART5 CONFIG
    UART5_CTL_R &= (~(1 << 0)) & (~(1 << 8)) & (~(1 << 9)); //UART5 DISABLING BEFORE CONFIG
    UART5_IBRD_R = 104;                               //BAUD RATE INTERGER PART
    UART5_FBRD_R = 11;                              //BAUD RATE FRACCTIONAL PART
    UART5_LCRH_R |= (0x3 << 5);                          //WORD LENGTH - 8 BITS
    UART5_CC_R = 0x05;                              //SELECTING POSC
    UART5_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9); //0-UART ENABLE,8-TRANSMIT ENABLE,9-RECEIVE ENABLE
}
unsigned char rx_data[RX_BUFFER_SIZE];
int main()
{
    spi1_config();
    uart5_config();
    while (1)
    {
        spi1_master();
        spi1_write('2');
        delay(10000);
        spi1_slave();
        rx_data[0] = spi1_receive();
        uart5_char_tx(rx_data[0]);
    }
}
