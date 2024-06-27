/**
 * SRAM storage in RTC part of ESP32
 * Persistant during deep sleep
 */
#pragma once

#include <Arduino.h>
#include "rtc.h"
#include "app/weather.h"

// GUI
extern RTC_DATA_ATTR uint8_t gui_state;
extern RTC_DATA_ATTR int menuIndex;
extern RTC_DATA_ATTR weatherData currentWeather;
extern RTC_DATA_ATTR int weatherIntervalCounter;
extern RTC_DATA_ATTR long gmtOffset;
extern RTC_DATA_ATTR bool alreadyInMenu;

// Hardware
extern RTC_DATA_ATTR bool WIFI_CONFIGURED;
extern RTC_DATA_ATTR bool BLE_CONFIGURED;
extern RTC_DATA_ATTR tmElements_t bootTime;
extern RTC_DATA_ATTR uint32_t lastIPAddress;
extern RTC_DATA_ATTR char lastSSID[30];
