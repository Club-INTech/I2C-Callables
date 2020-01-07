//
// Created by Teo-CD on 05/01/2020.
//

#ifndef I2CC_TESTS_I2CC_H
#define I2CC_TESTS_I2CC_H

#include "Wire.h"


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

    BufferedData() = default;

    explicit BufferedData(unsigned int size) {
        dataLength = size;
        dataArray = malloc(dataLength);
    }
} BufferedData;

static constexpr int callableCount = 20;

// TODO : Maybe make an enum
static constexpr uint8_t ERR_Read = 255;
static constexpr uint8_t ERR_Write = 254;

/// Array of function pointers on user-defined functions.
/// BufferedData* should be nullptr if not used, else instantiation is left to the function.
static BufferedData* (*callables[callableCount])() = {nullptr};


/// Data saved after the master sent a write command, to be retrieved with a requestFrom.
/// Data will be cleared after another write command.
static BufferedData* dataToReturn = nullptr;

// Updated during each handleWrite() before calling user function
static BufferedData currentArgs;

/// Initialize I2C slave then loop indefinitely, handle communications and function calls.
void startI2CC(uint8_t slaveID);

void sendDataBack();
void handleWrite(int bytesReceived);
}

#endif //I2CC_TESTS_I2CC_H
