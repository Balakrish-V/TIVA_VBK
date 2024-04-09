/*
 * uart_to_uart_main.c
 *
 *  Created on: 12-Feb-2024
 *      Author: TECQNIO
 */

#include "tm4c123xx.h"

#include "uart_header.h"

#include"delay.h"

int main()
{
        SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for PORTF since LEDs in Port F

        GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock write option in commit register
        GPIO_PORTF_CR_R = 0x01;
        GPIO_PORTF_CR_R |= 0x10;
        GPIO_PORTF_DIR_R |= 0x0E; // Configure PORTF Pin 1, 2 and 3 as digital output pins. Pin 0 and Pin 4 are switches, no direction change.
        GPIO_PORTF_PUR_R = 0x01; // Enable internal pull-up resistors for switches
        GPIO_PORTF_PUR_R |= 0x10;
        GPIO_PORTF_DEN_R |= 0x1F; // Enable PORTF Pin 1, 2 and 3 as digital pins

        uart1_config();

        unsigned char ch;

        while (1)
        {
            ch = uart1_char_rx();
            if(ch=='1')
                  GPIO_PORTF_DATA_R = LED_GREEN;
        }
 }

