#include "tm4c123xx.h"
#include <stdio.h>
// Function prototypes initialize, transmit and read functions
void I2C3_Init(void);
void I2C3_Write_byte(int slave_address, unsigned char data);
void delay(long k)
{
    while (k--)
        ;
}

int main(void)
{
    I2C3_Init();  //pd0-scl , pd1-sda
    while (1)
    {
        I2C3_Write_byte(0x11, 'A');
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
    I2C3_MTPR_R = 0x07; //100kbps
}

// Receive one byte of data from I2C slave device
void I2C3_Write_byte(int slave_address, unsigned char data)
{
    while ((I2C3_MCS_R & 1) == 1)
            ;
    I2C3_MSA_R = slave_address << 1;
    I2C3_MCS_R = 3; /* S-(saddr+w)-ACK */

    while (I2C1_MCS_R & 1)
            ;
    I2C3_MDR_R = data; /* write the last byte */
    I2C3_MCS_R = 5; /* -data-ACK-P */

    while (I2C3_MCS_R & 0x40)
        ; /* wait until bus is not busy */
}

