#include <Arduino.h>
#include <Wire.h>
#include "Vl53lxWrapper.h"

int xshutPin = 33;
int interruptPin = 32;
int sensorDeviceAddress = 0x12;
int sdaPin = 21;
int sclPin = 22;
Vl53lxWrapper *vl53lxWrapper;

void setup()
{
  const unsigned long baudRate = 115200;
  Serial.begin(baudRate);
  while (!Serial)
    delay(1000);
  Serial.println("Serial up.");
  vl53lxWrapper = new Vl53lxWrapper(xshutPin,
                                    interruptPin,
                                    sensorDeviceAddress,
                                    sdaPin,
                                    sclPin);
}

void loop()
{
  VL53LX_MultiRangingData_t measurement = vl53lxWrapper->getLatestMeasurement();
  Serial.println(measurement.RangeData->RangeMilliMeter);
}
