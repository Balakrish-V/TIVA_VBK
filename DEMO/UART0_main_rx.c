/*
 * UART0_main_rx.c
 *
 *  Created on: 09-Feb-2024
 *      Author: TECQNIO
 */

#include "tm4c123xx.h"

#include "uart_header.h"

#include "delay.h"

int main()
{
        SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for PORTF since LEDs in Port F

        uart0_config();

        GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock write option in commit register
        GPIO_PORTF_CR_R = 0x01;
        GPIO_PORTF_CR_R |= 0x10;
        GPIO_PORTF_DIR_R |= 0x0E; // Configure PORTF Pin 1, 2 and 3 as digital output pins. Pin 0 and Pin 4 are switches, no direction change.
        GPIO_PORTF_DEN_R |= 0x0E; // Enable PORTF Pin 1, 2 and 3 as digital pins

        unsigned char data;

        while (1)
        {
            // if 1 -> Blue LED Glow
            // if 2 -> Green LED Glow

           // while(((UART0_FR_R>>6)&1) != 0 );  //change of pins

           while(rx_flag_0==0);
            data=UART0_DR_R;

            if(data == '1')
                GPIO_PORTF_DATA_R = LED_RED;
            else if(data == '2')
                GPIO_PORTF_DATA_R = LED_GREEN;
        }
 }
