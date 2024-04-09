#include "tm4c123xx.h"
#include "uart_header.h"
unsigned char SPI1_Read(void)
{
    unsigned char dt;
    while(((SSI1_SR_R>>3)&0x1)==1);
    dt = SSI1_DR_R;
    return dt;
}

void SPI1_init(void)
{
    /* Enable clock to SPI1, GPIOD and GPIOF */

    SYSCTL_RCGCSSI_R |= (1 << 1); /*set clock enabling bit for SPI1 */
    SYSCTL_RCGCGPIO_R |= (1 << 3); /* enable clock to GPIOD for SPI1 */
    SYSCTL_RCGCGPIO_R |= (1 << 5); /* enable clock to GPIOF for slave select */

    /*Initialize PD3 and PD0 for SPI1 alternate function*/

    GPIO_PORTD_AMSEL_R &= ~((1<<0)|(1<<1)|(1<<2)); /* disable analog functionality RD0,RD2 and RD3 */
    GPIO_PORTD_DEN_R |= ((1<<0)|(1<<1)|(1<<2)); /* Set RD0 and RD2,RD3 as digital pin */
    GPIO_PORTD_AFSEL_R |=((1<<0)|(1<<1)|(1<<2)); /* enable alternate function of RD0 RD2 and RD3*/
    GPIO_PORTD_PCTL_R &= ~ 0x00000FFF; /* assign RD0,RD2,RD3 pins to SPI1 */
    GPIO_PORTD_PCTL_R |= 0x00000222; /* assign RD0,RD2,RD3 pins to SPI1  */


    // Initialize PF2 as a digital output as a slave select pin
    /*
    GPIO_PORTF_DEN_R |= (1 << 2); set PF2 pin digital
    GPIO_PORTF_DIR_R |= (1 << 2);  set PF2 pin output
    GPIO_PORTF_DATA_R |= (1 << 2);  keep SS idle high
    */
    /* Select SPI1 as a Master, POL = 0, PHA = 0, clock = 4 MHz, 8 bit data */

    SSI1_CR1_R &= (~(1 << 1));   // DISABLING THE SSI MODULE
    SSI1_CR1_R |= (1<<2);      // slave
    SSI1_CC_R = 0; /* Enable System clock Option */
    SSI1_CPSR_R = 4; /* Select prescaler value of 4 .i.e 16MHz/4 = 4MHz */
    SSI1_CR0_R = 0x00007; /* 4MHz SPI1 clock, SPI mode, 8 bit data */
    SSI1_CR1_R |= 2; /* enable SPI1 */
}

/* This function generates delay in ms */
/* calculations are based on 16MHz system clock frequency */

int main()
{
    SPI1_init();
    init_config();
    unsigned char ch;
    while (1)
    {
        ch=SPI1_Read();
        UART0_DR_R = ch;
        while(((UART0_FR_R>>7)&1)==0);
    }
}
