//
// Created by Teo-CD on 05/01/2020.
//

#include "I2CC.h"

namespace I2CC {
void __attribute__((noreturn)) startI2CC(uint8_t slaveID)
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

void sendDataBack()
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

void handleWrite(int bytesReceived)
{
    if (dataToReturn)
    {
        free(dataToReturn->dataArray);
        delete dataToReturn;
    }

    if (bytesReceived <= 0)
    {
        // TODO/FIXME : Helper function? Better way?
        dataToReturn = new ReturnData(1);
        *static_cast<uint8_t*>(dataToReturn->dataArray) = ERR_Write;
    }

    // Read the index now as the call is performed last
    unsigned int callableIndex = Wire.read();

    // Update currentArgs status and adjust allocated memory if necessary
    currentArgs.argsLength = bytesReceived - 1;
    currentArgs.readPosition = 0;
    currentArgs.args = realloc(currentArgs.args, currentArgs.argsLength);

    // Retrieve the new arguments
    Wire.readBytes(static_cast<uint8_t*>(currentArgs.args),currentArgs.argsLength);

    // Calls the user function which will take care of parsing arguments and creating the return data
    if (callables[callableIndex])
    {
        dataToReturn = callables[callableIndex]();
    }
}
}