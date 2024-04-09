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
    spi0_config();
    while (1)
    {
        spi0_char_tx('1');
        delay(1000);
    }
}

