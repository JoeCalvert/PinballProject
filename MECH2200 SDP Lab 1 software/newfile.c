/*
 * File:   main.c
 * Author: Roger Berry
 *
 * Created on 27 December 2019, 16:12
 */


#include <xc.h>
#include "Init.h"   //this file includes all of the port definitions
#include "Comms.h"  //this file includes comms functions
#include "ADC.h"    //this file includes ADC functions
#include "DAC.h"    //this file includes DAC functions
#include "PWM.h"    //this file includes PWM functions
#include "SPI.h"    //this file includes SPI and DREV8711 functions
#include "Timer.h"  //this file includes timer functions

/*
 * The default state for the CONFIG registers is as follows:
 * CONFI1 register:
 *  FCMEN = ON
 *  IESO = ON
 *  CLKOUTEN = ON (this is inverted logic and is therefore disabled
 *  BOREN = ON (both bits set and is enabled
 *  CP = OFF 
 *  MCLR = ON 
 *  PWRTE = OFF
 *  WDTE = ON
 *  FOSC = ECH
 * 
 * CONFIG2 register
 *  LVP = ON
 *  DEBUG = OFF
 *  LPBOR = OFF
 *  BORV = LO
 *  STVREN = ON
 *  PLLEN = ON
 *  ZCD = OFF
 *  PPS1WAY = ON
 *  WRT = OFF
 * 
 * 
 * On the basis that these are the default settings then we need to consider how we are using the device.
 * The following pragma statements configure the device as required
 *  
 * 
*/

//CONFIG1
#pragma config FCMEN 	= OFF 		// Fail safe clock disabled
#pragma config IESO 	= OFF  		// Internal/external switch over disable
#pragma config MCLRE    = OFF       // MCLR pin is a digital input
#pragma config WDTE     = OFF       // watch dog off
#pragma config FOSC	 	= HS    	// crystal oscillator

//CONFIG2
#pragma config LVP      = OFF       // low voltage programming disabled


//define constants

#define     STEPPER_MOTOR       1
#define     DC_MOTOR            2
#define     LED_STRING_LENGTH   10

//define strings
const unsigned char OptionMessage[] = "\r\n\r\n **** g8 lift ****\r\n";
const unsigned char OptionMessage1[] = " 1:Test option n";
const unsigned char OptionMessage2[] = " 2:Run lift \n";
const unsigned char OptionSelectMessage[] = "\r\nEnter number: ";
const unsigned char CRLF[] = "\r\n";

//stepper/DC motor test options list
const unsigned char StepperOptionMessage[] = "\r\n\r\n **** STEPPER MOTOR TEST OPION LIST ****\r\n";
const unsigned char MotorOptionMessage1[] = " 1: Toggle direction\r\n";
const unsigned char MotorOptionMessage2[] = " 2: Set the motor step interval\r\n";
const unsigned char MotorOptionMessage2a[] = " 2: Set the PWM duty cycle\r\n";
const unsigned char MotorOptionMessage3[] = " 3: Start motor\r\n";
const unsigned char MotorOptionMessage4[] = " 4: Return to main menu\r\n";
const unsigned char MotorRunningMessage[] = "\r\n Motor running. Enter any character to stop: ";

//stepper/DC Motor drive status messages
const unsigned char StepperMotorStatusMessage[] = "\r\n\r\n*** STEPPER MOTOR DRIVE STATUS ***\r\n";
const unsigned char StepperMotorStatusMessage1[] = "\r\n      Direction: ";
const unsigned char StepperMotorStatusMessage2[] = "\r\n  Step interval: ";
const unsigned char Clockwise[] = "CLOCKWISE";
const unsigned char AntiClockwise[] = "ANTICLOCKWISE";

//lift messages


//string error messages
const unsigned char MessageTooLong[] = "\r\n String entered is too long";
const unsigned char MessageNoValue[] = "\r\n No Value Entered";
const unsigned char InvalidNumber[] = "\r\n Value out of range";
const unsigned char TooManyDecimalPoints[] = "\r\n Too many decimal points";
const unsigned char TooLarge[] = "\r\n Value too large";
const unsigned char TooSmall[] = "\r\n Value too small";

//PWM test message
const unsigned char PWM_16Bit_TestMessage[] = "\r\n Enter a value between 1000 and 2000. Enter 0 to exit: ";
const unsigned char PWM_10Bit_TestMessage[] = "\r\n Enter a percentage value between 1 and 99. Enter 0 to exit: ";

//Stepper motor speed message
const unsigned char StepperMotorSpeedMessage[] = "\r\n Enter a step interval in microseconds. Value between 500 and 9999: ";

