/*
 * spi_header.h
 *
 *  Created on: 15-Feb-2024
 *      Author: TECQNIO
 */

#ifndef SPI_HEADER_H_
#define SPI_HEADER_H_
#define spi_2_flag ((SSI2_SR_R>>1)&1)
void spi2_config(void);
void spi2_char_tx(unsigned char data);

#endif /* SPI_HEADER_H_ */
