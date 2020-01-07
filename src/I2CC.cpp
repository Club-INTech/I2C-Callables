//
// Created by Teo-CD on 05/01/2020.
//

#include "I2CC.h"

using namespace I2CC;

void __attribute__((noreturn)) I2CC::startI2CC(uint8_t slaveID)
{
    // Begin as a slave and set ID
    Wire.begin(slaveID);

    // Set callbacks and loop indefinitely
    Wire.onReceive(handleWrite);
    Wire.onRequest(sendDataBack);

    // TODO : Decide if we want to give back execution
    while (true)
    {}
}

void I2CC::sendDataBack()
{
    if (dataToReturn)
    {
        Wire.write(static_cast<uint8_t*>(dataToReturn->dataArray),dataToReturn->dataLength);
    }
    else
    {
        Wire.write(ERR_Read);
    }
}

void I2CC::handleWrite(int bytesReceived)
{
    if (dataToReturn)
    {
        delete dataToReturn;
    }

    if (bytesReceived <= 0)
    {
        dataToReturn = new BufferedData(1);
        putData(ERR_Write,dataToReturn);
        return;
    }

    // Read the index now as the call is performed last
    unsigned int callableIndex = Wire.read();

    // Update currentArgs status and adjust allocated memory if necessary
    currentArgs.dataLength = bytesReceived - 1;
    currentArgs.cursor = 0;
    currentArgs.dataArray = realloc(currentArgs.dataArray, currentArgs.dataLength);

    if (!currentArgs.dataArray) return;

    // Retrieve the new arguments
    Wire.readBytes(static_cast<uint8_t*>(currentArgs.dataArray),currentArgs.dataLength);

    // Calls the user function which will take care of parsing arguments and creating the return data
    if (callables[callableIndex])
    {
        dataToReturn = callables[callableIndex](currentArgs);
    }
}

void I2CC::registerRPC(BufferedData* (*callable)(BufferedData&), unsigned int index)
{
    if (index < callableCount)
    {
        callables[index] = callable;
    }
}