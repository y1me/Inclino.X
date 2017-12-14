#ifndef HARDWARE_PROFILE_H


    #define HARDWARE_PROFILE_H

    #define CLOCK_FREQ          32000000

    #define DUMMY_DATA          0x00
/** DEBUG ***********************************************************/


/** DEBUG ***********************************************************/
	#define	TRIS_PORTTEST		
	#define PORTTEST          
    #define PORTTEST2           

/** TRIS ***********************************************************/
	#define INPUT_PIN           1
	#define OUTPUT_PIN          0
	#define INPUT_PORT          0xFF
	#define OUTPUT_PORT         0x00

/** SPI ***********************************************************/
    #define SPI_RX_IN_PROGRESS  0
    #define SPI_WCOL            SSP1CON1bits.WCOL
    #define SPI_BUF             SSP1BUF
    #define SPI_BF              SSP1STATbits.BF
    #define CS_DSPY             PORTCbits.RC1
    #define CS_SENS             PORTCbits.RC5

/** INT EXT ************************************************************/
    #define INT_EXT_F           PIR0bits.INTF

/** Power ************************************************************/
    #define VON                 PORTCbits.RC0

/** Push Button ************************************************************/
    #define BUTT0               PORTAbits.RA2
    #define BUTT1               PORTAbits.RA4
    #define BUTT2               PORTAbits.RA5

/** Timer Display ************************************************************/

 	#define TIM_PWM_INT_F           PIR1bits.TMR2IF
	#define TIM_PWM_REG             TMR2
	#define EN_TIM_PWM              TMR2ON

 #endif  //HARDWARE_PROFILE_H
