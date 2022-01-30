/* 
 * File: ADC.h  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  

//define analogue channel numbers
#define CH0             0b000000
#define CH26            0b011010
#define CH27            0b011011


//declare functions
void    InitialiseADC(void);
unsigned int    GetSpeedControlValue(void);
unsigned int    GetAnalogueChannel_1_Value(void);
unsigned int    GetAnalogueChannel_2_Value(void);



#endif	/* ADC_H */

