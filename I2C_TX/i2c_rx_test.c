#include "tm4c123xx.h"
#include <stdio.h>
// Function prototypes initialize, transmit and read functions
void I2C3_Init(void);
int I2C3_read_byte(int slave_address);
int data;
void delay(long k)
{
    while (k--)
        ;
}
void IntDefaultHandler(void)
{
    if(I2C0_SRIS_R & 1)
        data = I2C3_read_byte(0x22);
}
void int_handler(void)
{
    I2C0_SMIS_R |= (1 << 3);
}
int main(void)
{
    I2C3_Init();  //pd0-scl , pd1-sda
    int_handler();
    while (1)
    {

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
    I2C3_MTPR_R = 0x07; //100kbps
    I2C3_SOAR_R = 0X22; // SLAVE ADDress = 22
    I2C3_MCR_R = (1 << 5); // Enable I2C 3 slave/mstr function
    I2C3_SCSR_R = (1 << 0); //enable i2c slave operation
}

int I2C3_read_byte(int slave_address)
{
    static int data;
    /* send slave address and starting address */
    I2C3_MSA_R = slave_address << 1;
    while (I2C3_MCS_R & 1)
        ;

    I2C3_MCS_R = 7; /* -data-NACK-P */

    data = I2C3_SDR_R; /* store the data received */

    while (I2C3_MCS_R & 1)
        ;

    return data; /* no error */
}
