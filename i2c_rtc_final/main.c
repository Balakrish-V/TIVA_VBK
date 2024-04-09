#include "tm4c123xx.h"

// Function prototypes initialize, tranmit and rea functions
void I2C3_Init(void);
char I2C3_Write_Multiple(int slave_address, char slave_memory_address,
                         int bytes_count, char *data);
char I2C3_read_Multiple(int slave_address, char slave_memory_address,
                        int bytes_count, char *data);

int main(void)
{
    char *k;
    I2C3_Init();
    I2C3_Write_Multiple(0x68, 0x01, 1, "1");
    while (1)
    {
        I2C3_read_Multiple(0x68, 0x01, 1, k);
    }
}
// I2C intialization and GPIO alternate function configuration
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

static int I2C_wait_till_done(void)
{
    while (I2C3_MCS_R & 1)
        ;
    return I2C3_MCS_R & 0xE; /* return I2C error code, 0 if no error*/
}
// Receive one byte of data from I2C slave device
char I2C3_Write_Multiple(int slave_address, char slave_memory_address,
                         int bytes_count, char *data)
{
    char error;
    if (bytes_count <= 0)
        return -1; /* no write was performed */
    I2C3_MSA_R = slave_address << 1;
    I2C3_MDR_R = slave_memory_address;
    I2C3_MCS_R = 3; /* S-(saddr+w)-ACK-maddr-ACK */

    error = I2C_wait_till_done(); /* wait until write is complete */
    if (error)
        return error;

    /* send data one byte at a time */
    while (bytes_count > 1)
    {
        I2C3_MDR_R = *data++; /* write the next byte */
        I2C3_MCS_R = 1; /* -data-ACK- */
        error = I2C_wait_till_done();
        if (error)
            return error;
        bytes_count--;
    }

    /* send last byte and a STOP */
    I2C3_MDR_R = *data++; /* write the last byte */
    I2C3_MCS_R = 5; /* -data-ACK-P */
    error = I2C_wait_till_done();
    while (I2C3_MCS_R & 0x40)
        ; /* wait until bus is not busy */
    if (error)
        return error;
    return 0; /* no error */
}

char I2C3_read_Multiple(int slave_address, char slave_memory_address,
                        int bytes_count, char *data)
{
    char error;

    if (bytes_count <= 0)
        return -1; /* no read was performed */

    /* send slave address and starting address */
    I2C3_MSA_R = slave_address << 1;
    I2C3_MDR_R = slave_memory_address;
    I2C3_MCS_R = 3; /* S-(saddr+w)-ACK-maddr-ACK */
    error = I2C_wait_till_done();
    if (error)
        return error;

    I2C3_MSA_R = (slave_address << 1) + 1; /* restart: -R-(saddr+r)-ACK */

    if (bytes_count == 1) /* if last byte, don't ack */
        I2C3_MCS_R = 7; /* -data-NACK-P */
    else
        /* else ack */
        I2C3_MCS_R = 0xB; /* -data-ACK- */
    error = I2C_wait_till_done();
    if (error)
        return error;

    *data++ = I2C3_MDR_R; /* store the data received */

    if (--bytes_count == 0) /* if single byte read, done */
    {
        while (I2C3_MDR_R & 0x40)
            ; /* wait until bus is not busy */
        return 0; /* no error */
    }

    /* read the rest of the bytes */
    while (bytes_count > 1)
    {
        I2C3_MCS_R = 9; /* -data-ACK- */
        error = I2C_wait_till_done();
        if (error)
            return error;
        bytes_count--;
        *data++ = I2C3_MDR_R; /* store data received */
    }

    I2C3_MCS_R = 5; /* -data-NACK-P */
    error = I2C_wait_till_done();
    *data = I2C3_MDR_R; /* store data received */
    while (I2C3_MCS_R & 0x40)
        ; /* wait until bus is not busy */

    return 0; /* no error */
}
