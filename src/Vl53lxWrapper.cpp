#include <Arduino.h>
#include <Wire.h>
#include <vl53lx_class.h>
#include "Vl53lxWrapper.h"
#include "interrupt.h"

Vl53lxWrapper::Vl53lxWrapper(
    int xshutPin,
    int interruptPin,
    int sensorDeviceAddress,
    int sdaPin,
    int sclPin) : i2c(Wire),
                  vl53lxInstance(&i2c, xshutPin)
{
  VL53LX_Error error = VL53LX_ERROR_NONE;
  const uint32_t frequency = 400000;
  bool i2cBegan = i2c.begin(sdaPin, sclPin, frequency);
  if (!i2cBegan)
  {
    endlessLoop();
  }

  vl53lxInstance.begin();
  vl53lxInstance.VL53LX_Off();
  error = vl53lxInstance.InitSensor(sensorDeviceAddress);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), intaISR, FALLING);
  clearInterrupt();
}

void Vl53lxWrapper::endlessLoop()
{
  while (true)
  {
    delay(100);
  }
}

void Vl53lxWrapper::clearInterrupt()
{
  interruptTriggered = false;
  vl53lxInstance.VL53LX_ClearInterruptAndStartMeasurement();
  timeSinceLastInterruptClear = millis();
}

VL53LX_MultiRangingData_t Vl53lxWrapper::getLatestMeasurement()
{
  int status = 0;

  if (!interruptTriggered)
  {
    if ((millis() - timeSinceLastInterruptClear) > 1000)
    {
      clearInterrupt();
    }
    return multiRangingData;
  }

  uint8_t NewDataReady = 0;
  status = vl53lxInstance.VL53LX_GetMeasurementDataReady(&NewDataReady);
  if (status != 0 || NewDataReady == 0)
    return multiRangingData;

  status = vl53lxInstance.VL53LX_GetMultiRangingData(&multiRangingData);
  clearInterrupt();
  if (status != 0)
    return multiRangingData;

  return multiRangingData;
}

Vl53lxWrapper::~Vl53lxWrapper()
{
  i2c.~TwoWire();
}
