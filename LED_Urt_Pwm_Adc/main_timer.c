#include "tm4c123xx.h"
#include "delay.h"
#include "timer.h"

int main(void)
 {
    SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for PORTF since LEDs in Port F

    //  Enable Clock for Timer 1
        //  We are using 16 bit wide Timer
        SYSCTL_RCGCTIMER_R|=(1<<1);

    GPIO_PORTF_DEN_R = 0x0E; // Enable PORTF Pin 1, 2 and 3 as digital pins
    GPIO_PORTF_DIR_R = 0x0E; // Configure PORTF Pin 1, 2 and 3 as digital output pins. Pin 0 and Pin 4 are switches, no direction change.

    GPIO_PORTF_DATA_R=LED_RED;
    delay(1000000);
    GPIO_PORTF_DATA_R=LED_BLUE;
    delay(1000000);

   while(1)
   {
       if((TIMER1_RIS_R & 0x00000001)==1)
       {
           GPIO_PORTF_DATA_R = LED_GREEN;
           delay(1000000);
           TIMER1_ICR_R |= (1<<0);
       }
       else
       {
           GPIO_PORTF_DATA_R=LED_RED;
           delay(1000000);
           timer_initialize();
       }
   }
}
