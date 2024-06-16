#pragma once

#include <Arduino.h>
#include <bma.h>

extern RTC_DATA_ATTR BMA423 sensor;

void bma_setup();

static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);

static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
