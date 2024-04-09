/*
 * spi_header.h
 *
 *  Created on: 15-Feb-2024
 *      Author: TECQNIO
 */

#ifndef SPI_HEADER_H_
#define SPI_HEADER_H_

#define spi_0_flag ((SSI0_SR_R>>1)&1)
#define sw1 ((GPIO_PORTF_DATA_R >> 4) & 1) // Read switch 1
#define sw2 ((GPIO_PORTF_DATA_R >> 0) & 1) // Read switch 2

#define TX_EMPTY ((SSI0_SR_R>>0)&1)  //1 -> empty
#define TX_FIFO ((SSI0_SR_R>>1)&1)   //0 -> tx_fifo is full
#define RX_FIFO ((SSI0_SR_R>>2)&1)   //0 -> rx_fifo is empty
#define RX_FLAG ((SSI0_SR_R>>3)&1)   //1 -> receive once this bit becomes 1
#define TX_RX_BUSY ((SSI0_SR_R>>4)&1)//1 -> TX or RX
void spi0_char_tx(unsigned char data);
void spi2_config(void);
void spi2_char_tx(unsigned char data);
void switch_led_config(void);
unsigned char spi2_char_rx(void);
void spi0_config(void);
void switch_led_config(void);

#endif /* SPI_HEADER_H_ */
