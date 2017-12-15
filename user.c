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
 
extern volatile char test[10]; 
extern volatile int Button[3];

char RX_BUFF[8];
char TX_BUFF[8];
int eeAddr;
int loop;

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

void start_display(void)
{
    RX_BUFF[0] = 0; //disable display test
    TX_BUFF[0] = 0x0F;
    TX_BUFF[1] = 0x00;
    CS_DSPY = 0;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1; 

    RX_BUFF[0] = 0; //shutdown
    TX_BUFF[0] = 0x0C;
    TX_BUFF[1] = 0x00;
    CS_DSPY = 0;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;

    RX_BUFF[0] = 0; //Normal operation
    TX_BUFF[0] = 0x0C;
    TX_BUFF[1] = 0x01;
    CS_DSPY = 0;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;

    delay_us(5);

    CS_DSPY = 0; //No decode
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x09;
    TX_BUFF[1] = 0x00;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;

    delay_us(5);
    
    CS_DSPY = 0; //Scan limit
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x0B;
    TX_BUFF[1] = 0x05;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;

    delay_us(5);

    CS_DSPY = 0; //Intensity
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x0A;
    TX_BUFF[1] = 0x0A;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;

    delay_us(5);

    CS_DSPY = 0; //Digit0
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x01;
    TX_BUFF[1] = 0x1D;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit1
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x02;
    TX_BUFF[1] = 0x0F;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit2
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x03;
    TX_BUFF[1] = 0x06;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit3
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x04;
    TX_BUFF[1] = 0x04;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit4
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x05;
    TX_BUFF[1] = 0x0F;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit5
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x06;
    TX_BUFF[1] = 0x1D;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;

    delay_us(1000000);
    
    CS_DSPY = 0; //No decode
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x09;
    TX_BUFF[1] = 0x3F;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit0
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x01;
    TX_BUFF[1] = 0x00;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit1
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x02;
    TX_BUFF[1] = 0x00;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit2
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x03;
    TX_BUFF[1] = 0x00;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit3
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x04;
    TX_BUFF[1] = 0x00;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit4
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x05;
    TX_BUFF[1] = 0x00;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    
    CS_DSPY = 0; //Digit5
    RX_BUFF[0] = 0;
    TX_BUFF[0] = 0x06;
    TX_BUFF[1] = 0x00;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_DSPY = 1;
    

}

void ProcessIO(void)
{
    
    if (!flag.Sys_Init)
    {
        start_display();
        flag.Sys_Init = 1;
    }
    RX_BUFF[0] = 0;
    RX_BUFF[1] = 0;
    RX_BUFF[2] = 0;
    CS_SENS = 0; //Digit5

    TX_BUFF[0] = 0x08;
    TX_BUFF[1] = 0x00;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
    CS_SENS = 1;
    
    RX_BUFF[0] = 0;
    RX_BUFF[1] = 0;
    RX_BUFF[2] = 0;
    CS_SENS = 0; //Digit5

    TX_BUFF[0] = 0x10;
    TX_BUFF[1] = 0x00;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 3, &RX_BUFF[0]);
    CS_SENS = 1;
    
    RX_BUFF[0] = 0;
    RX_BUFF[1] = 0;
    RX_BUFF[2] = 0;
    CS_SENS = 0; //Digit5

    TX_BUFF[0] = 0x11;
    TX_BUFF[1] = 0x00;
    SPI1_Exchange8bitBuffer(&TX_BUFF[0], 3, &RX_BUFF[0]);
    CS_SENS = 1;
   
    
    
    if (flag.Button == 1) {

        flag.Button = 0;

    }
    
    if (Button[0] > 10000) {
            CS_DSPY = 0;
            RX_BUFF[0] = 0;
            TX_BUFF[0] = 0x01;
            TX_BUFF[1] = 0x06;
            SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
            CS_DSPY = 1;
            Button[0] = 0;
        }
    
    
    if (Button[1] > 10000) {
            CS_DSPY = 0;
            RX_BUFF[0] = 0;
            TX_BUFF[0] = 0x01;
            TX_BUFF[1] = 0x08;
            SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
            CS_DSPY = 1;            
            Button[1] = 0;
        }
    
    
    if (Button[2] > 10000) {
            CS_DSPY = 0;
            RX_BUFF[0] = 0;
            TX_BUFF[0] = 0x01;
            TX_BUFF[1] = 0x89;
            SPI1_Exchange8bitBuffer(&TX_BUFF[0], 2, &RX_BUFF[0]);
            CS_DSPY = 1;           
            Button[2] = 0;
        }
    
        

    if (flag.tim100u) //every 100�s
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

