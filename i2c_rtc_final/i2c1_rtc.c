#include "tm4c123xx.h"
#include "uart_header.h"
#include <stdio.h>
// Function prototypes initialize, transmit and read functions
void I2C1_Init(void);
void I2C1_Write_byte(int slave_address, unsigned char slave_memory_address,
                     unsigned char data);
int I2C1_read_byte(int slave_address, unsigned char slave_memory_address);
void delay(long k)
{
    while (k--)
        ;
}
int main(void)
{
    int sec, min, hr, day, month, year;
    uart0_config();
    I2C1_Init();  //pd0-scl , pd1-sda
    I2C1_Write_byte(0x68, 0x00, 0x00);
    I2C1_Write_byte(0x68, 0x01, 0x39);
    I2C1_Write_byte(0x68, 0x02, ((1 << 6) | (1 << 5) | 0x06));/*((1<<4)|(1<<1)|(1<<0))*/
    I2C1_Write_byte(0x68, 0x03, 0x2);
    I2C1_Write_byte(0x68, 0x04, 0x27);
    I2C1_Write_byte(0x68, 0x05, 0x12);
    I2C1_Write_byte(0x68, 0x06, 0x24);
    while (1)
    {
        uart0_string_tx("\r\n");
        hr = I2C1_read_byte(0x68, 0x02);
        uart0_int_to_bcd(hr);
        uart0_char_tx(':');
        min = I2C1_read_byte(0x68, 0x01);
        uart0_int_to_bcd(min);
        uart0_char_tx(':');
        sec = I2C1_read_byte(0x68, 0x00);
        uart0_int_to_bcd(sec);
        uart0_string_tx("\r\n");
        day = I2C1_read_byte(0x68, 0x04);
        uart0_int_to_bcd(day);
        uart0_char_tx('/');
        month = I2C1_read_byte(0x68, 0x05);
        uart0_int_to_bcd(month);
        uart0_char_tx('/');
        year = I2C1_read_byte(0x68, 0x06);
        uart0_int_to_bcd(year);
        delay(2230000);
    }
}
// I2C initialization and GPIO alternate function configuration
void I2C1_Init(void)
{
    SYSCTL_RCGCGPIO_R |= (1 << 0); // Enable the clock for port A
    SYSCTL_RCGCI2C_R |= (1 << 1); // Enable the clock for I2C 1
    GPIO_PORTA_DEN_R |= (1 << 6) | (1 << 7); // Assert DEN for port A
// Configure Port D pins 0 and 1 as I2C 3
    GPIO_PORTA_AFSEL_R |= (1 << 7) | (1 << 6);
    GPIO_PORTA_PCTL_R |= (3 << 24) | (3 << 28);
    GPIO_PORTA_ODR_R |= (1 << 7); // SDA (PD1 )
    I2C1_MCR_R = 0x0010; // Enable I2C 1 master function
    I2C1_MTPR_R = 0x07; //100kbps
}

// Receive one byte of data from I2C slave device
void I2C1_Write_byte(int slave_address, unsigned char slave_memory_address,
                     unsigned char data)
{
    I2C1_MSA_R = slave_address << 1;
    while ((I2C1_MCS_R & 1) == 1)
        ;
    I2C1_MDR_R = slave_memory_address;
    I2C1_MCS_R = 3; /* S-(saddr+w)-ACK-maddr-ACK */

    while (I2C1_MCS_R & 1)
        ;
    /* send data one byte at a time */

    I2C1_MDR_R = data; /* write the last byte */
    I2C1_MCS_R = 5; /* -data-ACK-P */
    while (I2C1_MCS_R & 1)
        ;
    while (I2C1_MCS_R & 0x40)
        ; /* wait until bus is not busy */
}

int I2C1_read_byte(int slave_address, unsigned char slave_memory_address)
{
    static int data;
    /* send slave address and starting address */
    I2C1_MSA_R = slave_address << 1;
    while (I2C1_MCS_R & 1)
        ;
    I2C1_MDR_R = slave_memory_address;
    I2C1_MCS_R = 3; /* S-(saddr+w)-ACK-maddr-ACK */
    while (I2C1_MCS_R & 1)
        ;
    I2C1_MSA_R = (slave_address << 1) + 1; /* restart: -R-(saddr+r)-ACK */

    I2C1_MCS_R = 7; /* -data-NACK-P */

    while (I2C1_MCS_R & 1)
        ;
    data = I2C1_MDR_R; /* store the data received */

    while (I2C1_MCS_R & 1)
        ;

    return data; /* no error */
}
