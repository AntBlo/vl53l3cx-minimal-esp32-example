#pragma once
#ifndef __Vl53lxWrapper
#define __Vl53lxWrapper
#include <Arduino.h>
#include <Wire.h>
#include <vl53lx_class.h>

class Vl53lxWrapper
{
private:
  unsigned long timeSinceLastInterruptClear = millis();

  TwoWire i2c;
  VL53LX vl53lxInstance;
  VL53LX_MultiRangingData_t multiRangingData;

  void endlessLoop();

  void clearInterrupt();

public:
  Vl53lxWrapper(
      int xshutPin,
      int interruptPin,
      int sensorDeviceAddress,
      int sdaPin,
      int sclPin);

  VL53LX_MultiRangingData_t getLatestMeasurement();

  ~Vl53lxWrapper();
};
#endif