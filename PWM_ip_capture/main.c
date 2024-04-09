#include "tm4c123xx.h"
#include <stdio.h>

void Delay(long k)
{
    while (k--)
        ;
}

int Timer0ACapture_PulseWidth(void);
void Timer0ACapture_init(void);

/* global variables to store and display pulse width or duration */
unsigned int time_period,frequency;
unsigned int risingEdge, fallingEdge;
float duty_cycle,d;

int main(void)
{
    Timer0ACapture_init();
    while (1)
    {
        time_period = Timer0ACapture_PulseWidth();
        // This difference will give the pulse duration in terms of number of clocks.
        /* In order to get pulse duration in the time domain,
           multiply the measured time difference value with the inverse of the operating frequency of Timer that is 16MHz.  */
        frequency = 16000000 /(2*time_period);
        duty_cycle = (((float)time_period * 62.5) / 1000000)*100;
        //pwm clock = 1/16mhz = 0.0625 microseconds \\this is constant
        Delay(1000);
    }
}

void Timer0ACapture_init(void)
{
    SYSCTL_RCGCTIMER_R |= 1;
    SYSCTL_RCGCGPIO_R |= 2;

    GPIO_PORTB_DIR_R &= ~(1 << 6);
    GPIO_PORTB_DEN_R |= (1 << 6);
    GPIO_PORTB_AFSEL_R |= (1 << 6);
    GPIO_PORTB_PCTL_R &= ~0x0F000000;
    GPIO_PORTB_PCTL_R |= 0x07000000;

    TIMER0_CTL_R &= ~(1 << 0);
    TIMER0_CFG_R = 0x4;  //16 bit mode
    TIMER0_TAMR_R = 0x17; // up counter
    TIMER0_CTL_R |= (1 << 3) | (1 << 2);  //capture rising and falling edge
    TIMER0_CTL_R |= (1 << 0);
}

/* This function captures consecutive rising and falling edges of a periodic signal */
int Timer0ACapture_PulseWidth(void)
{
    while (1)  //this while(1) is needed since we need to monitor the high,low continuously until the high is detected.
    {
        TIMER0_ICR_R = 4;  //clear timer capture flag
        while ((TIMER0_RIS_R & (1 << 2)) == 0);   //wait for interrupt to set when high detected
        if (GPIO_PORTB_DATA_R & (1 << 6))   //if any rising edge is detected on the PB6 Pin
        {
            risingEdge = TIMER0_TAV_R;    // if yes start counting
            TIMER0_ICR_R = 4;    // clear timer capture flag
            while ((TIMER0_RIS_R & (1 << 2)) == 0);  //wait for previous interrupt to set when low detected
            fallingEdge = TIMER0_TAV_R;
            return (fallingEdge - risingEdge) & 0x00FFFFFF;  //24 bit timer. 2 power 24 is 16,777,215
        }
    }
}

