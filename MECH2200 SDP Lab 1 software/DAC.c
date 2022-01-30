/* 
 * File: DAC.c  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

#include "DAC.h"


//initialise DACs

void    InitialiseDAC(void)
{
    //initialise DAC1
    //to buffer the DAC output the internal op amps are used 
    //therefore no pin allocation is made to ensure that the DAC output goes to
    //the OP amp input
    DAC1CON0bits.DACFM = 0b0;       //right justified
    DAC1CON0bits.DACPSS = 0b00;     //positive reference voltage from VDD
    DAC1CON0bits.DACNSS = 0b00;     //negative reference voltage from VSS
    
    //load DAC with 0V value 
    DAC1REF = 0;
    DACLDbits.DAC1LD = 0b1;         //load the DAC
    
    //enable internal op amp
    OPA1PCHSbits.PCH = 0b0010;      //allocate positive input of op amp to DAC1_out
    OPA1CONbits.UG = 0b1;           //non inverting buffer  
    OPA1CONbits.ORM = 0b00;         //disable output override
    OPA1CONbits.EN = 0b1;           //enable op amp        
    
    //enable DAC
    DAC1CON0bits.DACEN = 0b1;       //enable DAC
    
    
    //initialise DAC2
    DAC2CON0bits.DACFM = 0b0;       //right justified
    DAC2CON0bits.DACPSS = 0b00;     //positive reference voltage from VDD
    DAC2CON0bits.DACNSS = 0b00;     //negative reference voltage from VSS
    
    //load DAC with 0 V Value 
    DAC2REF = 0;
    DACLDbits.DAC2LD = 0b1;         //load the DAC
    
    //enable internal op amp
    OPA2PCHSbits.PCH = 0b0011;      //allocate positive input of op amp to DAC2_out
    OPA2CONbits.UG = 0b1;           //non inverting buffer  
    OPA2CONbits.ORM = 0b00;         //disable output override
    OPA2CONbits.EN = 0b1;           //enable op amp        

    //enable DAC
    DAC2CON0bits.DACEN = 0b1;       //enable DAC
}



//*************************************************
//load DAC 1 with value passed to function

void    LoadDAC_1(unsigned int Value)
{
    //load DAC with the passed value 
    DAC1REF = Value;
    DACLDbits.DAC1LD = 0b1;         //load the DAC
}



//*************************************************
//load DAC 2 with value passed to function

void    LoadDAC_2(unsigned int Value)
{
    //load DAC with the passed value 
    DAC2REF = Value;
    DACLDbits.DAC2LD = 0b1;         //load the DAC
}

