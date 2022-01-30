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

volatile unsigned int GLOBAL_Floor1Position = 0;
volatile unsigned int GLOBAL_Floor2Position = 1;
volatile unsigned int GLOBAL_Floor3Position = 2;

volatile unsigned int GLOBAL_MaxPosition = 100;

//***************************************************************
//interrupt routine
void __interrupt () HIGH_ISR(void)
{
    if(PIR1bits.RCIF)
    {
    reach_bottom = 1;
    PIR1bits.RCIF = 0;
    }
    if(PIR2bits.RCIF)
    {
        reach_top = 1;
        PIR2bits.RCIF = 0;
    }
}



//list functions
void DisplayStringError(unsigned int ErrorValue);
void MainMenu();
void RunElevator();
void CalibrationMenu();



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
    unsigned int StringStatus;
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
    unsigned int StringStatus;
    unsigned int Value;
    //While
        //Move to bottom "floor"
        //Open Door
        //Ask User to enter a floor number
        //Check if already at floor
        //Close door
        //Move to floor
        //Open Door
}

void    CalibrationMenu(){
    unsigned int StringStatus;
    unsigned int Value;
    //Move motor to bottom endstop
    //Set current motor position to 0
    //Move motor up until top endstop is hit
    //Set as max motor position
    //Allow user to enter a floor number
    // Move to floor
    // Ask user if position is ok
    // Allow user to adjust position of motor /0.1/1/10mm
    // Save floor position to global variable
}

void    MoveStepper(double mm){
    //Step Motor for amount of steps = mm
}
