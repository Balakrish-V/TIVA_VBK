#include "tm4c123xx.h"

void delay(long k)
{
    while (k--)
        ; // This is a simple busy-wait delay, consider using hardware timers for more accurate delays
}

int main()
{
    short int i = 0;
    // Enable PWM0 module clock
    SYSCTL_RCGCPWM_R |= (1 << 0); // Enable PWM0 module

    // Enable GPIO Port B clock
    SYSCTL_RCGCGPIO_R |= (1 << 1); // Enable clock for Port B

    // Wait for the PWM0 peripheral to be ready
    while (!(SYSCTL_PRPWM_R & (1 << 0)))
        ;

    // Wait for the GPIO Port B peripheral to be ready
    while (!(SYSCTL_PRGPIO_R & (1 << 1)))
        ;

    // Disable PWM divider
    SYSCTL_RCC_R &= ~(1 << 20); // Clear USEPWMDIV bit

    // Enable PWM output pin (PB4)
    GPIO_PORTB_AFSEL_R |= (1 << 4); // Enable alternate function for PB4
    GPIO_PORTB_PCTL_R |= (GPIO_PCTL_PB4_M0PWM2); // Configure PB4 as M0PWM2
    // GPIO_PORTB_DIR_R |= (1 << 4); // Direction set is not required as AFSEL will handle it
    GPIO_PORTB_DEN_R |= (1 << 4); // Enable digital function for PB4

    // Configure PWM generator 0
    PWM0_1_CTL_R &= (~(1 << 0));
    PWM0_1_CTL_R &= (~(1 << 1));
    PWM0_1_LOAD_R = 1600;
    PWM0_1_GENA_R |= (0x3) | (0x2 << 6);
    PWM0_1_CTL_R |= (1 << 0);
    PWM0_ENABLE_R |= (1<<6);

    while (1)
    {
        // Add any additional code or functionality here
        for (i = 0; i < 10; i++)
        {
            PWM0_0_CMPA_R = i;
            delay(10000);
        }
    }
}
