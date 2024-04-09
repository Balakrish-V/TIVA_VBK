/*
 * uart_driver.c
 *
 *  Created on: 09-Feb-2024
 *      Author: TECQNIO
 */
#include "tm4c123xx.h"

#include "uart_header.h"

void uart0_config(void)
{
    SYSCTL_RCGCUART_R |= 1 << 0; // Enable clock for UART0 Module - USB cable itself supports UART Communication

    SYSCTL_RCGCGPIO_R |= 1 << 0; // Enable clock for GPIO Module also - since uart in port A

    GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1); //(Port A corresponding pin 0 and 1)

    // Mode can be set in PCTL(port Control) register once the AFSEL corresponding bit is high

    GPIO_PORTA_PCTL_R |= (1 << 0) | (1 << 4); // Selecting pin 0 and 1 for func. as UART and selecting functionality also

    GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1); // Digitilalizing the pins 0 and 1

    // Disable uart before making the configurations
    UART0_CTL_R &= (~(1 << 0)) & (~(1 << 8)) & (~(1 << 9));

    UART0_IBRD_R = 104; // baud rate 104
    UART0_FBRD_R = 11;

    UART0_LCRH_R |= (0x3 << 5); //line control -> word length

    UART0_CC_R = 0x5; //clock select -> source of clock is precision internal oscillator

    // next is enable UART, UART Tx Enable and UART Rx Enable
    UART0_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9);

}

void uart1_config(void)
{
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

    UART1_LCRH_R |= (0x3 << 5) | (1 << 4); //line control -> word length

    UART1_CC_R = 0x5; //clock select -> source of clock is precision internal oscillator

    // next is enable UART, UART Tx Enable and UART Rx Enable
    UART1_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9);

}

void uart3_config(void)
{
    SYSCTL_RCGCUART_R |= 1 << 3;  // Enable clock for UART1 Module

    SYSCTL_RCGCGPIO_R |= 1 << 2; // Enable clock for GPIO Module also - since UART3 also in port C

    GPIO_PORTC_AFSEL_R |= (1 << 7) | (1 << 6); //for pin 15 - Port C 5(TX) , pin 16 - Port C 4(RX)

    // Mode can be set in PCTL(port Control) register once the AFSEL corresponding bit is high

    GPIO_PORTC_PCTL_R |= (1 << 28) | (1 << 24); // Selecting PC4 and PC5(4 bits for a pin) for func. as UART and selecting functionality also

    GPIO_PORTC_DEN_R |= (1 << 7) | (1 << 6); // Digitilalizing the pins 0 and 1

    // Disable uart before making the configurations
    UART3_CTL_R &= (~(1 << 0)) & (~(1 << 8)) & (~(1 << 9));

    UART3_IBRD_R = 104; // baud rate 104
    UART3_FBRD_R = 11;

    UART3_LCRH_R |= (0x3 << 5); //line control -> word length and enable FIFO's

    UART3_CC_R = 0x5; //clock select -> source of clock is precision internal oscillator

    // next is enable UART, UART Tx Enable and UART Rx Enable
    UART3_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9);

}

void uart0_char_tx(unsigned char data)
{
    UART0_DR_R = data;
    while (tx_flag_0 == 0)
        ;
}

void uart0_int_tx(int temp)
{
    int i, c;
    unsigned char arr[10];
    if (temp == 0)
    {
        uart0_char_tx('0');
    }
    else
    {
        if (temp < 0) {
            uart0_char_tx('-');
            temp = -temp; // Convert negative number to positive
        }
        for (c = 0, i = temp; i; i /= 10)
            c++;
        arr[c] = 0;
        for (c = c - 1, i = temp; i; i /= 10, c--)
        {
            arr[c] = i % 10 + 48;
        }
        for (i = 0; arr[i]; i++)
        {
            uart0_char_tx(arr[i]);
        }
    }
}

void uart0_int_to_bcd(int bcd)
{
    int res;
    int high_digit = (bcd & 0xF0) >> 4; // Extracting the high digit (left 4 bits)
    int low_digit = bcd & 0x0F;       // Extracting the low digit (right 4 bits)

    // Check if the BCD digits are valid (0-9)
    if (high_digit >= 0 && high_digit <= 9 && low_digit >= 0 && low_digit <= 9)
    {
        // Combine the digits and return the result
        res = high_digit * 10 + low_digit;
        uart0_int_tx(res);
    }
    else
    {

    }
}

void uart1_char_tx(unsigned char data)
{
    UART1_DR_R = data;
    while (tx_flag_1 == 0)
        ;
}

void uart3_char_tx(unsigned char data)
{
    UART3_DR_R = data;
    while (tx_flag_3 == 0)
        ;
}

void uart0_string_tx(char *p)
{
    while (*p)
    {
        while (tx_flag_0 == 0)
            ;
        UART0_DR_R = *p;
        p++;
    }
}
void uart1_string_tx(unsigned char *p)
{
    while (*p)
    {
        while (tx_flag_1 == 0)
            ;
        UART1_DR_R = *p;
        p++;
    }
}
void uart3_string_tx(unsigned char *p)
{
    while (*p)
    {
        while (tx_flag_3 == 0)
            ;
        UART3_DR_R = *p;
        p++;
    }
}
unsigned char uart0_char_rx(void)
{
    static unsigned char ch;
    while (rx_flag_0 == 0)
        ; // collect info once it becomes to ONE
    ch = UART0_DR_R;
    uart0_char_tx(ch);
    return ch;
}

void uart0_hex_tx(unsigned char ch)
{
    unsigned char temp1, temp2;
    temp1 = ((ch & 0xF0) >> 4);
    temp2 = (ch & 0x0F);
    switch (temp1)
    {
    case 'A':
        uart0_char_tx('A');
        break;
    case 'B':
        uart0_char_tx('B');
        break;
    case 'C':
        uart0_char_tx('C');
        break;
    case 'D':
        uart0_char_tx('D');
        break;
    case 'E':
        uart0_char_tx('E');
        break;
    case 'F':
        uart0_char_tx('F');
        break;
    default:
        uart0_char_tx(temp1);
        break;
    }
    switch (temp2)
    {
    case 'A':
        uart0_char_tx('A');
        break;
    case 'B':
        uart0_char_tx('B');
        break;
    case 'C':
        uart0_char_tx('C');
        break;
    case 'D':
        uart0_char_tx('D');
        break;
    case 'E':
        uart0_char_tx('E');
        break;
    case 'F':
        uart0_char_tx('F');
        break;
    default:
        uart0_char_tx(temp2);
        break;
    }
}

unsigned char uart1_char_rx(void)
{
    unsigned char ch;
    while (rx_flag_1 == 0)
        ;
    ch = UART1_DR_R;
    uart1_char_tx(ch);
    return ch;
}

unsigned char uart3_char_rx(void)
{
    static unsigned char ch;
    while (rx_flag_3 == 0)
        ;
    ch = UART3_DR_R;
    uart3_char_tx(ch);
    return ch;
}

