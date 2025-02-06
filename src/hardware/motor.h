#pragma once

#include <Arduino.h>

/**
 * Vibrates the build in vibration motor
 * @param vibrations_count The amount of vibrations in total
 * @param motor_on_ms A single vibration duration in milliseconds
 * @param motor_off_ms A pause between vibrations in milliseconds
 */
void vibrate_motor(uint8_t vibrations_count = 10, uint16_t motor_on_ms = 100, uint16_t motor_off_ms = 100);
