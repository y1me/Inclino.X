

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

#define DUMMY_DATA 0x0




uint8_t SPI1_Exchange8bit(uint8_t data);

 /**
  @Summary
    Exchanges buffer of data over SPI1

  @Description
    This routine exchanges buffer of data (of size one byte) over SPI1 bus.
    This is a blocking routine.

  @Preconditions
    The SPI1_Initialize() routine should be called
    prior to use this routine.

  @Param
    dataIn  - Buffer of data to be transmitted over SPI1.
    bufLen  - Number of bytes to be exchanged.
    dataOut - Buffer of data to be received over SPI1.

  @Returns
    Number of bytes exchanged over SPI1.

  @Example
    <code>
    uint8_t     myWriteBuffer[MY_BUFFER_SIZE];
    uint8_t     myReadBuffer[MY_BUFFER_SIZE];
    uint8_t     total;

    SPI1_Initialize();

    total = 0;
    do
    {
        total = SPI1_Exchange8bitBuffer(&myWriteBuffer[total], MY_BUFFER_SIZE - total, &myWriteBuffer[total]);

        // Do something else...

    } while(total < MY_BUFFER_SIZE);
    </code>
 */
uint8_t SPI1_Exchange8bitBuffer(uint8_t *dataIn, uint8_t bufLen, uint8_t *dataOut);

/**
  @Summary
    Gets the SPI1 buffer full status

  @Description
    This routine gets the SPI1 buffer full status

  @Preconditions
    The SPI1_Initialize() routine should be called
    prior to use this routine.

  @Param
    None

  @Returns
    true  - if the buffer is full
    false - if the buffer is not full.

  @Example
    Refer to SPI1_Initialize() for an example
 */
bool SPI1_IsBufferFull(void);

/**
  @Summary
    Gets the status of write collision.

  @Description
    This routine gets the status of write collision.

  @Preconditions
    The SPI1_Initialize() routine must have been called prior to use this routine.

  @Param
    None

  @Returns
    true  - if the write collision has occurred.
    false - if the write collision has not occurred.

  @Example
    if(SPI1_HasWriteCollisionOccured())
    {
        SPI1_ClearWriteCollisionStatus();
    }
*/
bool SPI1_HasWriteCollisionOccured(void);

/**
  @Summary
    Clears the status of write collision.

  @Description
    This routine clears the status of write collision.

  @Preconditions
    The SPI1_Initialize() routine must have been called prior to use this routine.

  @Param
    None

  @Returns
    None

  @Example
    if(SPI1_HasWriteCollisionOccured())
    {
        SPI1_ClearWriteCollisionStatus();
    }
*/
void SPI1_ClearWriteCollisionStatus(void);


#endif // _SPI1_H
/**
 End of File
*/
