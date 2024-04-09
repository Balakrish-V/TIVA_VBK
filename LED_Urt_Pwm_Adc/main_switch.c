#include "tm4c123xx.h"
#include "delay.h"

int main(void)
{
//    SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for PORTF since LEDs in Port F

    GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock write option in commit register
    GPIO_PORTF_CR_R = 0x01;
    GPIO_PORTF_CR_R |= 0x10;
    GPIO_PORTF_PUR_R = 0x01; // Enable internal pull-up resistors for switches
    GPIO_PORTF_PUR_R |= 0x10;
    GPIO_PORTF_DEN_R = 0x1F; // Enable PORTF Pin 1, 2 and 3 as digital pins
    GPIO_PORTF_DIR_R = 0x0E; // Configure PORTF Pin 1, 2 and 3 as digital output pins. Pin 0 and Pin 4 are switches, no direction change.

    int sw1, sw2, flag1 = 0, flag2 = 0;

    while (1)
    {
        sw1 = (GPIO_PORTF_DATA_R >> 4) & 1; // Read switch 1
        sw2 = (GPIO_PORTF_DATA_R >> 0) & 1; // Read switch 2

        if (sw1 == 0)
        {
            flag1 = 1;
            flag2 = 0;
        }
        else if (sw2 == 0)
        {
            flag2 = 1;
            flag1 = 0;
        }

        while (flag1 && !flag2)
        {
            GPIO_PORTF_DATA_R = LED_GREEN;
            delay(1000000); // Adjust delay for visibility
            GPIO_PORTF_DATA_R = LED_RED; // Turn off LED
            delay(1000000); // Adjust delay for visibility

            if(!((GPIO_PORTF_DATA_R >> 0) & 1))
                            break; // Exit loop if sw2 is not pressed
        }

        while (flag2 && !flag1)
        {
            GPIO_PORTF_DATA_R = LED_GREEN;
            delay(1000000); // Adjust delay for visibility
            GPIO_PORTF_DATA_R = LED_BLUE; // Turn off LED
            delay(1000000); // Adjust delay for visibility

            if(!((GPIO_PORTF_DATA_R >> 4) & 1))
                            break; // Exit loop if sw2 is not pressed
        }
    }
}
