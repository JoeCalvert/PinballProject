/* 
 * File: PWM.h  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PWM_H
#define	PWM_H

#define PERIPHERAL_PWM5_INTERRUPT_ENABLE        PIE6bits.PWM5IE
#define PERIPHERAL_PWM5_INTERRUPT_FLAG          PIR6bits.PWM5IF
#define PWM5_INTERRUPT_ENABLE                   PWM5INTEbits.PRIE
#define PWM5_INTERRUPT_FLAG                     PWM5INTFbits.PRIF
#define PWM5_LOAD                               PWM5LDCONbits.LDA

#define PERIPHERAL_PWM6_INTERRUPT_ENABLE        PIE6bits.PWM6IE
#define PERIPHERAL_PWM6_INTERRUPT_FLAG          PIR6bits.PWM6IF
#define PWM6_INTERRUPT_ENABLE                   PWM6INTEbits.PRIE
#define PWM6_INTERRUPT_FLAG                     PWM6INTFbits.PRIF
#define PWM6_LOAD                               PWM6LDCONbits.LDA

#define TIMER2_INTERRUPT_FLAG                   PIR1bits.TMR2IF
#define TIMER2_INTERRUPT_ENABLE                 PIE1bits.TMR2IE
#define TIMER4_INTERRUPT_FLAG                   PIR4bits.TMR4IF
#define TIMER4_INTERRUPT_ENABLE                 PIE4bits.TMR4IE


void    InitialisePWM_16Bit(void);
void    InitialisePWM_10Bit(void);
void    EnablePWM_1(void);
void    DisablePWM_1(void);
void    EnablePWM_2(void);
void    DisablePWM_2(void);
void    Enable_10BitPWM_Timer(void);
void    Disable_10BitPWM_Timer(void);



#endif	/* PWM_H */

