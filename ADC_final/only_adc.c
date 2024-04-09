#include "tm4c123xx.h"

#define LED_RED         0x02
#define LED_BLUE        0x04
#define LED_GREEN       0x08
#define DONE           ((ADC1_RIS_R>>3)&1)

unsigned int adc_result = 0;

void delay(long j)
{
    while (j--)
        ;
}

void adc_conf_intr(void)
{
    // NO NEED TO CONFIGURE CLOCK-> ADC is 16MHz
    SYSCTL_RCGCADC_R |= (1 << 1); //adc1 ..only 2 adcs are there
    // connect pe1 to POT
    SYSCTL_RCGCGPIO_R |= (1 << 4) | (1 << 5); //FOR ADC and FOR LED (port e anf f)
    // pot pe1 must be made input pin

    // port f
    GPIO_PORTF_DEN_R |= 0xff; //for leds
    GPIO_PORTF_DIR_R |= 0xff; //for leds -> output
    GPIO_PORTF_AFSEL_R = 0x00; //disable all the other functionalities

    // port e
    //for analog input pin have to select the alternate funtionality
    GPIO_PORTE_AFSEL_R |= (1 << 1);
    GPIO_PORTE_DEN_R &= ~(1 << 1); //ANALOG signal input pe1->analog ip
    // if any pin is used as an analog pin, then we need to enable that pin in amsel register
    GPIO_PORTE_AMSEL_R = (1 << 1); //DISABLE the digital func. by setting this bit and enabling the analog funtionality
    GPIO_PORTE_DIR_R &= ~(1 << 1); //for pot //we are giving the input to this pin

    //now we need to enable the sample sequencer
    ADC1_ACTSS_R &= ~(1 << 3); //1ST Disable it  ..active sample sequencer
    // why 3rd sampler above?
    // because we are sampling from only one pin
    // so we don't need to use any and other sampling sequnecer

    // next select trigger. trigger is what that starts our conversion
    // continuous trigger means as soon as the adc starts, it will start sampling
    // we can trigger it with timer or we can trigger it with the interupt
    // sample sequencer 3, so 15:12
    ADC1_EMUX_R = (0xF << 12); // ..event multiplexer..to select the event that triggers

    // when using pwm as trigger source -> use adc source select register
    // adc signal table
    // I am using analog input 2 --> AIN2
    ADC1_SSMUX3_R = 2; // sample sequence input select specifies which analog channel we use

    // sample seq control 3 register
    // here i use interrupt to check if the conversion has ended
    ADC1_SSCTL3_R = 0x6;

    // ENABLE IT
    ADC1_ACTSS_R |= (1 << 3);

}
int main()
{
    int adc_result;
    adc_conf_intr();
    while (1)
    {
        ADC1_PSSI_R |= (1 << 3); //To start sampling data from AN0 - processor sample sequence initiate
        while (DONE == 0)
            ;                   // waiting for conversion
        adc_result = ADC1_SSFIFO3_R; //storing the 12 bit data - since we used sequencer 3 here it is FIFO3
        ADC1_ISC_R = (1 << 3); // clear conv flag  ..interrupt status and clear register

        if (adc_result >= 0 && adc_result <= 1365)
        {
            GPIO_PORTF_DATA_R = LED_GREEN;
        }
        else if (adc_result >= 1365 && adc_result <= 2730)
        {
            GPIO_PORTF_DATA_R = LED_BLUE;
        }
        else if (adc_result >= 2730 && adc_result <= 4095)
        {
            GPIO_PORTF_DATA_R = LED_RED;
        }
    }
}
