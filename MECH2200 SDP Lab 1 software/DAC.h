/* 
 * File: DAC.h  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DAC_H
#define	DAC_H

#include <xc.h> // include processor files - each processor file is guarded.  

//define analogue channel numbers



//declare functions
void    InitialiseDAC(void);
void    LoadDAC_1(unsigned int);
void    LoadDAC_2(unsigned int);


#endif	/* DAC_H */


