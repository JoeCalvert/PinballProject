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

//define strings
const unsigned char OptionMessage[] = "\r\n\r\n **** ELEVATOR CONTROL ****\r\n";
const unsigned char OptionMessage1[] = "1. Run Elevator\r\n";
const unsigned char OptionMessage2[] = "2. Elevator Calibration\r\n";
const unsigned char OptionSelectMessage[] = "\r\nEnter option number: ";
const unsigned char CRLF[] = "\r\n";


//string error messages
const unsigned char MessageTooLong[] = "\r\n String entered is too long";
const unsigned char MessageNoValue[] = "\r\n No Value Entered";
const unsigned char InvalidNumber[] = "\r\n Value out of range";
const unsigned char TooManyDecimalPoints[] = "\r\n Too many decimal points";
const unsigned char TooLarge[] = "\r\n Value too large";
const unsigned char TooSmall[] = "\r\n Value too small";

//global variables
volatile unsigned int GLOBAL_TimerEventCounter = 0;
volatile unsigned int GLOBAL_TimerEventFlag = 0;
volatile unsigned int GLOBAL_MasterTimeOutCounter = 0;
volatile unsigned int GLOBAL_MasterTimeOutFlag = 0;
volatile unsigned char GLOBAL_ResultString[RESULT_STRING_LENGTH];
volatile unsigned char GLOBAL_RxString[RX_STRING_LENGTH];
volatile unsigned int GLOBAL_StepperMotorSpeed;
volatile unsigned int GLOBAL_DirectionStatus;



//list functions
void    DisplaySystemOptionsList(void);
void    DisplayStringError(unsigned int);
void    TestDAC(unsigned int);
void    TestPWM_16Bit(unsigned int);
void    TestStepperMotor(void);
void    DisplayStepperMotorOptionsList(unsigned int);
void    SetStepperMotorSpeed(void);
void    DisplayStepperMotorStatus(void);



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
    InitialisePWM_16Bit();
    InitialiseTimers();
    InitialiseSPI();
    InitialiseDRV8711();
    
    //enable interrupts
    INTCONbits.PEIE = 1;        //enable peripheral interrupts
    INTCONbits.GIE = 1;         //enable interrupts
    
    MainMenu();
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
                case 11:    //test PWM 3
                    TestPWM_16Bit(3);
                    break;

                case 13:    //test stepper motor
                    TestStepperMotor();
                    break;
                    
                case 16:    //Get DRV8711 status and display
                    SPIAddress = DRV_STATUS_REG;
                    SPIValue = ReadSPI(SPIAddress);
                    //display binary value
                    BinaryToResultString(2, GLOBAL_ResultString, SPIValue);
                    //display the result
                    SendMessage(CRLF);
                    SendString(GLOBAL_ResultString);
                    break;

                case 17:    //Get DRV8711 status
                    SPIAddress = DRV_STATUS_REG;
                    SPIValue = 0;
                    WriteSPI(SPIAddress,SPIValue);
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


void    MainMenu(){
    while(1)
    {
        //Print options to user
        SendMessage(OptionMessage);
        SendMessage(OptionMessage1);
        SendMessage(OptionMessage2);
        SendMessage(OptionSelectMessage);

        //Check for string entry
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

            //Execute user option
            switch(Value){
                case 1:
                    RunElevator();
                    break;
                case 2:
                    CalibrationMenu();
                    break;
                default:
                    SendMessage(InvalidNumber);
            }
        }
    }
}

void    RunElevator(){

}

void    CalibrationMenu(){

}



