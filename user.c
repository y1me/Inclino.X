#include <xc.h>
#include <pic16f18323.h>
#include "./h/user.h"
#include "./h/spi1.h"
#include "./h/hwprofile.h"

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

volatile char AINMux = 0x04, error = 0, Led = 0;;  
volatile char test[10]; 
volatile int AIN[4];
volatile int ADSValue, Vbatt = 0, Tbatt = 0, PrevVbatt = 0, PrevTbatt = 0, loop = 0, diff, iMux = 0;
extern volatile char RX_BUFF[32];
extern volatile char TX_BUFF[32];
extern volatile unsigned char error, *pRX_W, *pTX_stop, *pTX_W;
int eeAddr;

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

void DATAEE_WriteByte(uint16_t bAdd, uint8_t bData)
{
    uint8_t GIEBitValue = INTCONbits.GIE;

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

uint8_t DATAEE_ReadByte(uint16_t bAdd)
{
    NVMADRH = ((bAdd >> 8) & 0xFF);
    NVMADRL = (bAdd & 0xFF);
    NVMCON1bits.NVMREGS = 1;    
    NVMCON1bits.RD = 1;
    NOP();  // NOPs may be required for latency at high frequencies
    NOP();

    return (NVMDATL);
}


void delay_us(unsigned long delay)
{
    unsigned long count = 0;
    while (count < delay){
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            count++;
        }
}




void ProcessIO(void)
{
    
    if (!flag.Sys_Init)
    {

        flag.Sys_Init = 1;
    }
    
   
    
    
if (flag.Button == 1) {

    flag.Button = 0;

}
        

    if (flag.tim100u) //every 100µs
    {
        loop++;
         if (loop == 100) {

         }
        if (loop == 200) //every 20ms
        {
           
            loop = 0;
                        
        }
        flag.tim100u = 0;
    }
    
    

    
}

