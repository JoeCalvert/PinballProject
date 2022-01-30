/* 
 * File: PWM.c  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

#include "PWM.h"
#include "Init.h"


//declare global variables
extern  volatile unsigned int GLOBAL_PWM1_PulseTime;
extern  volatile unsigned int GLOBAL_PWM2_PulseTime;
extern  volatile unsigned int GLOBAL_PWM3_PulseTime;
extern  volatile unsigned int GLOBAL_PWM4_PulseTime;


//initialise 16 bit PWM modules

void    InitialisePWM_16Bit(void)
{
    //initialise PWM 5 (which is designated as PWM 3 on the target)
    //disable port output during initialisation
    PWM_3_DIRECTION = 0b1;      //set PWM5 tris to input
    
    //set up PWM 5 
    PWM5CON = 0x00;             //clear PWM5 con register
    PWM5OFCONbits.OFM = 0b00;   //independent run mode
    PWM5CLKCONbits.CS = 0b00;   //clock source Fosc
    PWM5CLKCONbits.PS = 0b101;  //1:32 prescaler
    PWM5OF = 0x0000;            //no offset applied
    PWM5PH = 0x0000;            //no phase offset applied
    PWM5PR = 19999;             //load period register with 20ms
    PWM5TMR = 0;                //clear the primary timer

    //load the duty cycle register for PWM3 with pulse width of 1.5ms
    //1000 = 1ms
    //2000 = 2ms
    //1500 = 1.5ms
    GLOBAL_PWM3_PulseTime = 1500;
    //load PWM5 pulse time value
    PWM5DC = GLOBAL_PWM3_PulseTime;
    
    //clear the interrupt flag and enable interrupts
    PWM5_INTERRUPT_FLAG = 0b0;
    PWM5_INTERRUPT_ENABLE = 0b1;
    PERIPHERAL_PWM5_INTERRUPT_FLAG = 0b0;
    PERIPHERAL_PWM5_INTERRUPT_ENABLE = 0b1;
    
    PWM5LDCONbits.LDA = 0b1;    //load all values into buffers
     
    PWM5CONbits.POL = 0b0;      //active low output
    PWM5CONbits.EN = 0b1;       //enable PWM module
    PWM_3_DIRECTION = 0b0;      //set PWM5 tris to output

    
    //initialise PWM 6 (which is designated as PWM 4 on the target)
    //disable port output during initialisation
    PWM_4_DIRECTION = 0b1;      //set PWM6 tris to input
    
    //set up PWM 5 
    PWM6CON = 0x00;             //clear PWM6 con register
    PWM6OFCONbits.OFM = 0b00;   //independent run mode
    PWM6CLKCONbits.CS = 0b00;   //clock source Fosc
    PWM6CLKCONbits.PS = 0b101;  //1:32 prescaler
    PWM6OF = 0x0000;            //no offset applied
    PWM6PH = 0x0000;            //no phase offset applied
    PWM6PR = 19999;             //load period register with 20ms
    PWM6TMR = 0;                //clear the primary timer

    //load the duty cycle register for PWM4 with pulse width of 1.5ms
    //1000 = 1ms
    //2000 = 2ms
    //1500 = 1.5ms
    GLOBAL_PWM4_PulseTime = 1500;
    //load PWM6 pulse time value
    PWM6DC = GLOBAL_PWM4_PulseTime;
    
    //clear the interrupt flag
    PWM6_INTERRUPT_FLAG = 0b0;
    PWM6_INTERRUPT_ENABLE = 0b1;
    PERIPHERAL_PWM6_INTERRUPT_FLAG = 0b0;
    PERIPHERAL_PWM6_INTERRUPT_ENABLE = 0b1;
    
    PWM6LDCONbits.LDA = 0b1;    //load all values into buffers
     
    PWM6CONbits.POL = 0b0;      //active low output
    PWM6CONbits.EN = 0b1;       //enable PWM module
    PWM_4_DIRECTION = 0b0;      //set PWM6 tris to output
}


//**********************************************
//initialise 10 bit PWM modules

void    InitialisePWM_10Bit(void)
{
    //initialise PWM 3 (which is designated as PWM 1 on the target)
    //disable port output during initialisation
    PWM_1_DIRECTION = INPUT;                            //set PWM1 tris to input
    //set up PWM 3, ensure PWM3 is off
    PWM3CON = 0x00;                                     //clear PWM3 con register
    //load duty cycle to 10%
    GLOBAL_PWM1_PulseTime = 100;                        //duty cycle percentage x 10 = 10%
    PWM3DCH = GLOBAL_PWM1_PulseTime >> 2;               //load duty cycle high value with pulse time shifted two places
    PWM3DCL = ((GLOBAL_PWM1_PulseTime & 0x0003) << 6);  //load duty cycle low value with the value of the two shifted bits but at the MSB location
    //set the PWM 3 pin to output
    PWM_1_DIRECTION = OUTPUT;                           //set PWM1 tris to output


    
    //initialise PWM 4 (which is designated as PWM 2 on the target)
    //disable port output during initialisation
    PWM_2_DIRECTION = INPUT;                            //set PWM2 tris to input
    //set up PWM 4 
    PWM4CON = 0x00;                                     //clear PWM4 con register
    //load duty cycle to 10%
    GLOBAL_PWM2_PulseTime = 100;                        //duty cycle percentage x 10 = 10%
    PWM4DCH = GLOBAL_PWM2_PulseTime >> 2;               //load duty cycle high value with pulse time shifted two places
    PWM4DCL = ((GLOBAL_PWM2_PulseTime & 0x0003) << 6);  //load duty cycle low value with the value of the two shifted bits but at the MSB location
    //set the PWM 4 pin to output
    PWM_2_DIRECTION = OUTPUT;                           //set PWM2 tris to output
}



//****************************************************
//Enable PWM 1

void    EnablePWM_1(void)
{
    PWM3CONbits.EN = 0b1;       //enable PWM3   
}


//****************************************************
//Disable PWM 1

void    DisablePWM_1(void)
{
    PWM3CONbits.EN = 0b0;       //disable PWM3   
}



//****************************************************
//Enable PWM 2

void    EnablePWM_2(void)
{
    PWM4CONbits.EN = 0b1;       //enable PWM4   
}


//****************************************************
//Disable PWM 2

void    DisablePWM_2(void)
{
    PWM4CONbits.EN = 0b0;       //disable PWM4   
}


//****************************************************
//Enable 10 bit PWM timer

void    Enable_10BitPWM_Timer(void)
{
    TMR2 = 0;                           //clear timer counter
    TIMER2_INTERRUPT_FLAG = 0b0;        //clear timer 2 interrupt flag
    TIMER2_INTERRUPT_ENABLE = 0b1;      //enable timer 2 interrupt
    T2CONbits.T2ON = 0b1;                //enable timer
}


//****************************************************
//Disable 10 bit PWM timer

void    Disable_10BitPWM_Timer(void)
{
    T2CONbits.T2ON = 0b0;               //disable timer 2
    TIMER2_INTERRUPT_FLAG = 0b0;        //clear timer 2 interrupt flag
    TIMER2_INTERRUPT_ENABLE = 0b0;      //disable timer 2 interrupt
}





