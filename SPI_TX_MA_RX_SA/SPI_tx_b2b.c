/*
 * spi2_main.c
 *
 *  Created on: 14-Feb-2024
 *      Author: TECQNIO
 */
#include "tm4c123xx.h"
#include "spi_header.h"
#include "uart_header.h"
#include "delay.h"
int main()
{
    switch_led_config();
    spi2_config();
    unsigned char ch;
    while (1)
    {
        if (sw2 == 0)
        {
            delay(100000);
            spi2_char_tx('1');
        }
        else if (sw1 == 0)
        {
            delay(100000);
            spi2_char_tx('2');
        }
        ch = spi2_char_rx();
        uart0_char_tx(ch);
        if (ch == '1')
            GPIO_PORTF_DATA_R = LED_GREEN;
        else if (ch == '2')
            GPIO_PORTF_DATA_R = LED_BLUE;
    }
}

