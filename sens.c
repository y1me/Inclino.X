#include <xc.h>
#include "./h/hwprofile.h"
#include "./h/dsply.h"
#include "./h/spi1.h"



void set_meas(void) {
    char RX[3], TX[3];
    TX[0] = 0x00;
    TX[1] = 0x00;
    CS_SENS = 0;
    SPI1_Exchange8bitBuffer(&TX[0], 2, &RX[0]);
    CS_SENS = 1;
    delay_us(450);
}


char get_temp(void) {
    char RX[3], TX[3];
    TX[0] = 0x08;
    TX[1] = 0x00;
    CS_SENS = 0;
    SPI1_Exchange8bitBuffer(&TX[0], 2, &RX[0]);
    CS_SENS = 1;
    delay_us(450);
    return RX[1];
}

void run_stx(void) {
    char RX[3], TX[3];
    TX[0] = 0x0E;
    TX[1] = 0x00;
    CS_SENS = 0;
    SPI1_Exchange8bitBuffer(&TX[0], 2, &RX[0]);
    CS_SENS = 1;
    delay_us(450);
}

void run_sty(void) {
    char RX[3], TX[3];
    TX[0] = 0x0F;
    TX[1] = 0x00;
    CS_SENS = 0;
    SPI1_Exchange8bitBuffer(&TX[0], 2, &RX[0]);
    CS_SENS = 1;
    delay_us(450);
}

int read_ch(char channel){
    char RX[3], TX[3];
    int data = 0;
    TX[0] = channel;
    TX[1] = 0x00;
    CS_SENS = 0;
    SPI1_Exchange8bitBuffer(&TX[0], 3, &RX[0]);
    CS_SENS = 1;
    data |= RX[1];
    data = data << 8;
    data |= RX[2];
    data = data >> 5;
    data &= 0x7FF;
    delay_us(450);
    return data;    
}



            