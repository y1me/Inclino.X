

#ifndef _SPI1_H
#define _SPI1_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



/**
  Section: Macro Declarations
*/






char SPI1_Exchange8bit(char data);
char SPI1_Exchange8bitBuffer(char *dataIn, char bufLen, char *dataOut);
bool SPI1_IsBufferFull(void);
bool SPI1_HasWriteCollisionOccured(void);
void SPI1_ClearWriteCollisionStatus(void);


#endif // _SPI1_H
/**
 End of File
*/
