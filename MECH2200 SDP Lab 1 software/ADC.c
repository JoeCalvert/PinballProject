/* 
 * File: ADC.c  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

#include "ADC.h"


//***************************************************
//initialise ADCs

void    InitialiseADC(void)
{
    //initialise ADC
    ADCON1bits.ADFM = 0b1;          //right justified
    ADCON1bits.ADCS = 0b110;        //Fosc/64 clock source
    ADCON1bits.ADNREF = 0b0;        //negative reference 0V
    ADCON1bits.ADPREF = 0b00;       //positive reference from VDD input
    
    //no trigger source
    ADCON2bits.TRIGSEL = 0x00;
    
    //enable ADC
    ADCON0bits.ADON = 0b1;          
}


//***************************************
//get speed potentiometer value. Range 0 to 5 V, result 0 to 1023

unsigned int    GetSpeedControlValue(void)
{
    unsigned int    Result;
    
    //select channel
    ADCON0bits.CHS = CH0;
    //start conversion
    ADCON0bits.GO = 0b1;
    //wait for conversion to complete
    while(ADCON0bits.GO == 1);
    //get result
    Result = ADRES;
    //return result
    return Result;
}


//***************************************
//get channel 1 input value. Range 0 to 5 V, result 0 to 1023

unsigned int    GetAnalogueChannel_1_Value(void)
{
    unsigned int    Result;
    
    //select channel
    ADCON0bits.CHS = CH26;
    //start conversion
    ADCON0bits.GO = 0b1;
    //wait for conversion to complete
    while(ADCON0bits.GO == 1);
    //get result
    Result = ADRES;
    //return result
    return Result;
}



//***************************************
//get channel 2 input value. Range 0 to 5 V, result 0 to 1023

unsigned int    GetAnalogueChannel_2_Value(void)
{
    unsigned int    Result;
    
    //select channel
    ADCON0bits.CHS = CH27;
    //start conversion
    ADCON0bits.GO = 0b1;
    //wait for conversion to complete
    while(ADCON0bits.GO == 1);
    //get result
    Result = ADRES;
    //return result
    return Result;
}

