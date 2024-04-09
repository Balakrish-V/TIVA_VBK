/*
 * adc_WORK.C
 *
 *  Created on: 27-Feb-2024
 *      Author: TECQNIO
 */
#include "tm4c123xx.h"
#include "uart_header.h"
#define LED_RED         0x02
#define LED_BLUE        0x04
#define LED_GREEN       0x08

unsigned int adc_result = 0;

void delay(long k)
{
    while(k--);
}
void uart1_led_config(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for PORTF since LEDs in Port F

    GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock write option in commit register
    GPIO_PORTF_DIR_R |= 0x0E; // Configure PORTF Pin 1, 2 and 3 as digital output pins. Pin 0 and Pin 4 are switches, no direction change.
    GPIO_PORTF_DEN_R |= 0x1F; // Enable PORTF Pin 1, 2 and 3 as digital pins

    SYSCTL_RCGCUART_R |= 1 << 1;  // Enable clock for UART1 Module
    SYSCTL_RCGCGPIO_R |= 1 << 2; // Enable clock for GPIO Module also - since UART in port C
    GPIO_PORTC_AFSEL_R |= (1 << 4) | (1 << 5); //for pin 15 - Port C 5(TX) , pin 16 - Port C 4(RX)
// Mode can be set in PCTL(port Control) register once the AFSEL corresponding bit is high
    GPIO_PORTC_PCTL_R |= (2 << 16) | (2 << 20); // Selecting PC4 and PC5(4 bits for a pin) for func. as UART and selecting functionality also
    GPIO_PORTC_DEN_R |= (1 << 4) | (1 << 5); // Digitilalizing the pins 0 and 1
// Disable uart before making the configurations
    UART1_CTL_R &= (~(1 << 0)) & (~(1 << 8)) & (~(1 << 9));
    UART1_IBRD_R = 104; // baud rate 104
    UART1_FBRD_R = 11;
    UART1_LCRH_R |= (0x3 << 5); //line control -> word length
    UART1_CC_R = 0x5; //clock select -> source of clock is precision internal oscillator
// next is enable UART, UART Tx Enable and UART Rx Enable
    UART1_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9);
}
int main()
{
    uart1_led_config();
    unsigned char rx;
    while (1)
    {
        rx = uart1_char_rx();
        if (rx == '1')
        {
            GPIO_PORTF_DATA_R = LED_GREEN;
            delay(100000);
            GPIO_PORTF_DATA_R = 0;
            delay(100000);
        }
        else if (rx == '2')
        {
            GPIO_PORTF_DATA_R = LED_RED;
        }
        else if (rx == '3')
        {
            GPIO_PORTF_DATA_R = LED_GREEN;
        }
    }
}

