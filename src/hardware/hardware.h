#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include "display.h"
#include "rtc.h"
#include "app/weather.h"

extern WatchyRTC rtc;
extern GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display;

extern tmElements_t currentTime;


extern RTC_DATA_ATTR int menuIndex;
extern RTC_DATA_ATTR bool WIFI_CONFIGURED;
extern RTC_DATA_ATTR bool BLE_CONFIGURED;
extern RTC_DATA_ATTR weatherData currentWeather;
extern RTC_DATA_ATTR int weatherIntervalCounter;
extern RTC_DATA_ATTR long gmtOffset;
extern RTC_DATA_ATTR bool alreadyInMenu;
extern RTC_DATA_ATTR tmElements_t bootTime;
extern RTC_DATA_ATTR uint32_t lastIPAddress;
extern RTC_DATA_ATTR char lastSSID[30];

float get_battery_voltage();

uint8_t get_board_revision();

void deep_sleep();

void hardware_setup(String datetime = "");

void handleButtonPress();

extern RTC_DATA_ATTR int menuIndex;
extern RTC_DATA_ATTR bool WIFI_CONFIGURED;
extern RTC_DATA_ATTR bool BLE_CONFIGURED;
