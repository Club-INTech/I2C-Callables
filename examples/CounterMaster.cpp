/**
 * Author : Teo-CD
 * Counter example, master.
 * The master will just send what it receives back and print it to Serial.
 */
#include <Arduino.h>
#include <Wire.h>

#define I2CC_MASTER // Compiles the wrapper functions for the master to use
#include "I2CC.h"

using namespace I2CC;

static constexpr uint8_t slaveID = 42;
static constexpr uint8_t counterRPC = 1;

uint8_t counter = 0;
BufferedData args(1);
BufferedData received(1);

void setup()
{
    Wire.begin();
    Serial.begin(9600);
}


void loop()
{
    putData<uint8_t>(counter,&args);
    dataRequest(slaveID,counterRPC,received,&args);
    getData<uint8_t>(counter,&received);    // Resend the incremented counter later

    args.cursor = 0;    // Reset cursor in order to reuse the same objects later.
    received.cursor = 0;

    Serial.println(counter);

    delay(500);
}
