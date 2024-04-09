/*
 * UART_main.c
 *
 *  Created on: 09-Feb-2024
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
        uart3_config();
        uart0_config();

        while (1)
        {
                if(((GPIO_PORTF_DATA_R >> 0) & 1) == 0 )
                {
                    // Before sending any data make sure no data is already present in those registers

                    //in the flag register there is a monitor only bit field to check if FIFO is full or not
                    //wait until TXFF bit becomes 0 and then start writing in this buffer

                    delay(1000000);//to avoid de-bouncing in switch press
                    uart0_char_tx('1');
                }
                else if(((GPIO_PORTF_DATA_R >> 4) & 1) == 0)
                {
                    delay(1000000);//to avoid de-bouncing in switch press
                    uart0_char_tx('2');
                }
          }
 }




