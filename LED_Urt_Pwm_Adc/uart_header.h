/*
 * uart_header.h
 *
 *  Created on: 09-Feb-2024
 *      Author: TECQNIO
 */

#ifndef UART_HEADER_H_
#define UART_HEADER_H_

void UART0_config(void);

void UART0_char_tx(unsigned char data);

void UART0_string_tx(unsigned char *p);

unsigned char UART0_char_rx(void);

#define rx_flag_0 ((UART0_FR_R>>6)&1)

#define tx_flag_0 ((UART0_FR_R>>7)&1)

#endif /* UART_HEADER_H */
