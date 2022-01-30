/* 
 * File: Timer.c  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

#include "Timer.h"


extern  volatile unsigned int GLOBAL_StepperMotorSpeed;


//******************************************************
//Initialise timers

void    InitialiseTimers(void)
{
    //note that timer 2 and timer 4 are used in 10 bit PWM generation and cannot be used for other purposes
    //and are initialised in the PWM initialisation
    
    //Initialise timer 1 for 1 us increment used in 'Delay' function
    T1CONbits.CKPS = 0b11;      //clock pre scale 1:8
    T1CONbits.CS = 0b00;        //Clock source Fosc/4
    
    
    //Initialise timer 3 for 1 us increment used in stepper motor speed control
    T3CONbits.CKPS = 0b11;      //clock pre scale 1:8
    T3CONbits.CS = 0b00;        //Clock source Fosc/4
    
    //initialise timer 2 for 10 bit PWM operation
    //do not enable timer at this point
    T2CLKCONbits.T2CS = 0b0001;                         //Fosc/4 clock source
    T2CONbits.T2CKPS = 0b111;                           //prescaler 128        
    T2PR = 249;                                         //4 ms period
    T2HLTbits.MODE = 0b00000;                           //free running timer reset by self compare
    T2HLTbits.CKSYNC = 0b1;                             //synch the timer 
    
}





//************************************************
//Delay function that is passed a value in microseconds.
//it is not an interrupt function and is therefore not accurate

void    Delay(unsigned int DelayTime)
{
    //test foe invalid value
    if(DelayTime > 0)
    {
        TIMER_1_ENABLE = TIMER_OFF;     //ensure timer is off
        TIMER_1_TIME = -DelayTime;      //load timer with - time in us
        TIMER_1_FLAG = 0b0;
        TIMER_1_ENABLE = TIMER_ON;      //timer on
        while(TIMER_1_FLAG == 0b0);     //wait for time to elapse
        TIMER_1_ENABLE = TIMER_OFF;     //turn timer off
        TIMER_1_FLAG = 0b0;             //clear flag
    }
}



//****************************************************
//Stepper timer on 

void    StepperTimerOn(void)
{
    TIMER_3_ENABLE = TIMER_OFF;                     //ensure timer is off
    TIMER_3_TIME = -GLOBAL_StepperMotorSpeed;       //load timer with - time in us
    TIMER_3_INTERRUPT_FLAG = CLEAR_FLAG;            //clear timer flag
    TIMER_3_INTERRUPT_ENABLE = INTERRUPT_ENABLED;   //enable timer interrupts
    TIMER_3_ENABLE = TIMER_ON;                      //timer on
}


//****************************************************
//Stepper timer on 

void    StepperTimerOff(void)
{
    TIMER_3_ENABLE = TIMER_OFF;                     //ensure timer is off
    TIMER_3_INTERRUPT_ENABLE = INTERRUPT_DISABLED;  //disable timer interrupts
    TIMER_3_INTERRUPT_FLAG = CLEAR_FLAG;            //clear timer flag
}


