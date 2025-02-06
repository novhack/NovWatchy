#include "motor.h"

void vibrate_motor(uint8_t vibrations_count, uint16_t motor_on_ms, uint16_t motor_off_ms) {
  // Enable vibration motor output
  pinMode(VIB_MOTOR_PIN, OUTPUT);
  bool motor_on = false;
  for (int i = 0; i < vibrations_count * 2; i++) {
    motor_on = !motor_on;
    digitalWrite(VIB_MOTOR_PIN, motor_on);
    delay(motor_on ? motor_on_ms : motor_off_ms);
  }
}
