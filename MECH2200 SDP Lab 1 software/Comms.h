/* 
 * File: Comms.h  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef COMMS_H
#define	COMMS_H

#include <xc.h> // include processor files - each processor file is guarded.  


//define registers
#define UART_FRAME_ERROR            RCSTAbits.FERR
#define UART_OVERRUN_ERROR          RCSTAbits.OERR
#define UART_INTERRUPT_RX_FLAG      PIR1bits.RCIF
#define TX_BUFFER_STATUS            TXSTAbits.TRMT

//define comms constants
#define BUFFER_SIZE                 20      //cyclic buffer is 20 bytes long
#define RX_STRING_LENGTH            30      //maximum rx string length
#define RESULT_STRING_LENGTH        30      //maximum value convertion string length
#define ASCII_NUMBER_OFFSET         0x0030
#define ASCII_LETTER_OFFSET         0x0041
#define NO_DATA                     0xFFFF  //error message number for no data entry
#define TOO_LONG                    0xFFFE  //error message number for data entry too long 
#define TIME_OUT                    0xFFFD  //time out occurred
#define INVALID_STRING              0xFFFC  //invalid string too many decimal points
#define VALUE_TOO_LARGE             0xFFFB  //string value too large
#define VALUE_TOO_SMALL             0xFFFA  //below minimum value
#define STRING_OK                   0x0000  //string ok
#define NULL                        0       //null string terminator

//declare functions
void    InitialiseComms (void);
void    DecimalToResultString(unsigned int, volatile unsigned char *, unsigned int);
void    BinaryToResultString(unsigned int, volatile unsigned char *, unsigned int);
unsigned int    StringToInteger(volatile unsigned char *);
unsigned int    GetChar(void);
unsigned int    GetString(unsigned int, volatile unsigned char *);
unsigned int    GetFloatString(unsigned int, volatile unsigned char *);
void    SendChar(unsigned int);
void    SendMessage(const unsigned char *);
void    SendString(volatile unsigned char *);
void __interrupt () HIGH_ISR(void);



#endif	/* COMMS_H */

