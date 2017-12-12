#ifndef PIC_INIT_H
#define PIC_INIT_H


/***************** PIC INIT FUNCTIONS PROTOYPES *********************/
void Port_Init(void);
void pps_init(void);
void SPI1_Initialize(void);

void INT_Init(void);

void Timer0_Init(void);
void Timer1_Init(void);
void Timer2_Init(void);

/***********************************************************************/

#endif // PIC_INIT_H