//global variables
volatile unsigned int GLOBAL_TimerEventCounter = 0;
volatile unsigned int GLOBAL_TimerEventFlag = 0;
volatile unsigned int GLOBAL_MasterTimeOutCounter = 0;
volatile unsigned int GLOBAL_MasterTimeOutFlag = 0;
volatile unsigned char GLOBAL_ResultString[RESULT_STRING_LENGTH];
volatile unsigned char GLOBAL_RxString[RX_STRING_LENGTH];
volatile unsigned int GLOBAL_PWM3_PulseTime;
volatile unsigned int GLOBAL_PWM4_PulseTime;
volatile unsigned int GLOBAL_StepperMotorSpeed;
volatile unsigned int GLOBAL_DirectionStatus;
//volatile unsigned int GLOBAL_CheckFloor;



//list functions
void    DisplaySystemOptionsList(void);
void    DisplayStringError(unsigned int);
void    DisplaySpeedControl(void);
void    DisplayAnalogueInput_1(void);
void    DisplayAnalogueInput_2(void);
void    TestPWM_10Bit(unsigned int);
void    TestPWM_16Bit(unsigned int);
void    TestStepperMotor(void);
void    DisplayStepperMotorOptionsList(unsigned int);
void    SetStepperMotorSpeed(void);
void    DisplayStepperMotorStatus(void);
void    RunLift(void);

//main function

void main(void) {
    
    unsigned int StringStatus;
    unsigned int Value;
    unsigned int SPIAddress;
    unsigned int SPIValue;

    //wait for PLL to stabilise
    while(OSCSTATbits.PLLR == 0);
    
    InitialisePorts(); 
    InitialiseComms();
    InitialiseADC();
    InitialiseDAC();
    InitialisePWM_10Bit();
    InitialisePWM_16Bit();
    InitialiseTimers();
    InitialiseSPI();
    InitialiseDRV8711();
    
    //enable interrupts
    INTCONbits.PEIE = 1;        //enable peripheral interrupts
    INTCONbits.GIE = 1;         //enable interrupts
    
    
    //main loop
    while(1)
    {
        //display options list
        DisplaySystemOptionsList();
        
        //test for any string entry
        StringStatus = GetString(2,GLOBAL_RxString);
        if(StringStatus != STRING_OK)
        {
            //string error
            DisplayStringError(StringStatus);
        }
        else
        {
            //string ok
            //convert string to binary value
            Value = StringToInteger(GLOBAL_RxString);
            //test for required action
            switch(Value)
            {
                case 1:     //test relay 1
                    //run lift
                    break;
                    
                case 2:    //test PWM 3
                    TestPWM_16Bit(3);
                    break;
                    
                case 3:    //test PWM 4
                    TestPWM_16Bit(4);
                    break;

                case 4:    //test stepper motor
                    TestStepperMotor();
                    break;
                   
                    
                default:    //invalid entry
                    SendMessage(InvalidNumber);
                    
            }
        }
    }
    //end of main loop. Should never get to this point
    return;
}

//*********************************************
//Display speed control analogue input value

void    DisplaySpeedControl(void)
{
    unsigned int Value;
    
    SendMessage(CRLF);
    //get ADC value
    Value = GetSpeedControlValue();
    //Convert value into string. Maximum length 4 digits
    DecimalToResultString(Value,GLOBAL_ResultString,4);
    //transmit string
    SendString(GLOBAL_ResultString);
}



//*********************************************
//Display analogue channel 1 input value

void    DisplayAnalogueInput_1(void)
{
    unsigned int Value;
    
    SendMessage(CRLF);
    //get ADC value
    Value = GetAnalogueChannel_1_Value();
    //Convert value into string. Maximum length 4 digits
    DecimalToResultString(Value,GLOBAL_ResultString,4);
    //transmit string
    SendString(GLOBAL_ResultString);
}



//*********************************************
//Display analogue channel 2 input value

void    DisplayAnalogueInput_2(void)
{
    unsigned int Value;
    
    SendMessage(CRLF);
    //get ADC value
    Value = GetAnalogueChannel_2_Value();
    //Convert value into string. Maximum length 4 digits
    DecimalToResultString(Value,GLOBAL_ResultString,4);
    //transmit string
    SendString(GLOBAL_ResultString);
}



//*********************************************
//test PWM 10 bit. Pass PWM number
//asked to enter duty cycle as a percentage
//0% to exit

