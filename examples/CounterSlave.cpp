/**
 * Author : Teo-CD
 * Counter example, slave.
 * The slave increment the received data and sends it back.
 */
#include "Arduino.h"
#include "I2CC.h"

using namespace I2CC;

BufferedData* receiveAndIncrement(BufferedData& args)
{
    uint8_t counter;

    getData<uint8_t>(counter,&args);

    BufferedData* returnData = new BufferedData(1);
    counter++;
    putData<uint8_t>(counter,returnData);

    return returnData;
}

void setup(){
    registerRPC(receiveAndIncrement,1);

    startI2CC(42);  // Does not return, so the loop() is useless.
}

void loop(){}