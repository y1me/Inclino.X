#include <xc.h>
#include "./h/hwprofile.h"
#include "./h/dsply.h"
#include "./h/spi1.h"

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

void send_display(char command, char data) {
    char RX[2], TX[2];
    TX[0] = command;
    TX[1] = data;
    CS_DSPY = 0;
    SPI1_Exchange8bitBuffer(&TX[0], 2, &RX[0]);
    CS_DSPY = 1;
}

void start_display(void)
{
    send_display(0x0F, 0x00); //disable display test
    delay_us(5);
    send_display(0x0C, 0x00); //shutdown
    delay_us(5);
    send_display(0x0C, 0x01); //Normal operation
    delay_us(5);
    send_display(0x09, 0x00);//No decode
    delay_us(5);
    send_display(0x0B, 0x05); //Scan limit
    delay_us(5);
    send_display(0x0A, 0x0A); //Intensity
    delay_us(5);
    send_display(0x01, 0x1D); //Digit0
    delay_us(5);
    send_display(0x02, 0x0F); //Digit1
    delay_us(5);
    send_display(0x03, 0x06); //Digit2
    delay_us(5);
    send_display(0x04, 0x04); //Digit3
    delay_us(5);
    send_display(0x05, 0x0F); //Digit4
    delay_us(5);
    send_display(0x06, 0x1D); //Digit5

    delay_us(1000000);

    send_display(0x09, 0x3F); //decode mode
    delay_us(5);
    send_display(0x01, 0x8F); //Digit0
    delay_us(5);
    send_display(0x02, 0x0F); //Digit1
    delay_us(5);
    send_display(0x03, 0x0F); //Digit2
    delay_us(5);
    send_display(0x04, 0x0F); //Digit3
    delay_us(5);
    send_display(0x05, 0x0F); //Digit4
    delay_us(5);
    send_display(0x06, 0x0F); //Digit5
}



            