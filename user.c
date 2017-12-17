#include <xc.h>
#include <pic16f18323.h>
#include "./h/user.h"
//#include "./h/spi1.h"
#include "./h/hwprofile.h"
#include "./h/dsply.h"
#include "./h/sens.h"
#include <math.h>
#include <stdio.h>
#include <string.h>


extern volatile struct chbits{
						unsigned RxUart:1; 
						unsigned TxUart:1; 
						unsigned Sys_Init:1; 
						unsigned tim100u:1; 
						unsigned Button:1; 
						unsigned Data1:1; 
						unsigned Data2:1; 
						unsigned cont:1;
		
					}flag ;
 
extern volatile char test[10]; 
extern volatile int Button[3];

char RX_BUFF[8];
char TX_BUFF[8];
char prt_val[10],*pval,dig;
int eeAddr;
int loop, RDAX, RDAY, Dout;
static char intens = 10;

/**
  @Summary
    Writes a data byte to Data EEPROM

  @Description
    This routine writes a data byte to given Data EEPROM location

  @Preconditions
    None

  @Param
    bAdd  - Data EEPROM location to which data to be written
    bData - Data to be written to Data EEPROM location
   Address F000h-F0FFh

  @Returns
    None

  @Example
    <code>
    uint16_t dataeeAddr = 0xF010;
    uint8_t dataeeData = 0x55;

    DATAEE_WriteByte(dataeeAddr, dataeeData);
    </code>
*/

void DATAEE_WriteByte(unsigned int bAdd, unsigned char bData)
{
    unsigned char GIEBitValue = INTCONbits.GIE;

    NVMADRH = ((bAdd >> 8) & 0xFF);
    NVMADRL = (bAdd & 0xFF);
    NVMDATL = bData;    
    NVMCON1bits.NVMREGS = 1;
    NVMCON1bits.WREN = 1;
    INTCONbits.GIE = 0;     // Disable interrupts
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
    // Wait for write to complete
    while (NVMCON1bits.WR)
    {
    }

    NVMCON1bits.WREN = 0;
    INTCONbits.GIE = GIEBitValue;   // restore interrupt enable
}

/**
  @Summary
    Reads a data byte from Data EEPROM

  @Description
    This routine reads a data byte from given Data EEPROM location

  @Preconditions
    None

  @Param
    bAdd  - Data EEPROM location from which data has to be read
   Address F000h-F0FFh

  @Returns
    Data byte read from given Data EEPROM location

  @Example
    <code>
    uint16_t dataeeAddr = 0xF010;
    uint8_t readData;

    readData = DATAEE_ReadByte(dataeeAddr);
    </code>
*/

unsigned char DATAEE_ReadByte(unsigned int bAdd)
{
    NVMADRH = ((bAdd >> 8) & 0xFF);
    NVMADRL = (bAdd & 0xFF);
    NVMCON1bits.NVMREGS = 1;    
    NVMCON1bits.RD = 1;
    NOP();  // NOPs may be required for latency at high frequencies
    NOP();

    return (NVMDATL);
}

void ProcessIO(void)
{
    
    if (!flag.Sys_Init)
    {
        VON = 1;
        start_display();
        flag.Sys_Init = 1;
    }
   
    if (flag.Button == 1) {

        flag.Button = 0;

    }
    
    if (Button[0] > 20000) {
        VON = 0;    
        Button[0] = 0;
    }
    
    
    if (Button[1] > 10000) {
        
        Button[1] = 0;
        }
    
    
    if (Button[2] > 3000) {
        
        send_display(0x0A, intens);
        intens ++;
        if (intens > 16) intens = 0;
        Button[2] = 0;
        }
    
        

    if (flag.tim100u) //every 100�s
    {
        loop++;
         if (loop == 100) {

         }
        if (loop == 10000) //every 20ms
        {
            send_display(0x0C, 0x00);
            set_meas();
            RDAX = read_ch(0x10);
            RDAY = read_ch(0x11);
            Dout = RDAX - RDAY;
            send_display(0x0C, 0x01);
            sprintf(prt_val,"%6d",Dout);
            pval = &prt_val[5];
            dig = 1;
            while(pval >= &prt_val[0]) {
                send_display(dig,*pval);
                if (*pval == 0x2D) {
                    send_display(dig, 0x0A);
                }
                if (*pval == 0x20) {
                    send_display(dig, 0x0F);
                }  
                dig++;
                *pval--;
            }
            loop = 0;
        }
            
                        
    }
        flag.tim100u = 0;
}