/* 
 * File: Comms.c  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

#include "Init.h"
#include "Comms.h"
#include "PWM.h"
#include "Timer.h"

//define external global variables
extern volatile unsigned int GLOBAL_CalibrationStatus;
extern volatile unsigned int GLOBAL_TimerEventCounter;
extern volatile unsigned int GLOBAL_TimerEventFlag;
extern volatile unsigned int GLOBAL_MasterTimeOutCounter;
extern volatile unsigned int GLOBAL_MasterTimeOutFlag;
extern volatile unsigned char GLOBAL_ResultString[RESULT_STRING_LENGTH];
extern  volatile unsigned int GLOBAL_PWM1_PulseTime;
extern  volatile unsigned int GLOBAL_PWM2_PulseTime;
extern  volatile unsigned int GLOBAL_PWM3_PulseTime;
extern  volatile unsigned int GLOBAL_PWM4_PulseTime;
extern  volatile unsigned int GLOBAL_StepperMotorSpeed;

//define variables used in this file
volatile unsigned char UART_RxBufferPointerType[BUFFER_SIZE];
volatile unsigned char *OldRxPointer = UART_RxBufferPointerType;
volatile unsigned char *NewRxPointer = UART_RxBufferPointerType;






void    InitialiseComms (void)
{
    //initialise UART for 9600/8/N/1
    BAUD1CONbits.BRG16 = 0;     //use 8 bit baud rate generator
    TX1STAbits.BRGH = 0;        //Low speed UART
    SPBRGH = 0;                 //set SPBRG word to 51 for 9600 baud
    SPBRGL = 51;                //32MHz oscillator
    
    //enable serial port
    TX1STAbits.SYNC = 0;         //set for asynchronous mode
    TX1STAbits.TXEN = 1;         //enable transmitter
    RC1STAbits.CREN = 1;         //enable receiver
    RC1STAbits.SPEN = 1;         //enable serial port
    PIE1bits.RCIE = 1;          //enable RX interrupts
    
    //initialise receiver arrays 
    OldRxPointer = UART_RxBufferPointerType;
    NewRxPointer = UART_RxBufferPointerType;

}



//***************************************************************
//Convert the passed integer value into a decimal string
//leading zeros may be added by specifying the length
//This is primarily used for fraction decimal places

void    DecimalToResultString(unsigned int Value, volatile unsigned char *ResultString, unsigned int Length)
{
    unsigned int Status = 0;
    
    //null the string
    *ResultString = (unsigned char)NULL;
    
    //convert from a digital value into an ASCII string
    if(Value >= 10000 || Length == 5)
    {
        *ResultString = (unsigned char)((Value / 10000) + ASCII_NUMBER_OFFSET);
        Value = Value % 10000;
        //conversion performed set status bit
        Status = 1; 
        //increment pointer
        ResultString++;
    }
    if(Status == 1 || Value >= 1000 || Length == 4)
    {
        *ResultString = (unsigned char)((Value / 1000) + ASCII_NUMBER_OFFSET);
        Value = Value % 1000;
        //conversion performed set status bit
        Status = 1;
        //increment pointer
        ResultString++;
    }
    if(Status == 1 || Value >= 100 || Length == 3)
    {
        *ResultString = (unsigned char)((Value / 100) + ASCII_NUMBER_OFFSET);
        Value = Value % 100;
        //conversion performed set status bit
        Status = 1;
        //increment pointer
        ResultString++;
    }
    if(Status == 1 || Value >= 10 || Length == 2)
    {
        *ResultString = (unsigned char)((Value / 10) + ASCII_NUMBER_OFFSET);
        Value = Value % 10;
        //conversion performed set status bit
        Status = 1;
        //increment pointer
        ResultString++;
    }
    //get units
    *ResultString = (unsigned char)(Value + ASCII_NUMBER_OFFSET);
    //increment pointer
    ResultString++;
    //null the string
    *ResultString = (unsigned char)NULL;
}




//***************************************************************
//Convert the passed string into an integer and return the result

unsigned int    StringToInteger(volatile unsigned char *StringPointer)
{
    unsigned int CharValue;
    unsigned int Result = 0;
    
    while(*StringPointer != (unsigned char)NULL)
    {
        //convert string from ASCII character to decimal 
        //note that string should only contain ASCII characters
        CharValue = (unsigned int)*StringPointer - 0x30; //cast is used to stop warning from compiler since number should never go negative
        //multiply and add
        Result = (Result * 10) + CharValue;
        //increment the pointer
        StringPointer++;
    }
    
    return Result;
}



//***************************************************************
//Convert the passed value into a binary string

void    BinaryToResultString(unsigned int ByteLength, volatile unsigned char *ResultString, unsigned int Value)
{
    unsigned int Index = 0;
    unsigned int Loop;
    unsigned int LoopLength;
    unsigned int Mask;

    //terminate string
    *ResultString = (unsigned char)NULL;

    //Add 0b header
    *ResultString = '0';
    ResultString++;
    *ResultString = 'b';
    ResultString++;

    //set mask to correct value
    if(ByteLength == 1)
    {
       //8 bit mask
        Mask = 0x0080;
        LoopLength = 8;
    }
    else
    {
        //16 bit mask
        Mask = 0x8000;
        LoopLength = 16;
    }
    //loop for all 16 bits of integer
    for(Loop = 0; Loop < LoopLength; Loop++)
    {
        //test status of highest bit
        if((Value & Mask) == 0)
        {
            //bit is a zero
            *ResultString = '0';
        }
        else
        {
            //bit is a 1
            *ResultString = '1';
        }
        //insert a space every 4 bits
        //correct for zero start point
        if((Loop + 1) %4 == 0)
        {
            //increment index
            ResultString++;
            //add a space to the string
            *ResultString = ' ';
        }
        //shift left to get next highest bit
        Value = Value << 1;
        //increment index
        ResultString++;
    }
    //terminate the string
    *ResultString = (unsigned char)NULL;
}




//***************************************************************
//get a character from the UART buffer
//if the value is 0xFFFF then no character is in the buffer

unsigned int GetChar()
{
    //declare variables
    unsigned int ReturnValue = 0xFFFF;
    
    //test for any characters in the receiver buffer
    if (OldRxPointer != NewRxPointer)
    {
        //character in the buffer, return the value
        ReturnValue = *OldRxPointer;
        //increment the Rd pointer
        OldRxPointer++;
        //test for wrap around
        if(OldRxPointer == (UART_RxBufferPointerType + BUFFER_SIZE - 1))
        {
            OldRxPointer = UART_RxBufferPointerType;
        }
    }
    
    return ReturnValue;
}





//***************************************************************
//GetString gets the entered data from the comms ending in a CR and returns an integer
//Return error codes as necessary
//Length of the string is passed along with string array pointer

unsigned int    GetString(unsigned int Length, volatile unsigned char *RxString)
{
    //declare variables
    unsigned int RxChar;
    unsigned int CharCounter = 0;
    unsigned int ReturnValue = STRING_OK;
    
    //null the receiver string
    *RxString = (unsigned char)NULL;
    
    //get the string but test for master time out
    while(GLOBAL_MasterTimeOutFlag == 0)
    {
        //check comms buffer for character
        RxChar = GetChar();
        
        if(RxChar != 0xFFFF)
        {
            //character received 
            //test for termination character
            if((unsigned char)RxChar == '\r')
            {
                break;
            }
            else
            {
                if(RxChar >= '0' && RxChar <= '9')
                {
                    //set return value to signify one or more characters
                    ReturnValue = 0;
                    //test for string less than length
                    if(CharCounter < Length)
                    {
                        //echo character
                        SendChar(RxChar);
                        //save the character in the array
                        *RxString = (unsigned char)RxChar;
                        //increment the pointer to the array
                        RxString++;
                        //null the end of the string
                        *RxString = (unsigned char)NULL;
                        //increment the character counter
                        CharCounter++;
                    }
                    else
                    {
                        //string too long
                        //echo 'X'character
                        SendChar('X');
                        //set status to 'too long'
                        ReturnValue = TOO_LONG;
                    }
                }
            }
        } 
    }
    //test for no data entered
    if(CharCounter == 0)
    {
        ReturnValue = NO_DATA;
    }
    //test for overriding timeout
    if(GLOBAL_MasterTimeOutFlag == 1)
    {
        ReturnValue = TIME_OUT;
    }
    //return error value
    return ReturnValue;
}


//***************************************************************
//GetFloatString gets the entered data from the comms ending in a CR
//Return error codes as necessary
//Length of the maximum string is passed along with the pointer to the string array

unsigned int    GetFloatString(unsigned int Length, volatile unsigned char *RxString)
{
    //declare variables
    unsigned int RxChar;
    unsigned int CharCounter = 0;
    unsigned int ReturnValue = STRING_OK;
    unsigned int DecimalPointCounter = 0;
    
    //null the receiver string
    *RxString = (unsigned char)NULL;
    
    //get the string but test for master time out
    while(GLOBAL_MasterTimeOutFlag == 0)
    {
        //check comms buffer for character
        RxChar = GetChar();
        if(RxChar != 0xFFFF)
        {
            //character received 
            //test for termination character
            if((unsigned char)RxChar == '\r')
            {
                break;
            }
            else
            {
                if((RxChar >= '0' && RxChar <= '9') || RxChar == '.')
                {
                    //test for multiple decimal places
                    if(RxChar == '.')
                    {
                        //increment decimal point counter
                        DecimalPointCounter++;
                    }
                    //set return value to signify one or more characters
                    ReturnValue = 0;
                    //test for string less than length
                    if(CharCounter < Length)
                    {
                        //echo character
                        SendChar(RxChar);
                        //save the character in the array
                        *RxString = (unsigned char)RxChar;
                        //increment the pointer to the array
                        RxString++;
                        //null the end of the string
                        *RxString = (unsigned char)NULL;
                        //increment the character counter
                        CharCounter++;
                    }
                    else
                    {
                        //string too long
                        //echo 'X'character
                        SendChar('X');
                        //set status to 'too long'
                        ReturnValue = TOO_LONG;
                    }
                }
            }
        } 
    }
    //test for no data entered
    if(CharCounter == 0)
    {
        ReturnValue = NO_DATA;
    }
    //test for too many decimal points
    if(DecimalPointCounter > 1)
    {
        ReturnValue = INVALID_STRING;
    }
    //test for overriding timeout
    if(GLOBAL_MasterTimeOutFlag == 1)
    {
        ReturnValue = TIME_OUT;
    }
    //return error value
    return ReturnValue;
}



//***************************************************************
//writes the single character to the UART

void    SendChar(unsigned int Value)
{
    //wait for TX buffer to be clear
    while(TX_BUFFER_STATUS == 0);
    //transmit character
    TXREG = (unsigned char)Value;    //note that TXREG is byte width
}



//***************************************************************
//writes the passed array string from code memory to the UART

void    SendMessage(const unsigned char *StringPointer)
{
    
    //test for end of string
    while(*StringPointer != (unsigned char)NULL)
    {
        //can a character be loaded
        if(TX_BUFFER_STATUS != 0)
        {
            TXREG = *StringPointer;
            StringPointer++;
        }
    }
}


//***************************************************************
//writes the passed array string from ram to the UART

void    SendString(volatile unsigned char *StringPointer)
{
    //test for end of string
    while(*StringPointer != (unsigned char)NULL)
    {
        //can a character be loaded
        if(TX_BUFFER_STATUS != 0)
        {
            TXREG = *StringPointer;
            StringPointer++;
        }
    }
}



//***************************************************************
//interrupt routine

void __interrupt () HIGH_ISR(void)
{
    //declare variables
    
    //test for UART receiver interrupt source
    if(UART_INTERRUPT_RX_FLAG)
    {
        //save character in buffer using pointers
        *NewRxPointer = RCREG;
        //increment pointer
        NewRxPointer++;
        //test for wrap around
        if(NewRxPointer == (UART_RxBufferPointerType + BUFFER_SIZE - 1))
        {
            //re-initialise the array pointer
            NewRxPointer = UART_RxBufferPointerType;
        }
        //clear interrupt flag
        UART_INTERRUPT_RX_FLAG = 0;
    }
    

    //test for loading 10 bit PWMs
    if(TIMER2_INTERRUPT_FLAG)
    {
        TIMER2_INTERRUPT_FLAG = CLEAR_FLAG;
        //load new PWM pulse time value
        PWM3DCH = GLOBAL_PWM1_PulseTime >> 2;               //load duty cycle high value with pulse time shifted two places
        PWM3DCL = ((GLOBAL_PWM1_PulseTime & 0x0003) << 6);  //load duty cycle low value with the value of the two shifted bits but at the MSB location
        PWM4DCH = GLOBAL_PWM2_PulseTime >> 2;               //load duty cycle high value with pulse time shifted two places
        PWM4DCL = ((GLOBAL_PWM2_PulseTime & 0x0003) << 6);  //load duty cycle low value with the value of the two shifted bits but at the MSB location
    }
    

    //test for updating stepper motor speed and stepping motor
    if(TIMER_3_INTERRUPT_FLAG)
    {
        TIMER_3_INTERRUPT_FLAG = CLEAR_FLAG;
        //load new PWM pulse time value
        TIMER_3_TIME = -GLOBAL_StepperMotorSpeed;
        //toggle DRV8711 step signal
        if(DRV8711_STEP_READ_LATCH == 0)
        {
           DRV8711_STEP_WRITE = 1; 
        }
        else
        {
           DRV8711_STEP_WRITE = 0;
        }
    }
    
    

    //test for loading PWM5 request from PWM5 controller
    if(PERIPHERAL_PWM5_INTERRUPT_FLAG)
    {
        PERIPHERAL_PWM5_INTERRUPT_FLAG = CLEAR_FLAG;
        PWM5_INTERRUPT_FLAG = 0b0;
        //load new PWM pulse time value
        PWM5DC = GLOBAL_PWM3_PulseTime;
        //load new data 
        PWM5_LOAD = 0b1;    //load all values into buffers
    }
    
    //test for loading PWM6 request from PWM6 controller
    if(PERIPHERAL_PWM6_INTERRUPT_FLAG)
    {
        PERIPHERAL_PWM6_INTERRUPT_FLAG = CLEAR_FLAG;
        PWM6_INTERRUPT_FLAG = 0b0;
        //load new PWM pulse time value
        PWM6DC = GLOBAL_PWM4_PulseTime;
        //load new data 
        PWM6_LOAD = 0b1;    //load all values into buffers
    }
}






