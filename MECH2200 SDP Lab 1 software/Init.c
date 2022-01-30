/* 
 * File: Init.c  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

#include "Init.h"

void    InitialisePorts(void)
{
    //many pins of the PIC16F1779 can be defined in regards to their operation and location
    //this is known as PERIPHERAL PIN SELECTION
    //the target has been designed in a particular way that specifies where certain I/O pins
    //are allocated. Specifically the EUART TX and RX, the SPI/IIC and PWM
    // define pins for input
    
    RXPPS = PORTC_BIT7;
    SSPDATPPS = PORTC_BIT4;
    
    //define ports for output
    
    RC6PPS = TX_OUTPUT;
    RC2PPS = SDO_OUTPUT;
    RC3PPS = SCLK_OUTPUT;
    RA2PPS = PWM3_OUTPUT;
    RA3PPS = PWM4_OUTPUT;
    RA4PPS = PWM5_OUTPUT;
    RA5PPS = PWM6_OUTPUT;
    
    
    
    //set up GPIO ports
    //They default as analogue ports 
    //we are going to define them all as digital

    GPIO_1_TYPE = DIGITAL;
    GPIO_2_TYPE = DIGITAL;
    GPIO_3_TYPE = DIGITAL;
    GPIO_4_TYPE = DIGITAL;
    GPIO_5_TYPE = DIGITAL;
    GPIO_6_TYPE = DIGITAL;
    GPIO_7_TYPE = DIGITAL;
    GPIO_8_TYPE = DIGITAL;
    
    //They default as inputs and although we do not need the following instructions 
    //they show you the commands to specify them.

    GPIO_1_DIRECTION = INPUT;
    GPIO_2_DIRECTION = INPUT;
    GPIO_3_DIRECTION = INPUT;
    GPIO_4_DIRECTION = INPUT;
    GPIO_5_DIRECTION = INPUT;
    GPIO_6_DIRECTION = INPUT;
    GPIO_7_DIRECTION = INPUT;
    GPIO_8_DIRECTION = INPUT;
    
    //all ports have their weak pull-ups enabled but they are globally disabled by the
    //WPUEN bit in the OPTION_REG
    //when a port is set to an output, the weak pull up is automatically disabled
    
    GPIO_1_PULLUP = ENABLED;
    GPIO_2_PULLUP = ENABLED;
    GPIO_3_PULLUP = ENABLED;
    GPIO_4_PULLUP = ENABLED;
    GPIO_5_PULLUP = ENABLED;
    GPIO_6_PULLUP = ENABLED;
    GPIO_7_PULLUP = ENABLED;
    GPIO_8_PULLUP = ENABLED;
    
    //OPEN DRAIN CONTROL
    //many ports have the option to be open drain but in this application their
    //default of OPEN DRAIN DISABLED is maintained. The register to select this option is the ODCONx
    
    //SLEW RATE CONTROL
    //The speed at which a port can operate is defined by the SLRCONx register. The default is to 
    //have the rate limited
    
    //INPUT LEVEL CONTROL
    //this defines the voltages at which a logic level is detected and defaults as TTL
    //TTL (logic low < 0.8V logich high > 2V)
    //STL (logic low < 1V logic high > 4 V) with better noise immunity
    //for this reason STL is selected
    
    GPIO_1_LEVEL = ST;
    GPIO_2_LEVEL = ST;
    GPIO_3_LEVEL = ST;
    GPIO_4_LEVEL = ST;
    GPIO_5_LEVEL = ST;
    GPIO_6_LEVEL = ST;
    GPIO_7_LEVEL = ST;
    GPIO_8_LEVEL = ST;
    
    //set up solenoid ports
    //first ensure that the they are set to a logic low level
    
    RELAY_1_WRITE = OFF;
    RELAY_2_WRITE = OFF;
    RELAY_3_WRITE = OFF;
    
    //set the port bits to outputs
    
    RELAY_1_DIRECTION = OUTPUT;
    RELAY_2_DIRECTION = OUTPUT;
    RELAY_3_DIRECTION = OUTPUT;
    
    //set up the analogue input pins
    
    SPEED_CONTROL_TYPE = ANALOGUE;
    SPEED_CONTROL_PULLUP = DISABLED;
    AN_1_TYPE = ANALOGUE;
    AN_1_PULLUP = DISABLED;
    AN_2_TYPE = ANALOGUE;
    AN_2_PULLUP = DISABLED;
    
    //set up the analogue outputs pins
    
    DAC_1_TYPE = ANALOGUE; 
    DAC_2_TYPE = ANALOGUE;
    DAC_1_DIRECTION = OUTPUT;
    DAC_2_DIRECTION = OUTPUT;
    
    //set the PWM ports to outputs
    
    PWM_1_DIRECTION = OUTPUT;
    PWM_2_DIRECTION = OUTPUT;
    PWM_3_DIRECTION = OUTPUT;
    PWM_4_DIRECTION = OUTPUT;
    
    //set up the DRV8711 I/O control
    DRV8711_RESET_WRITE = 0b1;      //reset is active high
    DRV8711_RESET_DIRECTION = OUTPUT;
    DRV8711_STEP_WRITE = 0b0;
    DRV8711_STEP_DIRECTION = OUTPUT;
    DRV8711_DIR_WRITE = 0b0;
    DRV8711_DIR_DIRECTION = OUTPUT;
    //Note that RC0 and RC1 have no ANSEL
    //also note that these signals come from the DRV7811 drive board with
    //open drain drive therefore the weak pull ups are enabled by default 
    DRV8711_STALL_LEVEL = ST;
    DRV8711_FAULT_LEVEL = ST;
    
    //set up the SPI port directions
    SPI_CLK_WRITE = 0b0;
    SPI_CLK_DIRECTION = OUTPUT;
    SPI_SDO_WRITE = 0b0;
    SPI_SDO_DIRECTION = OUTPUT;
    SPI_SDI_TYPE = DIGITAL;
    SPI_SDI_PULLUP = DISABLED;
    SPI_SDI_LEVEL = ST;
    SPI_SDI_DIRECTION = INPUT;
    SPI_ENABLE_WRITE = 0b0;
    SPI_ENABLE_DIRECTION = OUTPUT;
    
    
    //set comms RX port bit to digital with ST levels
    RX_TYPE = DIGITAL;
    RX_LEVEL = ST;
    
    //enable global weak pullups
    GLOBAL_WEAK_PULLUP = 0;
}