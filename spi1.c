#include <xc.h>
#include "./h/hwprofile.h"
#include "./h/spi1.h"

/**
  Section: Module APIs
*/

char SPI1_Exchange8bit(char data)
{
    // Clear the Write Collision flag, to allow writing
    SPI_WCOL = 0;

    SPI_BUF = data;

    while(SPI_BF == SPI_RX_IN_PROGRESS)
    {
    }

    return (SPI_BF);
}

char SPI1_Exchange8bitBuffer(char *dataIn, char bufLen, char *dataOut)
{
    char bytesWritten = 0;
    
    if ( dataOut == NULL || dataIn == NULL)  return bytesWritten;

    if( bufLen != 0 )
    {

        while(bytesWritten < bufLen)
        {
            *dataOut = SPI1_Exchange8bit(*dataIn);
            *dataIn++;
            *dataOut++;
            bytesWritten++;
        }
    }
    return bytesWritten;

}

bool SPI1_IsBufferFull(void)
{
    return (SPI_BF);
}

bool SPI1_HasWriteCollisionOccured(void)
{
    return (SPI_WCOL);
}

void SPI1_ClearWriteCollisionStatus(void)
{
    SPI_WCOL = 0;
}
/**
 End of File
*/