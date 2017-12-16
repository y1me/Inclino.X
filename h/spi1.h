#ifndef _SPI1_H
#define _SPI1_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void delay_us(unsigned long );
char SPI1_Exchange8bit(char );
char SPI1_Exchange8bitBuffer(char *, char , char *);
bool SPI1_IsBufferFull(void);
bool SPI1_HasWriteCollisionOccured(void);
void SPI1_ClearWriteCollisionStatus(void);


#endif // _SPI1_H
/**
 End of File
*/
