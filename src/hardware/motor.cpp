#include "motor.h"

void motor_vibrate(uint8_t intervalMs, uint8_t length) {
    pinMode(VIB_MOTOR_PIN, OUTPUT);
    bool motorOn = false;
    for (int i = 0; i < length; i++) {
        motorOn = !motorOn;
        digitalWrite(VIB_MOTOR_PIN, motorOn);
        delay(intervalMs);
    }
}
