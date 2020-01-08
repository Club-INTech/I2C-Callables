//
// Created by Teo-CD on 07/01/2020.
//

#ifndef I2CC_TESTS_DATAMANIPULATION_H
#define I2CC_TESTS_DATAMANIPULATION_H

#include "Arduino.h"

namespace I2CC {

/**
 * Struct allowing the return of any arbitrary data to the master.
 */
typedef struct BufferedData {
    /// Should be malloc'd
    void* dataArray;
    /// Length in bytes
    unsigned int dataLength = 0;
    unsigned int cursor = 0;

    explicit BufferedData(unsigned int size) {
        dataLength = size;
        dataArray = malloc(dataLength);
        if (!dataArray)
        {
            dataLength = 0;
        }
    }

    ~BufferedData() {
        if (!dataArray) free(dataArray);
    }
} BufferedData;

/// Swaps the order of individual bytes of an array, effectively changing the endianness.
void swapBytes(void* arrayToSwap, unsigned int size);

/// Inserts data in a BufferedData object.
/// The cursor shows where to start inserting data.
/// Checks for out of bounds.
/// Use swapBytes to work with a different endianness.
/// \param changeEndianness Swap the order of received bytes.
/// \return True if the data could be inserted
template<typename T, int size = sizeof(T)>
bool putData(const T& data, BufferedData* buffer, bool changeEndianness = false)
{
    if (size > buffer->dataLength - buffer->cursor)
    {
        return false;
    }

    // Pointer arithmetic on void* is forbidden in C++, cast to an int capable of holding a pointer
    T* inputPosition = reinterpret_cast<T*>(reinterpret_cast<intptr_t>(buffer->dataArray) + (buffer->cursor));
    *inputPosition = data;

    if (changeEndianness)
    {
        swapBytes(inputPosition,size);
    }

    // Update the position of the cursor
    buffer->cursor += size;

    return true;
}

/// Extracts data from a BufferedData object.
/// The cursor shows where to start reading in the array.
/// Checks for out of bounds.
/// Use swapBytes to work with a different endianness.
/// \param out Variable in which the data will be written
/// \param changeEndianness Swap the order of received bytes.
/// \return True if the data could be extracted
template<typename T, int size = sizeof(T)>
bool getData(T& out, BufferedData* buffer, bool changeEndianness = false)
{
    if (size > buffer->dataLength - buffer->cursor)
    {
        return false;
    }

    T* readPosition = reinterpret_cast<T*>(reinterpret_cast<intptr_t>(buffer->dataArray) + (buffer->cursor));

    if (changeEndianness)
    {
        swapBytes(readPosition,size);
    }

    out = *readPosition;

    buffer->cursor += size;

    return true;
}

}
#endif //I2CC_TESTS_DATAMANIPULATION_H
