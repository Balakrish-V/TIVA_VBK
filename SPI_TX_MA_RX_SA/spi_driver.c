/*
 * spi_driver.c
 *
 *  Created on: 12-Feb-2024
 *      Author: TECQNIO
 */

#include "tm4c123xx.h"
#include "delay.h"
#include "spi_header.h"

void spi0_config(void)
{
    // Enable the clock for spi module0 which we have chosen
    SYSCTL_RCGCSSI_R = (1 << 0);

    // Enable the clock to GPIO Port - A in which the SPI Protocol is being connected
    SYSCTL_RCGCGPIO_R |= 1 << 0;

    // Next we have enable the alternate functions for the pins in respective port number
    GPIO_PORTA_AFSEL_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);

    // Check for table 23.5 - to particularly choose which alternate function you want to use ...page 669
    // keep the digital function 1 to 15 in mind and write (1 to 15) in the respective 4 bits which were given
    GPIO_PORTA_PCTL_R |= (2 << 16) | (2 << 20) | (2 << 8) | (2 << 12);

    // Disable the digital functionality
    GPIO_PORTA_AMSEL_R &= ~((1 << 2) | (1 << 3) | (1 << 4) | (1 << 5));

    // Digital functions enable
    GPIO_PORTA_DEN_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);

    // TO ENABLE PULL UP FEATURE
    GPIO_PORTA_LOCK_R = 0x4C4F434B; // unlock write option in commit register
    GPIO_PORTA_CR_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
    GPIO_PORTA_PUR_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5); // Enable internal pull-up resistors for switches

    // now SSE bit must be clear before making any configurations in the spi protocol
    SSI0_CR1_R &= ~(1 << 1);

    // To make the spi protocol as a master , CPOL = CPHA = 0
    SSI0_CR1_R &= ~(1 << 2);

    // Clock Source - > main oscillator is the clock source
    // precision internal oscillator is the default clock source
    SSI0_CC_R = 0x00;

    // We need to set the prescale value since we have chosen main oscillator as the clock source
    SSI0_CPSR_R = 4; // meaning 16Mhz/4

    SSI2_CR0_R = 0x907;

    // Now the required configurations being done, we have to enable the spi module
    SSI0_CR1_R |= (1 << 1);

}

void switch_led_config(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for PORTF since LEDs in Port F
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock write option in commit register
    GPIO_PORTF_CR_R = 0x01;
    GPIO_PORTF_CR_R |= 0x10;
    GPIO_PORTF_PUR_R = 0x01; // Enable internal pull-up resistors for switches
    GPIO_PORTF_PUR_R |= 0x10;
    GPIO_PORTF_DEN_R = 0x1F; // Enable PORTF Pin 1, 2 and 3 as digital pins
    GPIO_PORTF_DIR_R = 0x0E; // Configure PORTF Pin 1, 2 and 3 as digital output pins. Pin 0 and Pin 4 are switches, no direction change.
}

void spi0_char_tx(unsigned char data)
{
    while (TX_EMPTY == 0); //send once the data buffer is free
    SSI0_DR_R = data;
    while (TX_RX_BUSY); //wait till the data is completely written into the fifo buffer
}

unsigned char spi0_char_rx(void)
{
    static unsigned char ch;
    while (RX_FLAG == 1);
    ch = SSI0_DR_R;
    return ch;
}
