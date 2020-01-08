//
// Created by Teo-CD on 08/01/2020.
//

#include "DataManipulation.h"

void I2CC::swapBytes(void* arrayToSwap, unsigned int size)
{
    uint8_t* byteArray = static_cast<uint8_t*>(arrayToSwap);
    for (unsigned int i = 0;i<size/2;i++)
    {
        uint8_t save = byteArray[i];
        byteArray[i] = byteArray[size-1-i];
        byteArray[size-1-i] = save;
    }
}