#include "tm4c123xx.h"
void delay(long k)
{
    while (k--)
        ;
}
int main()
{
    short int i;
    SYSCTL_RCGCPWM_R |= (1 << 1); //PWM1 MODULE CHANNEL 6
    SYSCTL_RCGCGPIO_R |= (1 << 5); //SYS CLOCK port f
    SYSCTL_RCC_R &= ~(1 << 20); // USEPWMDIV

    //pf2 ->blue led internally mapped to pwm output since pf2->blue led
    //if we want to use pwm channel any other than the led connected pins pf2,3,4 use another separte led
    GPIO_PORTF_AFSEL_R |= (1 << 2);
    GPIO_PORTF_PCTL_R |= 0x00000500;
    GPIO_PORTF_DEN_R |= (1 << 2);

    //pwm1ch6
    PWM1_3_CTL_R &= ~(1 << 0); //disable pwm - ch6 of pwm1 block available in pwmn - n-3
    //down counter
    PWM1_3_CTL_R &= ~(1 << 1);
    /* pwm clk =16mhz */
    /* pwm gen freq =10khz  ->we set this */
    // write the load value in such a way that, the timer counts exactly 10khz time period */
    /* t=((1/10000)*1000) == 0.1ms (1/10khz)*/
    /* pwm counter takes 100us to count from 0 to 1600 */
    // the on and off time totally will be 0.1ms for 10khz frequency
    /* 0.1ms = ON TIME + OFF TIME */
    /* LOAD VALUE SHOULD BE IN SUCH A WAY THAT TO COUNT FROM 0 to SET VALUE or vice versa within 0.1ms */
    // time for one tick of pwm clock = 1/16mhz = 0.0625 microseconds \\this is constant
    // load value = (Required time period)/(time for one tick of timer)
    // load value = (100us)/(0.0625us) = 1600
    // adjust this value for different glowing speeds
    PWM1_3_LOAD_R = 16000;  //16000 -1khz
    //now set duty cycle
    //0.1ms => 100us = on time + off time == 50us - compare reg
   // PWM1_3_CMPA_R = 8000; //duty cycle = 50% //comparator A
    //what should i do when the pwm counter reaches from 0->1600 or vice versa?
    //say pwm counter counted from 0 to 1600..now at 1600 high --here
    //by 800 it is off --so no need to program action when it reaches 0 -> rollover happens
    //drive low when comparator a(800) drive high when 1600
    //1600->800 = low when counting
    PWM1_3_GENA_R |= (0x3) | (0x2 << 6); // 3:2, 7:6
    PWM1_3_CTL_R |= (1 << 0); //enable

    //direction
    //pwm1ch6 as output
    PWM1_ENABLE_R |= (1 << 6);
    while (1)
    {
        //now we need to vary the duty cycle in run time
        for (i = 0; i < 16000; i++)
        {

            PWM1_3_CMPA_R = i;
            delay(10000);
        }
        delay(1000000);

        for (i = 16000; i > 0; i--)
        {

            PWM1_3_CMPA_R = i;
            delay(1000);
        }
        delay(1000000);

    }
}
