#include "tm4c123xx.h"
#include "uart_header.h"
#include "stdio.h"
// Function prototypes initialize, transmit and read functions
void I2C3_Init(void);
void I2C3_Write_byte(int slave_address, unsigned char slave_memory_address,
                     unsigned char data);
unsigned char I2C3_read_byte(int slave_address,
                             unsigned char slave_memory_address);
void delay(long k)
{
    while (k--)
        ;
}
unsigned char sec=0,min=0,hr=0;
int main(void)
{
    //char time[12];
    uart0_config();
    I2C3_Init();
    I2C3_Write_byte(0x68, 0x00, 0x01);
    while (1)
    {
        sec = I2C3_read_byte(0x68, 0x00);
        delay(10000);
        uart0_char_tx(sec);
    }
}
// I2C initialization and GPIO alternate function configuration
void I2C3_Init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x00000008; // Enable the clock for port D
    SYSCTL_RCGCI2C_R |= 0x00000008; // Enable the clock for I2C 3
    GPIO_PORTD_DEN_R |= 0x03; // Assert DEN for port D
// Configure Port D pins 0 and 1 as I2C 3
    GPIO_PORTD_AFSEL_R |= 0x00000003;
    GPIO_PORTD_PCTL_R |= 0x00000033;
    GPIO_PORTD_ODR_R |= 0x00000002; // SDA (PD1 ) pin as open darin
    I2C3_MCR_R = 0x0010; // Enable I2C 3 master function
    I2C3_MTPR_R = 0x07;
}

// Receive one byte of data from I2C slave device
void I2C3_Write_byte(int slave_address, unsigned char slave_memory_address,
                     unsigned char data)
{
    I2C3_MSA_R = slave_address << 1;
    while ((I2C3_MCS_R & 1) == 1)
        ;
    I2C3_MDR_R = slave_memory_address;
    I2C3_MCS_R = 3; /* S-(saddr+w)-ACK-maddr-ACK */

    while (I2C3_MCS_R & 1)
        ;
    /* send data one byte at a time */

    I2C3_MDR_R = data; /* write the last byte */
    I2C3_MCS_R = 5; /* -data-ACK-P */
    while (I2C3_MCS_R & 1)
        ;
    while (I2C3_MCS_R & 0x40)
        ; /* wait until bus is not busy */
}

unsigned char I2C3_read_byte(int slave_address,
                             unsigned char slave_memory_address)
{
    static unsigned char data;
    /* send slave address and starting address */
    I2C3_MSA_R = slave_address << 1;
    while (I2C3_MCS_R & 1)
        ;
    I2C3_MDR_R = slave_memory_address;
    I2C3_MCS_R = 3; /* S-(saddr+w)-ACK-maddr-ACK */
    while (I2C3_MCS_R & 1)
        ;
    I2C3_MSA_R = (slave_address << 1) + 1; /* restart: -R-(saddr+r)-ACK */

    I2C3_MCS_R = 7; /* -data-NACK-P */

    while (I2C3_MCS_R & 1)
        ;
    data = I2C3_MDR_R; /* store the data received */

    while (I2C3_MCS_R & 1)
        ;
    while (I2C3_MDR_R & 0x40)
        ; /* wait until bus is not busy */

    return data; /* no error */
}
