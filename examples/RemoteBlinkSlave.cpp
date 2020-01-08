/**
 * Author : Teo-CD
 * Blinking LED example, slave.
 * The slave defines a callbacks that needs an uint8_t argument and turns
 * its LED on or off according to its value.
 */
#include "Arduino.h"
#include "I2CC.h"

using namespace I2CC;

BufferedData* toggleLED(BufferedData& args)
{
    uint8_t newLED;

    getData<uint8_t>(newLED,&args);

    digitalWrite(LED_BUILTIN,newLED);

    return nullptr; // No return value, return nullptr.
}

void setup(){
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);

    registerRPC(toggleLED,2);

    startI2CC(42);  // Does not return, so the loop() is useless.
}

void loop(){}