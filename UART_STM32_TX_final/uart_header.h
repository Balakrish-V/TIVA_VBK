/*
 * uart_header.h
 *
 *  Created on: 09-Feb-2024
 *      Author: TECQNIO
 */

#ifndef UART_HEADER_H_
#define UART_HEADER_H_

void uart0_config(void);
void uart1_config(void);
void uart3_config(void);

void uart0_char_tx(unsigned char data);
void uart1_char_tx(unsigned char data);
void uart3_char_tx(unsigned char data);

void uart0_string_tx(unsigned char *p);
void uart1_string_tx(unsigned char *p);
void uart3_string_tx(unsigned char *p);

unsigned char uart0_char_rx(void);
unsigned char uart1_char_rx(void);
unsigned char uart3_char_rx(void);

#define rx_flag_0 ((UART0_FR_R>>6)&1)
#define rx_flag_1 ((UART1_FR_R>>6)&1)
#define rx_flag_3 ((UART3_FR_R>>6)&1)

#define tx_flag_0 ((UART0_FR_R>>7)&1)
#define tx_flag_1 ((UART1_FR_R>>7)&1)
#define tx_flag_3 ((UART3_FR_R>>7)&1)
#define tx_flag_5 ((UART5_FR_R>>7)&1)

#endif /* UART_HEADER_H_ */
