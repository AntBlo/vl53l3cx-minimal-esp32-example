
#ifndef __interrupt
#define __interrupt

#include <Arduino.h>

portMUX_TYPE syncInterrupt = portMUX_INITIALIZER_UNLOCKED;
bool interruptTriggered = false;

void IRAM_ATTR intaISR()
{
  portENTER_CRITICAL(&syncInterrupt);
  interruptTriggered = true;
  portEXIT_CRITICAL(&syncInterrupt);
}
#endif
