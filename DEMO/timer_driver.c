/*
 * timer.c
 *
 *  Created on: 07-Feb-2024
 *      Author: TECHHE
 */

#include "tm4c123xx.h"

#define UP_COUNTER 1<<4
#define DOWN_COUNTER 0<<4
#define R_16_32_BIT 0x4
#define ENABLE_TIMER (1<<0)
#define PERIODIC 0x2
#define ONE_SHOT 0x1

void timer_initialize(void)
{
    //  Disable Timer 1A before configuring
    TIMER1_CTL_R &= (~(1<<0));

    //  Configuration register
    TIMER1_CFG_R = 0x4;

    //  Select Mode of the register
    TIMER1_TAMR_R = PERIODIC; //  Periodic Mode
    TIMER1_TAMR_R |= UP_COUNTER; //  Down Counter

    //  Set Interval Load Register Value
    TIMER1_TAILR_R = 16000000;//16,000,000

    //  Enable the timer
    TIMER1_CTL_R |= ENABLE_TIMER;
}




