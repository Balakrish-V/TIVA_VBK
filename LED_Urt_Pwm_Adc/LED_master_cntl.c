#include "tm4c123xx.h"
#include "uart_header.h"
#include <stdio.h>
#include <string.h>

short int brightness = 16000, k = 0;

void delay(long k)
{
    while (k--)
        ;
}

void UART0_init(void)
{
    SYSCTL_RCGCUART_R |= 1 << 0;
    SYSCTL_RCGCGPIO_R |= 1 << 0;
    GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1);
    // Mode can be set in PCTL(port Control) register once the AFSEL corresponding bit is high
    GPIO_PORTA_PCTL_R |= (1 << 0) | (1 << 4);
    GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);
    // Disable uart before making the configurations
    UART0_CTL_R &= (~(1 << 0)) & (~(1 << 8)) & (~(1 << 9));
    UART0_IBRD_R = 104; // baud rate 104
    UART0_FBRD_R = 11;
    UART0_LCRH_R |= (0x3 << 5); //line control -> word length
    UART0_CC_R = 0x5; //clock select -> source of clock is precision internal oscillator
    // next is enable UART, UART Tx Enable and UART Rx Enable
    UART0_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9);
}

unsigned char UART0_char_rx(void)
{
    static unsigned char ch;
    while (rx_flag_0 == 0)
        ;
    ch = UART0_DR_R;

    if ((brightness < 16000) && (brightness != 0))
    {
        if ((ch == 'i') || (ch == 'd'))
        {
            if (ch == 'i')
            {
                if ((brightness - 1600) != 0)
                {
                    brightness -= 1600;
                    UART0_char_tx('+');
                }
                else
                {
                    UART0_string_tx("Decrease Brightness\r\n");
                }
            }
            else if (ch == 'd')
            {
                if ((brightness + 1600) != 16000)
                {
                    brightness += 1600;
                    UART0_char_tx('-');
                }
                else
                {
                    UART0_string_tx("Increase Brightness\r\n");
                }
            }
        }
    }
    else
    {
        if (brightness == 0)
            brightness = 1600;

        if (brightness >= 16000)
            brightness = 14400;
    }
    return ch;
}

void UART0_char_tx(unsigned char data)
{
    UART0_DR_R = data;
    while (tx_flag_0 == 0)
        ;
}

void UART0_string_tx(unsigned char *p)
{
    while (*p != '\0')
    {
        UART0_DR_R = *p;
        while (tx_flag_0 == 0)
            ;
        p++;
    }
}

void UART0_string_rx(unsigned char *p, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        while (rx_flag_0 == 0)
            ;
        p[i] = UART0_DR_R;
        UART0_char_tx(p[i]);
        if (p[i] == '\r')
            break;
    }
    p[i] = '\0';
}
/*
 void UART0_integer(int num)
 {
 unsigned char a[10];
 sprintf(a,"%d",num);
 UART0_string_tx(a);
 }
 */
void led_config(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for PORTF since LEDs in Port F
    GPIO_PORTF_DEN_R = 0x1F; // Enable PORTF Pin 1, 2 and 3 as digital pins
    GPIO_PORTF_DIR_R = 0x0E; // Configure PORTF Pin 1, 2 and 3 as digital output pins. Pin 0 and Pin 4 are switches, no direction change.
}

void pwm_config(void)
{
    SYSCTL_RCGCPWM_R |= (1 << 1); //PWM1 MODULE CHANNEL 6
    SYSCTL_RCC_R &= ~(1 << 20); // USEPWMDIV
    GPIO_PORTF_AFSEL_R |= (1 << 2); //pf2
    GPIO_PORTF_PCTL_R |= 0x00000500;
    GPIO_PORTF_DEN_R |= (1 << 2);
    PWM1_3_CTL_R &= ~(1 << 0); //disable pwm - ch6 of pwm1 block available in pwmn - n-3
    PWM1_3_CTL_R &= ~(1 << 1); // up/down
    PWM1_3_LOAD_R = 16000;  //16000 -1khz
//    PWM1_3_CMPA_R = 8000; //duty cycle = 50% //comparator A
    PWM1_3_GENA_R |= (0x3) | (0x2 << 6); // 3:2, 7:6 //what action to do
    PWM1_3_CTL_R |= (1 << 0); //enable
    /*
     PWM1_3_CMPA_R = i;
     */
}

int main()
{
    unsigned char lc[10], opt;
    UART0_init();
    led_config();
    pwm_config();
    UART0_string_tx("Choose from the below\r\n");
    UART0_string_tx("LED Control 1.UART\r\n2.PWM\r\n");
    opt = UART0_char_rx();
    if (opt == '1')
    {
        while (1)
        {
            A:
            UART0_string_tx("\r\nLight Colour:\r\n");
            UART0_string_rx(lc, 10);
            switch (lc[0])
            {
            case 'r':
                GPIO_PORTF_DATA_R = LED_RED;
                break;
            case 'b':
                GPIO_PORTF_DATA_R = LED_BLUE;
                break;
            case 'g':
                GPIO_PORTF_DATA_R = LED_GREEN;
                break;
            default:
                UART0_string_tx("Not an option\r\n");
                goto A;
            }
        }
    }
    else if (opt == '2')
    {
        while (!k)
        {
            UART0_string_tx("Press 'i' to increase and 'd' to decrease\r\n");
            k++;
        }
        while (1)
        {
            PWM1_ENABLE_R |= (1 << 6);
            PWM1_3_CMPA_R = brightness;
            UART0_char_rx();
        }
    }
}
