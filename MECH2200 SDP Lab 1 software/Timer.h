/* 
 * File: Timer.h  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER_H
#define	TIMER_H

//define registers

#define TIMER_1_ENABLE              T1CONbits.T1ON
#define TIMER_1_TIME                TMR1
#define TIMER_1_FLAG                PIR1bits.TMR1IF

#define TIMER_3_ENABLE              T3CONbits.T3ON
#define TIMER_3_TIME                TMR3
#define TIMER_3_INTERRUPT_FLAG      PIR4bits.TMR3IF
#define TIMER_3_INTERRUPT_ENABLE    PIE4bits.TMR3IE

//define constants

#define TIMER_OFF           0b0
#define TIMER_ON            0b1
#define INTERRUPT_ENABLED   0b1
#define INTERRUPT_DISABLED  0b0
#define CLEAR_FLAG          0b0

#include <xc.h> // include processor files - each processor file is guarded.  

//function declarations

void    InitialiseTimers(void);
void    Delay(unsigned int);
void    StepperTimerOn(void);
void    StepperTimerOff(void);


#endif	/* TIMER_H */

