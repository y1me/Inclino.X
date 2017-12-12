#include "./h/pic_init.h"
#include "./h/hwprofile.h"
#include <xc.h>




/************************************************************************
*						PIC INIT        								*
************************************************************************/
void Port_Init(void)  //init i/o 
{
    //Set/Unset analog pin

    ANSELAbits.ANSA5 = 0;
    ANSELCbits.ANSC2 = 0;
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    ANSELCbits.ANSC5 = 0;
    TRISCbits.TRISC2 = INPUT_PIN;
    TRISCbits.TRISC3 = OUTPUT_PIN;
    TRISAbits.TRISA5 = OUTPUT_PIN;
    TRISAbits.TRISA4 = INPUT_PIN;

}

void pps_init(void) {
  GIE = 0;
  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCKbits.PPSLOCKED = 0;		// unlock PPS
  /*****SPI*****/
    SSP1CLKPPSbits.SSP1CLKPPS = 0x12;   //RC2->MSSP1:SCK1;
    SSP1DATPPSbits.SSP1DATPPS = 0x14;   //RC4->MSSP1:SDI1;
    RC2PPSbits.RC2PPS = 0x18;   //RC2->MSSP1:SCK1;
    RC3PPSbits.RC3PPS = 0x19;   //RC3->MSSP1:SDO1;
  /*****INT EXT*****/
  INTPPSbits.INTPPS = 0x12; //RC2->INT EXT;
  
 
  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCKbits.PPSLOCKED = 1;		// lock PPS
}

void SPI1_Initialize(void)
{
    // Set the SPI1 module to the options selected in the User Interface
    
    // R_nW write_noTX; P stopbit_notdetected; S startbit_notdetected; BF RCinprocess_TXcomplete; SMP Middle; UA dontupdate; CKE Idle to Active; D_nA lastbyte_address; 
    SSP1STAT = 0x00;
    
    // SSPEN enabled; WCOL no_collision; CKP Idle:Low, Active:High; SSPM FOSC/4; SSPOV no_overflow; 
    SSP1CON1 = 0x20;
    
    // SSP1ADD 0; 
    SSP1ADD = 0x00;
}


void INT_Init(void)  //init Interrupt
{
    //I2C interrupt
    //PIE1bits.SSP1IE = 0;
    
    // INT Reg control
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    //UART interrupt
    //PIE1bits.TXIE = 0;
    
    //INT1 interrupt

	INTCONbits.INTEDG = 0;
	PIE0bits.INTE = 1;
	PIR0bits.INTF = 0;
    
    //PIE1bits.RCIE = 1;
 
    /*
	RCONbits.IINTCONPEN = 1;
	
	INTCONbits.GIE_GIEH = 1;
	INTCONbits.PEIE_GIEL = 1;

	//INT1 interrupt

	INTCON3bits.INT1IE = 1;
	INTCON3bits.INT1IP = 1;
	INTCON2bits.INTEDG1 = 0;
	
	//SPI interrupt
	
	IPR1bits.SSP1IP = 1;
	PIR1bits.SSP1IF = 0;
	PIE1bits.SSP1IE = 1;
    
  
*/
}

void Timer0_Init(void)  //init timer0
{
	/*
	T0CONbits.T08BIT = 0;
	T0CONbits.T0CS = 0;
	T0CONbits.PSA = 0;
	T0CONbits.T0PS0 = 1;
	T0CONbits.T0PS1 = 1;
	T0CONbits.T0PS2 = 1;
	//TMR0L = 75;
	INTCONbits.TMR0IE = 1;
	INTCON2bits.TMR0IP = 0;

	T0CONbits.TMR0ON = 1;
     */ 
}

void Timer1_Init(void)  //init timer1
{
	/*
	T1CONbits.RD16 = 1;
	T1CONbits.TMR1CS = 0;
	T1CONbits.T1CKPS1 = 0;
	T1CONbits.T1CKPS0 = 0;

	//TMR0L = 75;
	PIE1bits.TMR1IE = 1;
	IPR1bits.TMR1IP = 1;

	T1CONbits.TMR1ON = 0;
     */ 
}

void Timer2_Init(void)  //init timer2
{
    T2CONbits.T2OUTPS = 0; // postscaler = 1/1
    T2CONbits.T2CKPS = 1; // prescaler = 1/4
    PR2 = 0xC8;
    TMR2 = 0x00;

    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    T2CONbits.TMR2ON = 1;

}
