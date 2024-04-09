#include "tm4c123xx.h"
#include <stdint.h>
unsigned char dta;
void init_config(void)
{
    //CLOCK CONFIG
    SYSCTL_RCGCUART_R |= (1 << 0);     //UART0 PORT CLOCK
    SYSCTL_RCGCSSI_R |= (1 << 0);    //CLOCK FOR SPI MODULE 0
    SYSCTL_RCGCGPIO_R |= (1 << 0);     //CLOCK FOR PORTA

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

    //PA2,PA3,PA4,PA5 CONFIG
    GPIO_PORTA_AFSEL_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
    GPIO_PORTA_PCTL_R |= 0x00222211;
    GPIO_PORTA_DEN_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
    // GPIO_PORTA_DATA_R |= (1 << 3);

    //SSI0 CONFIG ==CPOL == CPHA == 1,1
    SSI0_CR1_R &= (~(1 << 1));
    // SSI0_CR1_R |= (1 << 0);// DISABLING THE SSI MODULE
    SSI0_CR1_R |= (1 << 2);      // slave
    SSI0_CC_R = 0x0;        // SELECTING SYSTEM CLOCK
    SSI0_CPSR_R = 128;     // DATASHEET - 1MBPS
    SSI0_CR0_R = 0xC7; // SELECT DATA SIZE AS 8BIT AND FRAME AS FREE SCALE FRAME AND SPO AND SPH AS 0 AND SCR AS 0
    SSI0_CR1_R |= (1 << 1); // ENABLE THE Spi module
}

unsigned char spi_receive()
{
    char dt;
    while (((SSI0_SR_R >> 3) & 0x1) == 0)
        ;
    dt = SSI0_DR_R;
    return dt;
}
void my_Int_config(void)
{
    NVIC_EN0_R = (1 << 7); //spi0 interrupt
    NVIC_PRI1_R = (3 << 28); //highest priority
    SSI0_IM_R = (1 << 2); //spi0 rx interrupt
}
void IntDefaultHandler(void)
{
    if ((SSI0_MIS_R >> 2) & 1)
    {
        dta = spi_receive();
        /* Displaying in UART */
        while ((UART0_FR_R & (1 << 5)) != 0)
            ;
        UART0_DR_R = dta;
    }
    //interrupt clear
}
int main()
{
    init_config();
    my_Int_config();
    while (1)
    {

    }
}
