/* 
 * File: SPI.h  
 * Author: Roger Berry
 * Comments: None
 * Revision history: 1.0 27/12/2019
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SPI_H
#define	SPI_H

//define register addresses
#define DRV_CTRL_REG    0
#define DRV_TORQUE_REG  1
#define DRV_OFF_REG     2
#define DRV_BLANK_REG   3
#define DRV_DECAY_REG   4
#define DRV_STALL_REG   5
#define DRV_DRIVE_REG   6
#define DRV_STATUS_REG  7

//define DRV8711 parameter masks
#define ENABLE_DRIVE                        0b0000000000000001
#define DISABLE_DRIVE                       0b1111111111111110
#define STEP_MASK                           0b0000000001111000
#define DEAD_TIME_MASK                      0b0000110000000000
#define TORQUE_MASK                         0b0000000011111111
#define CLEAR_TORQUE                        0b1111111100000000
#define TOFF_MASK                           0b0000000011111111
#define TBLANK_MASK                         0b0000000011111111
#define ABT_MASK                            0b0000000100000000
#define TDECAY_MASK                         0b0000000011111111
#define DECAY_MODE_MASK                     0b0000011100000000
#define OVERCURRENT_THRESHOLD_MASK          0b0000000000000011
#define OVERCURRENT_DEGLITCH_MASK           0b0000000000001100
#define LOW_SIDE_DRIVE_TIME_MASK            0b0000000000110000
#define HIGH_SIDE_DRIVE_TIME_MASK           0b0000000011000000
#define LOW_SIDE_DRIVE_CURRENT_MASK         0b0000001100000000
#define HIGH_SIDE_DRIVE_CURRENT_MASK        0b0000110000000000
#define CLEAR_MODE                          0b0000000011111111
#define SET_STEPPER_MODE                    0b0000000000000000
#define SET_PWM_MODE                        0b0000000100000000

//define DRV8711 register values

    // From high bits to low bits
    // DTIME = 850ns = 0b11 (default))
    // ISGAIN = 40 = 0b11 (not default))
    // EXSTALL = internal stall detect = 0b0 (not default)
    // MODE = 1/4 steps = 0b0010 (default))
    // RSTEP = no action = 0b0 (default))
    // DIR = via pin = 0b0 (default)
    // ENBL = Motor disabled = 0b0 (default)
#define DRV_CTRL_REG_VALUE 0b111100010000;

    // From high bits to low bits
    // Reserved bit
    // SMPLTH = 100us 0b001 (default)
    // TORQUE setting = 0x1E (75 mA not default)
#define DRV_TORQUE_REG_VALUE 0x11E

    // From high bits to low bits
    // Reserved bits
    // PWMMODE = use internal indexer (default)
    // TOFF = 16.5 us (default)
#define DRV_OFF_REG_VALUE 0x030

    // From high bits to low bits
    // Reserved bits
    // ABT = 1 enable adaptive blanking
    // TBLANK = 3.56us i.e. 0x80 (default)
#define DRV_BLANK_REG_VALUE 0x080

    // From high bits to low bits
    // Reserved bit
    // DECMOD =  auto 0b101 (not default)
    // TDECAY = 0x10 (default)
#define DRV_DECAY_REG_VALUE 0x510

    // From high bits to low bits
    // VDIV = dive 16 = 0b01 (not default)
    // SDCNT = after 1st step = 0b00 (default))
    // SDTHR = 0x10 (not default)
#define DRV_STALL_REG_VALUE 0x410

    // From high bits to low bits
    // IDRIVEP = 150ma = 0b10 (default)
    // IDRIVEN = 300ma = 0b10 (default)
    // TDRIVEP = 500ns = 0b01 (defaut))
    // TDRIVEN = 500ns = 0b01 (default)
    // OCPDEG = 4us = 0b10 (default)
    // OCPTH = 500mV = 0b01 (default))
#define DRV_DRIVE_REG_VALUE 0b101001011001



//define labels for register access

#define SPI_CLOCK_IDLE_STATE_CKP    SSP1CON1bits.CKP
#define SPI_RX_BIT_SAMPLE_SMP       SSP1STATbits.SMP
#define SPI_TX_CLOCK_EDGE_CKE       SSP1STATbits.CKE
#define SPI_RX_BUFFER_FULL          SSP1STATbits.BF

//define modes

#define STEPPER_MODE                0
#define PWM_MODE                    1

#define LED_DISPLAY_CLEAR           0x76
#define LED_DISPLAY_DCA_CONTROL     0x77
#define LED_DISPLAY_CURSOR_CONTROL  0x79
#define LED_DISPLAY_BRIGHTNESS      0x7A
#define LED_DISPLAY_DIGIT_1         0x7B
#define LED_DISPLAY_DIGIT_2         0x7C
#define LED_DISPLAY_DIGIT_3         0x7D
#define LED_DISPLAY_DIGIT_4         0x7E

void    InitialiseSPI(void);
void    WriteSPI(unsigned int, unsigned int);
unsigned int    ReadSPI(unsigned int);
void    InitialiseDRV8711(void);
void    MotorOn(void);
void    MotorOff(void);
void    LoadTorque(unsigned int);
void    SetDRV8711_Mode(unsigned int);
void    WriteLED_Display(unsigned char, volatile unsigned char *, unsigned int);
void    SPI_LED_DisplayWrite(unsigned char);



#endif	/* SPI_H */