void    TestPWM_10Bit(unsigned int PWM_Number)
{
    unsigned int Status = 0;
    unsigned int StringStatus;
    unsigned int Value;
    
    //enable selected PWM
    switch(PWM_Number)
    {
        case 1: //PWM 1 selected
            EnablePWM_1();
            Enable_10BitPWM_Timer();
            break;
            
        case 2: //PWM 2 selected
            EnablePWM_2();
            Enable_10BitPWM_Timer();
            break;
            
        //no default    
    }
    
    //loop until exit character received
    while(Status == 0)
    {
        //send the command string
        SendMessage(PWM_10Bit_TestMessage);
        StringStatus = GetString(2,GLOBAL_RxString);
        if(StringStatus != STRING_OK)
        {
            //string error
            DisplayStringError(StringStatus);
        }
        else
        {
            //convert string to binary
            Value = StringToInteger(GLOBAL_RxString);
            //test for string value
            if(Value == 0)
            {
                //exit PWM test
                Status = 1;
            }
            else
            {
                //test for which PWM to change
                switch(PWM_Number)
                {
                    case 1: //load PWM 1 time value
                        GLOBAL_PWM1_PulseTime = Value * 10;
                        break;

                    case 2: //load PWM 2 time value
                        GLOBAL_PWM2_PulseTime = Value * 10;
                        break;

                    //No default
                }

            }
        }
    }
    //turn 10 bit PWM timer off
    Disable_10BitPWM_Timer();
    //disable 10 bit PWMs
    DisablePWM_1();
    DisablePWM_2();
    //reset PWM values to 10%
    GLOBAL_PWM1_PulseTime = 100;
    GLOBAL_PWM2_PulseTime = 100;
}    



//*********************************************
//test PWM 16 bit. Enter a number between 1000 and 2000 to change the duty cycle
//Time in microseconds. Enter 0 to exit

void    TestPWM_16Bit(unsigned int PWM_Number)
{
    unsigned int Status = 0;
    unsigned int StringStatus;
    unsigned int Value;
    
    //loop until exit character received
    while(Status == 0)
    {
        //send the command string
        SendMessage(PWM_16Bit_TestMessage);
        StringStatus = GetString(4,GLOBAL_RxString);
        if(StringStatus != STRING_OK)
        {
            //string error
            DisplayStringError(StringStatus);
        }
        else
        {
            //convert string to binary
            Value = StringToInteger(GLOBAL_RxString);
            //test for string value
            if(Value == 0)
            {
                //exit PWM test
                Status = 1;
            }
            else if(Value < 1000)   
            {
                //string value is too small
                //string error
                DisplayStringError(VALUE_TOO_SMALL);
            }
            else if(Value > 2000)
            {
                //string value is too big
                //string error
                DisplayStringError(VALUE_TOO_LARGE);
            }
            else
            {
                //test for which PWM to change
                switch(PWM_Number)
                {
                    case 3: //load PWM 5 time value
                        GLOBAL_PWM3_PulseTime = Value;
                        break;

                    case 4: //load PWM 6 time value
                        GLOBAL_PWM4_PulseTime = Value;
                        break;

                    //No default
                }

            }
        }
    }
    //reset PWM values to 1500
    GLOBAL_PWM3_PulseTime = 1500;
    GLOBAL_PWM4_PulseTime = 1500;
}



//*********************************************
//test stepper motor.
//this has a secondary HMI for selecting direction, speed and motor on/off

void    TestStepperMotor(void)
{
    unsigned int Status = 0;
    unsigned int StringStatus;
    unsigned int Value;
    unsigned int MotorStopStatus;
    
    //set for stepper mode
    SetDRV8711_Mode(STEPPER_MODE);
    
    //set direction to clockwise
    GLOBAL_DirectionStatus = 0;

    //run motor control options menu
    while(Status == 0)
    {
        //display status
        DisplayStepperMotorStatus();
        //display options list
        DisplayStepperMotorOptionsList(STEPPER_MOTOR);

        //test for any string entry
        StringStatus = GetString(1,GLOBAL_RxString);
        if(StringStatus != STRING_OK)
        {
            //string error
            DisplayStringError(StringStatus);
        }
        else
        {
            //string ok
            //convert string to binary value
            Value = StringToInteger(GLOBAL_RxString);
            //test for required action
            switch(Value)
            {
                case 1:     //toggle motor direction
                    if(GLOBAL_DirectionStatus == 0)
                    {
                        //change from clockwise to anticlockwise
                        DRV8711_DIR_WRITE = 0b1;
                        GLOBAL_DirectionStatus = 1;
                    }
                    else
                    {
                        //change from anticlockwise to clockwise
                        DRV8711_DIR_WRITE = 0b0;
                        GLOBAL_DirectionStatus = 0;
                    }
                    break;

                case 2:     //set motor speed
                    SetStepperMotorSpeed();
                    break;

                case 3:     //switch motor on until character received
                    SendMessage(MotorRunningMessage);
                    //stepper is off therefore turn it on
                    MotorOn();
                    //enable the stepper interrupt timer
                    StepperTimerOn();
                    //wait for character to exit
                    MotorStopStatus = 0;
                    while(MotorStopStatus == 0)
                    {
                        Value = GetChar();
                        if(Value != 0xFFFF)
                        {
                            MotorStopStatus = 1;
                        }
                    }
                    //disable the stepper interrupt timer
                    StepperTimerOff();
                    //set the step output to 0
                    DRV8711_STEP_WRITE = 0b0;
                    //switch motor drive off
                    MotorOff();
                    break;

                case 4:     //return to main screen
                    Status = 1;
                    break;

                default:    //invalid entry
                    SendMessage(InvalidNumber);
            }
        }
    }
    //ensure the stepper interrupt timer is off
    StepperTimerOff();
    //set the step output to 0
    DRV8711_STEP_WRITE = 0b0;
    //ensure that motor drive is off
    MotorOff();
    //set stepper direction output to 0 and clockwise
    DRV8711_DIR_WRITE = 0; 
}



