/**
 * Author : Teo-CD
 * Blinking LED example, master.
 * The master will blink its own LED and make the slave blink in opposite.
 */
#include <Arduino.h>
#include <Wire.h>

#define I2CC_MASTER // Compiles the wrapper functions for the master to use
#include "I2CC.h"

using namespace I2CC;

static constexpr uint8_t slaveID = 42;
static constexpr uint8_t blinkRPC = 2;

uint8_t ledStatus = true;
BufferedData args(1);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Wire.begin();

    digitalWrite(LED_BUILTIN, HIGH);
}


void loop()
{
    putData<uint8_t>(ledStatus,&args);
    executeRPC(slaveID,blinkRPC,&args);
    ledStatus = false;
    args.cursor = 0;    // Reset cursor in order to reuse the same object later.

    digitalWrite(LED_BUILTIN,LOW);
    delay(500);

    putData<uint8_t>(ledStatus,&args);
    executeRPC(slaveID,blinkRPC,&args);
    ledStatus = true;
    args.cursor = 0;

    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
}
