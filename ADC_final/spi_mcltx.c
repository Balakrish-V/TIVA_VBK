#include "tm4c123xx.h"

void SPI1_Write(unsigned char data)
{
    GPIO_PORTF_DATA_R &= ~(1 << 2); /* Make PF2 Selection line (SS) low */
    while ((SSI1_SR_R & 2) == 0)
        ; /* wait untill Tx FIFO is not full */
    SSI1_DR_R = data; /* transmit byte over SSI1Tx line */
    while (SSI1_SR_R & 0x10)
        ; /* wait until transmit complete */
    GPIO_PORTF_DATA_R |= 0x04; /* keep selection line (PF2) high in idle condition */
}

void SPI1_init(void)
{
    /* Enable clock to SPI1, GPIOD and GPIOF */

    SYSCTL_RCGCSSI_R |= (1 << 1); /*set clock enabling bit for SPI1 */
    SYSCTL_RCGCGPIO_R |= (1 << 3); /* enable clock to GPIOD for SPI1 */
    SYSCTL_RCGCGPIO_R |= (1 << 5); /* enable clock to GPIOF for slave select */

    /*Initialize PD3 and PD0 for SPI1 alternate function*/

    GPIO_PORTD_AMSEL_R &= ~0x09; /* disable analog functionality RD0 and RD3 */
    GPIO_PORTD_DEN_R |= 0x09; /* Set RD0 and RD3 as digital pin */
    GPIO_PORTD_AFSEL_R |= 0x09; /* enable alternate function of RD0 and RD3*/
    GPIO_PORTD_PCTL_R &= ~0x0000F00F; /* assign RD0 and RD3 pins to SPI1 */
    GPIO_PORTD_PCTL_R |= 0x00002002; /* assign RD0 and RD3 pins to SPI1  */

    /* Initialize PF2 as a digital output as a slave select pin */

    GPIO_PORTF_DEN_R |= (1 << 2); /* set PF2 pin digital */
    GPIO_PORTF_DIR_R |= (1 << 2); /* set PF2 pin output */
    GPIO_PORTF_DATA_R |= (1 << 2); /* keep SS idle high */

    /* Select SPI1 as a Master, POL = 0, PHA = 0, clock = 4 MHz, 8 bit data */

    SSI1_CR1_R = 0; /* disable SPI1 and configure it as a Master */
    SSI1_CC_R = 0; /* Enable System clock Option */
    SSI1_CPSR_R = 4; /* Select prescaler value of 4 .i.e 16MHz/4 = 4MHz */
    SSI1_CR0_R = 0x00007; /* 4MHz SPI1 clock, SPI mode, 8 bit data */
    SSI1_CR1_R |= 2; /* enable SPI1 */
}

/* This function generates delay in ms */
/* calculations are based on 16MHz system clock frequency */

void Delay_ms(int time_ms)
{
    int i, j;
    for (i = 0; i < time_ms; i++)
        for (j = 0; j < 3180; j++)
        {
        }
}
int main()
{
    SPI1_init();
    unsigned char ch = 'A';
    while (1)
    {
        SPI1_Write(ch);
        Delay_ms(100);
    }
}