//***********************************************
//Set stepper motor speed. load a value between 500 and 9999

void    SetStepperMotorSpeed(void)
{
    unsigned int    Value;
    unsigned int    StringStatus; 
    
    //send the command string
    SendMessage(StepperMotorSpeedMessage);
    //get the string, maximum 4 characters
    StringStatus = GetString(4,GLOBAL_RxString);
    if(StringStatus != STRING_OK)
    {
        //string error
        DisplayStringError(StringStatus);
    }
    else
    {
        //convert string to binary
        Value = StringToInteger(GLOBAL_RxString);
        //test for value too small
        if(Value < 500)
        {
            DisplayStringError(VALUE_TOO_SMALL);
        }
        else
        {
            //load the speed value into the speed control register
            GLOBAL_StepperMotorSpeed = Value;
        }
    }
}    
    


//*********************************************
//***************************************************
//option list
void    DisplaySystemOptionsList(void)
{
    //list the user options
    SendMessage(OptionMessage);
    SendMessage(OptionMessage1);
    SendMessage(OptionMessage2);
    SendMessage(OptionSelectMessage);
}



//*********************************************
//display Stepper motor drive options list

void    DisplayStepperMotorOptionsList(unsigned int MotorType)
{
     SendMessage(StepperOptionMessage);
     //no default
    SendMessage(MotorOptionMessage1);
    SendMessage(MotorOptionMessage2);
    SendMessage(MotorOptionMessage3);
    SendMessage(MotorOptionMessage4);
    SendMessage(OptionSelectMessage);
}


//*********************************************
//display stepper motor status

void    DisplayStepperMotorStatus(void)
{
    //send status header message
    SendMessage(StepperMotorStatusMessage);
    
    //send motor direction information
    SendMessage(StepperMotorStatusMessage1);
    if(GLOBAL_DirectionStatus == 0)
    {
        SendMessage(Clockwise);
    }
    else
    {
        SendMessage(AntiClockwise);
    }

    //send motor speed information
    SendMessage(StepperMotorStatusMessage2);
    //convert the integer value into a string
    DecimalToResultString(GLOBAL_StepperMotorSpeed, GLOBAL_ResultString, 4);
    //display the result
    SendString(GLOBAL_ResultString);
}

//*********************************************
//display string error

void    DisplayStringError(unsigned int ErrorValue)
{
    switch(ErrorValue)
    {
        case TOO_LONG:  //string is too long
            SendMessage(MessageTooLong);
            break;
            
        case NO_DATA:  //string has no data
            SendMessage(MessageNoValue);
            break;
            
        case INVALID_STRING:  //string has too many decimal points
            SendMessage(TooManyDecimalPoints);
            break;
            
        case VALUE_TOO_LARGE:  //string exceeds maximum value
            SendMessage(TooLarge);
            break;
            
        case VALUE_TOO_SMALL:  //string exceeds minimum value
            SendMessage(TooSmall);
            break;
            
        //No default
    }
}
void RunLift(void){
    
    unsigned int StringStatus;
    unsigned int Value;
    
    while(1)
    {
        //display options list
        DisplaySystemOptionsList();
        
        //test for any string entry
        StringStatus = GetString(2,GLOBAL_RxString);
        if(StringStatus != STRING_OK)
        {
            //string error
            DisplayStringError(StringStatus);
        }
        else
        {
            //string ok
            //convert string to binary value
            Value = StringToInteger(GLOBAL_RxString);
            //test for required action
            switch(Value)
            {
                case 1:     //test relay 1
                    //floor 0
                    break;
                    
                case 2:     //test relay 2
                    //floor 1
                    break;
                    
                case 3:     //test relay 3
                    //floor 2
                    break;
                default:    //invalid entry
                    SendMessage(InvalidNumber);      
            }
        }
    }
    //end of main loop. Should never get to this point
    return;
}

void MoveMM(double mm){
    //amount in mm 
}

