/* 
 * File: Init.h  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef INIT_H
#define	INIT_H

#include <xc.h> // include processor files - each processor file is guarded.  


//redefine port bit names by their application and description
//GPIO allocation
#define GPIO_1_READ_PORT        PORTBbits.RB0
#define GPIO_1_READ_LATCH       LATBbits.LATB0
#define GPIO_1_WRITE            PORTBbits.RB0
#define GPIO_1_DIRECTION        TRISBbits.TRISB0
#define GPIO_1_TYPE             ANSELBbits.ANSB0
#define GPIO_1_PULLUP           WPUBbits.WPUB0
#define GPIO_1_LEVEL            INLVLBbits.INLVB0

#define GPIO_2_READ_PORT        PORTBbits.RB2
#define GPIO_2_READ_LATCH       LATBbits.LATB2
#define GPIO_2_WRITE            PORTBbits.RB2
#define GPIO_2_DIRECTION        TRISBbits.TRISB2
#define GPIO_2_TYPE             ANSELBbits.ANSB2
#define GPIO_2_PULLUP           WPUBbits.WPUB2
#define GPIO_2_LEVEL            INLVLBbits.INLVB2

#define GPIO_3_READ_PORT        PORTBbits.RB3
#define GPIO_3_READ_LATCH       LATBbits.LATB3
#define GPIO_3_WRITE            PORTBbits.RB3
#define GPIO_3_DIRECTION        TRISBbits.TRISB3
#define GPIO_3_TYPE             ANSELBbits.ANSB3
#define GPIO_3_PULLUP           WPUBbits.WPUB3
#define GPIO_3_LEVEL            INLVLBbits.INLVB3

#define GPIO_4_READ_PORT        PORTBbits.RB4
#define GPIO_4_READ_LATCH       LATBbits.LATB4
#define GPIO_4_WRITE            PORTBbits.RB4
#define GPIO_4_DIRECTION        TRISBbits.TRISB4
#define GPIO_4_TYPE             ANSELBbits.ANSB4
#define GPIO_4_PULLUP           WPUBbits.WPUB4
#define GPIO_4_LEVEL            INLVLBbits.INLVLB4

#define GPIO_5_READ_PORT        PORTBbits.RB5
#define GPIO_5_READ_LATCH       LATBbits.LATB5
#define GPIO_5_WRITE            PORTBbits.RB5
#define GPIO_5_DIRECTION        TRISBbits.TRISB5
#define GPIO_5_TYPE             ANSELBbits.ANSB5
#define GPIO_5_PULLUP           WPUBbits.WPUB5
#define GPIO_5_LEVEL            INLVLBbits.INLVLB5

#define GPIO_6_READ_PORT        PORTCbits.RC5
#define GPIO_6_READ_LATCH       LATCbits.LATC5
#define GPIO_6_WRITE            PORTCbits.RC5
#define GPIO_6_DIRECTION        TRISCbits.TRISC5
#define GPIO_6_TYPE             ANSELCbits.ANSC5
#define GPIO_6_PULLUP           WPUCbits.WPUC5
#define GPIO_6_LEVEL            INLVLCbits.INLVLC5

#define GPIO_7_READ_PORT        PORTDbits.RD4
#define GPIO_7_READ_LATCH       LATDbits.LATD4
#define GPIO_7_WRITE            PORTDbits.RD4
#define GPIO_7_DIRECTION        TRISDbits.TRISD4
#define GPIO_7_TYPE             ANSELDbits.ANSD4
#define GPIO_7_PULLUP           WPUDbits.WPUD4
#define GPIO_7_LEVEL            INLVLDbits.INLVLD4

#define GPIO_8_READ_PORT        PORTDbits.RD5
#define GPIO_8_READ_LATCH       LATDbits.LATD5
#define GPIO_8_WRITE            PORTDbits.RD5
#define GPIO_8_DIRECTION        TRISDbits.TRISD5
#define GPIO_8_TYPE             ANSELDbits.ANSD5
#define GPIO_8_PULLUP           WPUDbits.WPUD5
#define GPIO_8_LEVEL            INLVLDbits.INLVLD5

//Solenoid allocation
#define RELAY_1_WRITE           PORTEbits.RE0
#define RELAY_1_READ            LATEbits.LATE0
#define RELAY_1_DIRECTION       TRISEbits.TRISE0

#define RELAY_2_WRITE           PORTEbits.RE1
#define RELAY_2_READ            LATEbits.LATE1
#define RELAY_2_DIRECTION       TRISEbits.TRISE1

#define RELAY_3_WRITE           PORTEbits.RE2
#define RELAY_3_READ            LATEbits.LATE2
#define RELAY_3_DIRECTION       TRISEbits.TRISE2

//DRV8711 allocation
#define DRV8711_STALL_READ          PORTCbits.RC0
// PORTC-RC0 has no analogue option
#define DRV8711_STALL_DIRECTION     TRISCbits.TRISC0
#define DRV8711_STALL_PULLUP        WPUCbits.WPUC0
#define DRV8711_STALL_LEVEL         INLVLCbits.INLVLC0

#define DRV8711_FAULT_READ          PORTCbits.RC1
// PORTC-RC1 has no analogue option
#define DRV8711_FAULT_DIRECTION     TRISCbits.TRISC1
#define DRV8711_FAULT_PULLUP        WPUCbits.WPUC1
#define DRV8711_FAULT_LEVEL         INLVLCbits.INLVLC1

#define DRV8711_DIR_WRITE           LATDbits.LATD1
#define DRV8711_DIR_READ_LATCH      LATDbits.LATD1
#define DRV8711_DIR_DIRECTION       TRISDbits.TRISD1

#define DRV8711_STEP_WRITE          LATDbits.LATD2
#define DRV8711_STEP_READ_LATCH     LATDbits.LATD2
#define DRV8711_STEP_DIRECTION      TRISDbits.TRISD2

#define DRV8711_RESET_WRITE         LATDbits.LATD3
#define DRV8711_RESET_READ_LATCH    LATDbits.LATD3
#define DRV8711_RESET_DIRECTION     TRISDbits.TRISD3

//special writes for DC motor control
//these are renamed port bits to ease understanding
#define DRV8711_A1IN_WRITE          LATDbits.LATD2
#define DRV8711_A2IN_WRITE          LATDbits.LATD1
#define DRV8711_B1IN_WRITE          PORTAbits.RA2
#define DRV8711_B2IN_WRITE          PORTAbits.RA3


//ANALOGUE INPUTS
#define SPEED_CONTROL_TYPE      ANSELAbits.ANSA0
#define SPEED_CONTROL_PULLUP    WPUAbits.WPUA0
#define AN_1_TYPE               ANSELDbits.ANSD6
#define AN_1_PULLUP             WPUDbits.WPUD6
#define AN_2_TYPE               ANSELDbits.ANSD7
#define AN_2_PULLUP             WPUDbits.WPUD7

//ANALOGUE OUTPUTS
#define DAC_1_TYPE              ANSELAbits.ANSA1
#define DAC_1_DIRECTION         TRISAbits.TRISA1

#define DAC_2_TYPE              ANSELBbits.ANSB1
#define DAC_2_DIRECTION         TRISBbits.TRISB1

//PWM outputs
#define PWM_1_DIRECTION         TRISAbits.TRISA2   

#define PWM_2_DIRECTION         TRISAbits.TRISA3   

#define PWM_3_DIRECTION         TRISAbits.TRISA4   

#define PWM_4_DIRECTION         TRISAbits.TRISA5   

//comms ports
#define RX_TYPE                 ANSELCbits.ANSC7
#define RX_LEVEL                INLVLCbits.INLVLC7

//SPI ports
#define SPI_CLK_WRITE           PORTCbits.RC3
#define SPI_CLK_DIRECTION       TRISCbits.TRISC3

#define SPI_SDO_WRITE           PORTCbits.RC2
#define SPI_SDO_DIRECTION       TRISCbits.TRISC2

#define SPI_ENABLE_WRITE        LATDbits.LATD0
#define SPI_ENABLE_DIRECTION    TRISDbits.TRISD0

#define SPI_SDI_READ            LATCbits.LATC4
#define SPI_SDI_TYPE            ANSELCbits.ANSC4
#define SPI_SDI_PULLUP          WPUCbits.WPUC4
#define SPI_SDI_LEVEL           INLVLCbits.INLVLC4
#define SPI_SDI_DIRECTION       TRISCbits.TRISC4

#define GLOBAL_WEAK_PULLUP      OPTION_REGbits.nWPUEN


//add further definitions in the 'Init.h' file and include it in all files

#define OUTPUT              0b0
#define INPUT               0b1
#define DIGITAL             0b0
#define ANALOGUE            0b1
#define DISABLED            0b0
#define ENABLED             0b1
#define TTL                 0b0
#define ST                  0b1
#define OFF                 0b0
#define ON                  0b1
#define PORTC_BIT7          0b00010111
#define PORTC_BIT4          0b00010100
#define TX_OUTPUT           0b00100100
#define SDO_OUTPUT          0b00100011
#define SCLK_OUTPUT         0b00100001
#define PWM3_OUTPUT         0b00011001      //(10 bit PWM)
#define PWM4_OUTPUT         0b00011010      //(10 bit PWM)
#define PWM5_OUTPUT         0b00011101      //(16 bit PWM)
#define PWM6_OUTPUT         0b00011110      //(16 bit PWM)


void    InitialisePorts(void);



#endif	/* INIT_H */

