#include "tm4c123xx.h"
#include "uart_header.h"
#include <stdio.h>
// Function prototypes initialize, transmit and read functions
void I2C3_Init(void);
void I2C3_Write_byte(int slave_address, unsigned char slave_memory_address,
                     unsigned char data);
int I2C3_read_byte(int slave_address, unsigned char slave_memory_address);
void delay(long k)
{
    while (k--)
        ;
}
int main(void)
{
    int x1, y1, z1, x2, y2, z2;
    uart0_config();
    I2C3_Init();  // PD0 (SCL), PD1 (SDA)

    // Configure accelerometer
    I2C3_Write_byte(0x53, 0x31, 0x03); // Set data format to full resolution
    I2C3_Write_byte(0x53, 0x2D, 0x08); // Set power mode to measure
    I2C3_Write_byte(0x53, 0x2C, 0x0A); // Set BW_rate
    I2C3_Write_byte(0x53, 0x27, 0x70); // Activity of x,y,z detection

    while (1)
    {
        x1 = y1 = z1 = x2 = y2 = z2 = 0;
        uart0_string_tx("\r\n");

        // Read X-axis
        x1 = I2C3_read_byte(0x53, 0x32);
//        x2 = I2C3_read_byte(0x53, 0x33);
        uart0_int_tx(x1);
//        uart0_char_tx(',');
//        uart0_int_tx(x2);
        uart0_char_tx(' ');
        uart0_char_tx(' ');

        // Read Y-axis
        y1 = I2C3_read_byte(0x53, 0x34);
//        y2 = I2C3_read_byte(0x53, 0x35);
        uart0_int_tx(y1);
 //       uart0_char_tx(',');
//        uart0_int_tx(y2);
        uart0_char_tx(' ');
        uart0_char_tx(' ');

        // Read Z-axis
        z1 = I2C3_read_byte(0x53, 0x36);
//        z2 = I2C3_read_byte(0x53, 0x37);
        uart0_int_tx(z1);
//        uart0_char_tx(',');
//        uart0_int_tx(z2);
        uart0_char_tx(' ');
        uart0_char_tx(' ');

        delay(2230000);
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
    // I2C3_MTPR_R = 0x07; //100kbps
    I2C3_MTPR_R = (7 << 6) | 4999;
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

int I2C3_read_byte(int slave_address, unsigned char slave_memory_address)
{
    static int data;
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

    return data; /* no error */
}
