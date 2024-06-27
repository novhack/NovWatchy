#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include "display.h"
#include "rtc.h"

extern WatchyRTC rtc;
extern GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display;

extern tmElements_t currentTime;

float get_battery_voltage();

uint8_t get_board_revision();

void deep_sleep();

void hardware_setup(String datetime = "");

void handleButtonPress();
