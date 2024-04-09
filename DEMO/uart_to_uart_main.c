/*
 * uart_to_uart_main.c
 *
 *  Created on: 12-Feb-2024
 *      Author: TECQNIO
 */
#include "tm4c123xx.h"
#include "uart_header.h"
#include "delay.h"
int main()
{
    SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for PORTF since LEDs in Port F
    GPIO_PORTF_DIR_R |= 0x0E; // Configure PORTF Pin 1, 2 and 3 as digital output pins.
    GPIO_PORTF_DEN_R |= 0x0E; // Enable PORT F Pin 1, 2 and 3 as digital pins

    uart0_config();

    unsigned char ch[10]= "12345";
    while(1)
    {
        uart0_string_tx(ch);
    }
}

