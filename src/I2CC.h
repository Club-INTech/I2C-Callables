//
// Created by Teo-CD on 05/01/2020.
//

#ifndef I2CC_TESTS_I2CC_H
#define I2CC_TESTS_I2CC_H

#include "Wire.h"
#include "DataManipulation.h"

namespace I2CC {

#if !defined(I2CC_MASTER)

static constexpr int callableCount = 20;

// TODO : Maybe make an enum
static constexpr uint8_t ERR_Read = 255;
static constexpr uint8_t ERR_Write = 254;

/// Array of function pointers on user-defined functions.
/// BufferedData* should be nullptr if not used, else instantiation is left to the function.
static BufferedData* (*callables[callableCount])(BufferedData&) = {nullptr};


/// Data saved after the master sent a write command, to be retrieved with a requestFrom.
/// Data will be cleared after another write command.
static BufferedData* dataToReturn = nullptr;

// Updated during each handleWrite() before calling user function
static BufferedData currentArgs(0);

/// Initialize I2C slave, handle communications and function calls.
/// Can loop indefinitely or return.
void startI2CC(uint8_t slaveID, bool infiniteLoop = true);

// Functions that handle Wire I2C callbacks
void sendDataBack();
void handleWrite(int bytesReceived);

/// Adds callbacks to the callables array.
/// Replaces callback if one is already present at index.
void registerRPC(BufferedData* (*callable)(BufferedData&), unsigned int index);

#else

//
// Helper functions for the master.
//

/// Execute a callable function on a slave, arguments optional.
void executeRPC(uint8_t slaveID, uint8_t rpcID, BufferedData* arguments = nullptr);
/// Execute a callable function and retrieve the data it sends back, arguments optional.
bool dataRequest(uint8_t slaveID, uint8_t rpcID, BufferedData& returnData, BufferedData* arguments = nullptr);

#endif

}

#endif //I2CC_TESTS_I2CC_H
