#include <xc.h>
#include <pic16f18323.h>
#include "./h/user.h"
//#include "./h/spi1.h"
#include "./h/hwprofile.h"
#include "./h/dsply.h"
#include "./h/sens.h"
#include <stdlib.h>
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
 
extern volatile int test[20],*ptest; 
extern volatile int Button[3];

//char RX_BUFF[8];
//char TX_BUFF[8];
char prt_val[10],*pval,dig;
int eeAddr;
extern volatile unsigned int loop;
static unsigned int loopD = 0;
int RDAX, RDAY, Dout, DoutO = 0, Calib1, Calib2;
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

int calib_offset(void) {
    //Add exit without calib 
    loop = 0;
    int loopC = 0;
    //print calib
    clean_dspl_ndec();
    send_display(0x01, 0x00);
    send_display(0x02, 0x1F);
    send_display(0x03, 0x04);
    send_display(0x04, 0x0E);
    send_display(0x05, 0x77);
    send_display(0x06, 0x4E);
    while ( loop < 30000); //wait 3s
    
    //enable first calibration measure
    while (Button[1] < 20000);
    Button[1] = 0;
    loop = 0;
    clean_dspl_ndec();
    send_display(0x01, 0x30);
    send_display(0x02, 0x1F);
    send_display(0x03, 0x04);
    send_display(0x04, 0x0E);
    send_display(0x05, 0x77);
    send_display(0x06, 0x4E);  
    while ( loop < 30000); //wait 3s before measure
    loop = 0;
    send_display(0x0C, 0x00);
    ptest = &test[0];
    
    while (loopC < 16) {
        while ( loop < 2000); //wait 200ms
        loop = 0;
        set_meas();
        RDAX = read_ch(0x10);
        RDAY = read_ch(0x11);
        *ptest = RDAX - RDAY;
        Calib1 += RDAX - RDAY;
        loopC++;
        *ptest++;    
    }
    
    send_display(0x0C, 0x01);
    Calib1 /= 16;
    
    
    loopC = 0;
    clean_dspl_ndec();
    send_display(0x01, 0x6D);
    send_display(0x02, 0x1F);
    send_display(0x03, 0x04);
    send_display(0x04, 0x0E);
    send_display(0x05, 0x77);
    send_display(0x06, 0x4E);
    
    while (Button[1] < 20000);
    Button[1] = 0;
    send_display(0x0C, 0x00);
    while ( loop < 30000); //wait 3s before measure
    loop = 0;
    
    ptest = &test[0];
    
    while (loopC < 16) {
        while ( loop < 2000); //wait 200ms
        loop = 0;
        set_meas();
        RDAX = read_ch(0x10);
        RDAY = read_ch(0x11);
        *ptest = RDAX - RDAY;
        Calib2 += RDAX - RDAY;
        loopC++;
        *ptest++;
    }
    
    send_display(0x0C, 0x01);
    Calib2 /= 16;
    //while (Button[1] < 20000);
    //Button[1] = 0;
    Calib2 += Calib1;
    Calib2 /= 2;
    return Calib2;
    
}

void update_dsp(int data) {
    clean_dspl_ndec();
    if (data > 0) {
        if (data > DEG5) { 
            send_display(0x06, 0x7F);
            return;
        }
        if (data > DEG4) { 
            send_display(0x06, 0x4F);
            send_display(0x05, 0x78);
            return;
        }
        if (data > DEG3) { 
            send_display(0x06, 0x4F);
            send_display(0x04, 0x78);
            return;
        }
        if (data > DEG2) { 
            send_display(0x06, 0x4F);
            send_display(0x03, 0x78);
            return;
        }
        if (data > DEG1) { 
            send_display(0x06, 0x4F);
            send_display(0x02, 0x78);
            return;
            }
        if (data > DEG0_8) { 
            send_display(0x06, 0x4F);
            send_display(0x01, 0x78);
            return;
        }
        if (data > DEG0_5){
            send_display(0x06, 0x4E);
            send_display(0x01, 0x78);
            send_display(0x05, 0x01);
            return;
        }
        if (data > DEG0_3){
            send_display(0x06, 0x4E);
            send_display(0x01, 0x78);
            send_display(0x04, 0x01);
            return;
        }

        send_display(0x01, 0x78);
        send_display(0x06, 0x4E);
        send_display(0x04, 0x30);
        send_display(0x03, 0x06);
        return;
        
    }
    else {
        data = abs(data);
        
        if (data > DEG5) { 
            send_display(0x01, 0x7F);
            return;
        }
        if (data > DEG4) { 
            send_display(0x01, 0x79);
            send_display(0x02, 0x4E);
            return;
        }
        if (data > DEG3) { 
            send_display(0x01, 0x79);
            send_display(0x03, 0x4E);
            return;
        }
        if (data > DEG2) { 
            send_display(0x01, 0x79);
            send_display(0x04, 0x4E);
            return;
        }
        if (data > DEG1) { 
            send_display(0x01, 0x79);
            send_display(0x05, 0x4E);
            return;
        }
        if (data > DEG0_8) { 
            send_display(0x01, 0x79);
            send_display(0x06, 0x4E);
            return;
        }
        if (data > DEG0_5){
            send_display(0x01, 0x78);
            send_display(0x06, 0x4E);
            send_display(0x02, 0x01);
            return;
        }
        if (data > DEG0_3){
            send_display(0x01, 0x78);
            send_display(0x06, 0x4E);
            send_display(0x03, 0x01);
            return;
        }
        if (data > DEG0_1){
            send_display(0x01, 0x78);
            send_display(0x06, 0x4E);
            send_display(0x03, 0x01);
            send_display(0x04, 0x01);
            return;
        }
        send_display(0x01, 0x78);
        send_display(0x06, 0x4E);
        send_display(0x04, 0x30);
        send_display(0x03, 0x06);
        return;
        
    }
    
    
}

void ProcessIO(void)
{
    
    if (!flag.Sys_Init && loop > 20)
    {  
        start_display();
        VON = 1;
        flag.Sys_Init = 1;
    }
   
    if (flag.Button == 1) {

        flag.Button = 0;

    }
    
    if (Button[0] > 20000) {
        VON = 0;    
        Button[0] = 0;
    }
    
    
    if (Button[1] > 20000) {
        Button[1] = 0;
        DoutO = calib_offset();
        Button[1] = 0;
        }
    
    
    if (Button[2] > 3000) {
        
        send_display(0x0A, intens);
        intens ++;
        if (intens > 16) intens = 0;
        Button[2] = 0;
        }
    
    
    
    if ( loop >= 200){ //every 20ms
        loop = 0;
        VON = 1;
        set_meas();
        RDAX = read_ch(0x10);
        RDAY = read_ch(0x11);
        Dout = RDAX - RDAY;
        Dout -= DoutO;
        VON = 0;
        loopD++;
        
        
    }
        
    if (loopD >= 16) //every 200ms
    {
        send_display(0x0C, 0x00);

        send_display(0x0C, 0x01);
        
        update_dsp(Dout);
        
        loopD = 0;                   
    }


}