/* 
 * File: SPI.c  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

#include "SPI.h"
#include "Init.h"
#include "Timer.h"
#include "Comms.h"


extern volatile unsigned int GLOBAL_StepperPowerStatus;
extern volatile unsigned char GLOBAL_ResultString[RESULT_STRING_LENGTH];


//*******************************************************
//Initialise SPI

void    InitialiseSPI(void)
{
    //initialise SPI mode 0 
    //mode 0 i.e. data is sampled on the rising edge of the clock and shifted out on the falling edge
    //the clock idle state is low 
    
    SSP1CON1bits.SSPEN = 0b0;               //disable SPI        
    SSP1ADD = 63;                           //DRV8711 SPI speed limit is 250 KHz so set for 125 KHz
    SSP1CON1bits.SSPM = 0b1010;             //set for SPI master mode FOSC/(SSP1ADD + 1)
    SSP1CON3bits.BOEN = 0b1;                //load rx byte regardless of the state of BF flag
    SPI_CLOCK_IDLE_STATE_CKP = 0b0;         //clock idle state is low
    SPI_RX_BIT_SAMPLE_SMP = 0b1;            //sample at end of data time
    SPI_TX_CLOCK_EDGE_CKE = 0b1;            //Data changes when clock goes from active to idle (high to low)
    SSP1CON1bits.SSPEN = 0b1;               //enable SPI        
    
}



//***************************************************************
//SPI Transmit

void    WriteSPI(unsigned int RegisterAddress, unsigned int Value)
{
    unsigned char TxByteHigh;
    unsigned char TxByteLow;
    unsigned char RxByte;
    unsigned int RxWord;

    //prep data
    //construct write command 
    RegisterAddress = RegisterAddress << 12;
    Value = Value | RegisterAddress;
    TxByteHigh = (char)(Value >> 8);
    TxByteLow = (char)Value;

    //enable device
    SPI_ENABLE_WRITE = 0b1;
                
    //clear overflow condition
    SSP1CON1bits.SSPOV = 0;
    //transmit high byte
    SSP1BUF = TxByteHigh;
    //wait for received byte
    while(SPI_RX_BUFFER_FULL == 0);
    //get received SPI byte
    RxByte = SSP1BUF;
    //shift and save
    RxWord = (unsigned int)RxByte << 8;
    //transmit low byte
    SSP1BUF = TxByteLow;
    //wait for received byte
    while(SPI_RX_BUFFER_FULL == 0);
    //get received SPI byte
    RxByte = SSP1BUF;
    //shift and save
    RxWord = RxWord | (unsigned int)RxByte;
    
    //disable chip select
    SPI_ENABLE_WRITE = 0b0;
    
}

//***************************************************************
//SPI Receive

unsigned int    ReadSPI(unsigned int RegisterAddress)
{
    unsigned char TxByteHigh;
    unsigned char TxByteLow;
    unsigned char RxByte;
    unsigned int RxWord;
    unsigned int Value;

    //prepare data
    //construct read command. Note the data is just 0's 
    RegisterAddress = RegisterAddress << 12;
    Value = RegisterAddress | 0x8000;   //set MSB for read
    TxByteHigh = (char)(Value >> 8);
    TxByteLow = (char)Value;

    //enable device
    SPI_ENABLE_WRITE = 0b1;
                
    //clear overflow condition
    SSP1CON1bits.SSPOV = 0;
    //transmit high byte
    SSP1BUF = TxByteHigh;
    //wait for received byte
    while(SPI_RX_BUFFER_FULL == 0);
    //get received SPI byte
    RxByte = SSP1BUF;
    //shift and save
    RxWord = (unsigned int)RxByte << 8;
    //transmit low byte 
    SSP1BUF = TxByteLow;
    //wait for received byte
    while(SPI_RX_BUFFER_FULL == 0);
    //get received SPI byte
    RxByte = SSP1BUF;
    //shift and save
    RxWord = RxWord | (unsigned int)RxByte;

    //mask off upper 4 bits i.e. address and read/write bit
    RxWord = RxWord & 0x0fff;
    
    //disable device
    SPI_ENABLE_WRITE = 0b0;
    
    return RxWord;
}



//*************************************************
//initialise stepper motor driver DRV8711 device
void    InitialiseDRV8711(void)
{
    //declare variables
    unsigned int SPIAddress = 0;
    unsigned int SPIValue = 0;

    
    //wait 10 ms
    Delay(10000);
    //release drive reset
    DRV8711_RESET_WRITE = 0b0;
    //wait 10 ms
    Delay(10000);
    
    //load driver with parametric settings
    SPIAddress = DRV_CTRL_REG;
    SPIValue = DRV_CTRL_REG_VALUE; 
    WriteSPI(SPIAddress,SPIValue);
        
    SPIAddress = DRV_TORQUE_REG;
    SPIValue = DRV_TORQUE_REG_VALUE; 
    WriteSPI(SPIAddress,SPIValue);
    
    SPIAddress = DRV_OFF_REG;
    SPIValue = DRV_OFF_REG_VALUE;
    WriteSPI(SPIAddress,SPIValue);
    
    SPIAddress = DRV_BLANK_REG;
    SPIValue = DRV_BLANK_REG_VALUE;
    WriteSPI(SPIAddress,SPIValue);
    
    SPIAddress = DRV_DECAY_REG;
    SPIValue = DRV_DECAY_REG_VALUE;
    WriteSPI(SPIAddress,SPIValue);
    
    SPIAddress = DRV_STALL_REG;
    SPIValue = DRV_STALL_REG_VALUE;
    WriteSPI(SPIAddress,SPIValue);
    
    SPIAddress = DRV_DRIVE_REG;
    SPIValue = DRV_DRIVE_REG_VALUE;
    WriteSPI(SPIAddress,SPIValue);
}


//Enable stepper motor and set step time to the value in
//GLOBAL_STEP_INTERVAL
void    MotorOn (void)
{
    //declare variables
    unsigned int SPIAddress;
    unsigned int SPIValue;
     
    //get the present value of the control register
    SPIAddress = DRV_CTRL_REG;
    //get the present value of the control register
    SPIValue = ReadSPI(SPIAddress);
    //set the drive enable bit
    SPIValue = SPIValue | ENABLE_DRIVE;
    WriteSPI(SPIAddress,SPIValue);  
}


//disable stepper motor
void    MotorOff (void)
{
    //declare variables
    unsigned int SPIAddress;
    unsigned int SPIValue;
    
    //get the present value of the control register
    SPIAddress = DRV_CTRL_REG;
    SPIValue = ReadSPI(SPIAddress);
    //clear the drive enable bit
    SPIValue = SPIValue & DISABLE_DRIVE;
    WriteSPI(SPIAddress,SPIValue); 
}


//load torque current
void    LoadTorque (unsigned int TorqueValue)
{
    //declare variables
    unsigned int SPIAddress;
    unsigned int SPIValue;
    
    //get the present value of the control register
    SPIAddress = DRV_TORQUE_REG;
    SPIValue = ReadSPI(SPIAddress);
    //clear the present torque value
    SPIValue = SPIValue & CLEAR_TORQUE;
    //load new torque value
    SPIValue = SPIValue | TorqueValue;
    WriteSPI(SPIAddress,SPIValue);       
}



//************************************
//set the DRV8711 MODE passed to the function
//stepper or PWM

void    SetDRV8711_Mode(unsigned int Mode)
{
    //declare variables
    unsigned int SPIAddress;
    unsigned int SPIValue;
    
    //get the present value of the control register
    SPIAddress = DRV_OFF_REG;
    SPIValue = ReadSPI(SPIAddress);
    
    //clear the present mode value
    SPIValue = SPIValue & CLEAR_MODE;
    //load the new mode
    switch(Mode)
    {
        case STEPPER_MODE:  //set the DRV8711 to stepper mode
            SPIValue = SPIValue | SET_STEPPER_MODE;
            break;
            
        case PWM_MODE:  //set the DRV8711 to PWM mode
            SPIValue = SPIValue | SET_PWM_MODE;
            break;
            
        //no default    
    }
    //write new value to DRV8711
    WriteSPI(SPIAddress,SPIValue);       
}


//***************************************************************
//Write to the LED display through SPI format
//pass the command, a pointer to the string to be actioned and also either the number of characters
//or the cursor position depending on the command

void    WriteLED_Display(unsigned char Command, volatile unsigned char *StringPointer, unsigned int Value )
{
    //enable device
    //note that the CS is active low
    GPIO_8_WRITE = 0b0;
                
    //clear overflow condition
    SSP1CON1bits.SSPOV = 0;
    
    //test for Command
    switch(Command)
    {
        case LED_DISPLAY_CLEAR:
            //Clear display 
            //transmit command
            SPI_LED_DisplayWrite(Command);
            //no more characters
            break;
            
        case LED_DISPLAY_DCA_CONTROL:
            //Decimal control
            //controls the dots on the display
            //transmit command
            SPI_LED_DisplayWrite(Command);
            //send the character in the passed string pointer
            SPI_LED_DisplayWrite(*StringPointer);            
            //no more characters
            break;
            
        case LED_DISPLAY_CURSOR_CONTROL:
            //Set for cursor control
            SPI_LED_DisplayWrite(Command);
            //Set cursor position passed in 'Value'(left justified, 0 through to 3)
            SPI_LED_DisplayWrite(Value);
            //display string until NULL
            //test for any characters to be displayed
            while(*StringPointer != NULL)
            {
                //send the character in the passed string pointer
                SPI_LED_DisplayWrite(*StringPointer);  
                //increment the character pointer 
                StringPointer++;
            }
            //no more characters
            break;
            
        case LED_DISPLAY_BRIGHTNESS:
            //Set brightness control
            //transmit command
            SPI_LED_DisplayWrite(Command);
            //send the character in the passed string pointer
            SPI_LED_DisplayWrite(*StringPointer);            
            //no more characters
            break;
            
        case LED_DISPLAY_DIGIT_1:
            //Digit 1 segment control
            SPI_LED_DisplayWrite(Command);
            //send the value in the first location of the passed array pointer
            SPI_LED_DisplayWrite(*StringPointer);  
            break;
            
        case LED_DISPLAY_DIGIT_2:
            //Digit 2 segment control
            SPI_LED_DisplayWrite(Command);
            //send the value in the first location of the passed array pointer
            SPI_LED_DisplayWrite(*StringPointer);  
            break;
            
        case LED_DISPLAY_DIGIT_3:
            //Digit 3 segment control
            SPI_LED_DisplayWrite(Command);
            //send the value in the first location of the passed array pointer
            SPI_LED_DisplayWrite(*StringPointer);  
            break;

        case LED_DISPLAY_DIGIT_4:
            //Digit 4 segment control
            SPI_LED_DisplayWrite(Command);
            //send the value in the first location of the passed array pointer
            SPI_LED_DisplayWrite(*StringPointer);  
            break;
            
            //no default
    }
    
    //no more data    
    //disable chip select i.e. set high
    GPIO_8_WRITE = 0b1;
}


//***********************************************
//SPI LED display write

void    SPI_LED_DisplayWrite(unsigned char Value)
{
    //transmit byte
    SSP1BUF = Value;
    //wait for transmission to be completed
    while(SPI_RX_BUFFER_FULL == 0);
}
