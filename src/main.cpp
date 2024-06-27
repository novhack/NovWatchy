#include <Arduino.h>
#include "hardware/hardware.h"

void setup() {
  // Device either restarted or woken up from deep sleep
  hardware_setup();

  //gui_setup();

  // After all is processed put device back to sleep
  deep_sleep();
}

void loop() {
}
