/*
 * spi_driver.c
 *
 *  Created on: 12-Feb-2024
 *      Author: TECQNIO
 */

#include "tm4c123xx.h"

#include "delay.h"

#include "spi_header.h"


void spi2_config(void)
{
    // Enable the clock for spi module2 which we have chosen
    SYSCTL_RCGCSSI_R = (1<<2);

    // Enable the clock to GPIO Port - B in which the SPI Protocol is being connected
    SYSCTL_RCGCGPIO_R |= 1<<1;

    // Next we have enable the alternate functions for the pins in respective port number
    GPIO_PORTB_AFSEL_R |= (1<<7) | (1<<6) | (1<<4) | (1<<5);

    // Check for table 23.5 - to particularly choose which alternate function you want to use ...page 669
    // keep the digital function 1 to 15 in mind and write (1 to 15) in the respective 4 bits which were given
    GPIO_PORTB_PCTL_R |= (2<<16) | (2<<20) | (2<<24) | (2<<28);

    // Digital functions enable
    GPIO_PORTB_DEN_R |= (1<<4) | (1<<5) | (1<<6) | (1<<7);

    // now SSE bit must be clear before making any configurations in the spi protocol
    SSI2_CR1_R &= ~(1<<1);

    // To make the spi protocol as a master
    SSI2_CR1_R = 0x00000000;

    // Clock Source - > main oscillator is the clock source
    // precision internal oscillator is the default clock source
    SSI2_CC_R = 0x00;

    // We need to set the prescale value since we have chosen main oscillator as the clock source
    SSI2_CPSR_R = 10; // meaning 16Mhz/10

    // SSI is a generic term..under this there are various serial communication protocols being used
    // also the number of bits which we send is configured in this register
    SSI2_CR0_R = 0x7;

    // Now the required configurations being done, we have to enable the spi module
    SSI2_CR1_R |=  (1<<1);
}

void spi2_char_tx(unsigned char data)
{
    SSI2_DR_R = data;
    while(spi_2_flag == 0); //wait till the data is completely written into the fifo buffer
}
