/*
 * spi2_main.c
 *
 *  Created on: 14-Feb-2024
 *      Author: TECQNIO
 */
#include "tm4c123xx.h"
#include "spi_header.h"
#include "delay.h"
int main()
{
    SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for PORTF since LEDs in Port F

    GPIO_PORTF_DEN_R = 0x0E; // Enable PORTF Pin 1, 2 and 3 as digital pins
    GPIO_PORTF_DIR_R = 0x0E; // Configure PORTF Pin 1, 2 and 3 as digital output pins. Pin 0 and Pin 4 are switches, no direction change.

    spi2_config();

    while(1)
    {
        spi2_char_tx('A');
    }
}

