#include "tm4c123xx.h"

void init_config(void)
{
    //CLOCK CONFIG
    SYSCTL_RCGCUART_R |= (1 << 0);     //UART0 PORT CLOCK
    SYSCTL_RCGCSSI_R |= (1 <<1);    //CLOCK FOR SPI MODULE 1
    SYSCTL_RCGCGPIO_R |= (1 << 3)| (1<<0);     //CLOCK FOR PORTD

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
    GPIO_PORTD_AFSEL_R |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    GPIO_PORTD_PCTL_R |= 0x00002222;
    GPIO_PORTD_DEN_R |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    // GPIO_PORTA_DATA_R |= (1 << 3);

    //SSI1 CONFIG --CPOL == CPHA == 1,1
    SSI1_CR1_R &= (~(1 << 1));
    // SSI1_CR1_R |= (1 << 0);// DISABLING THE SSI MODULE
    SSI1_CR1_R |= (1 << 2);      // slave
    SSI1_CC_R = 0x0;        // SELECTING SYSTEM CLOCK
    SSI1_CPSR_R = 128;     // DATASHEET - 1MBPS
    SSI1_CR0_R = 0xC7; // SELECT DATA SIZE AS 8BIT AND FRAME AS FREE SCALE FRAME AND SPO AND SPH AS 0 AND SCR AS 0
    SSI1_CR1_R |= (1 << 1); // ENABLE THE SLAVE
}

void spi_write(unsigned char data)
{
    while (((SSI1_SR_R >> 1) & 1) == 0)
        ;
    SSI1_DR_R = data;
    while (((SSI1_SR_R >> 4) & 1) == 1)
        ;
}
unsigned char spi_receive()
{
    char dt;
    while (((SSI1_SR_R >> 3) & 0x1) == 0)
        ;
    dt = SSI1_DR_R;
    return dt;
}

int main()
{
    init_config();
    unsigned char dta;
    while (1)
    {
        dta = spi_receive();
        while ((UART0_FR_R & (1 << 5)) != 0)
            ;
        UART0_DR_R = dta;
    }
}
