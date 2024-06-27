#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include "display.h"
#include "rtc.h"

extern GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display;
extern WatchyRTC rtc;

extern tmElements_t current_time;
extern esp_sleep_wakeup_cause_t wake_up_reason;

/**
 * Runs every time device is woken up from the deep sleep
 */
void hardware_setup(String datetime = "");

/**
 * Returns Watchy board number
 */
uint8_t get_board_revision();

/**
 * Reads and returns battery voltage
 */
float get_battery_voltage();

/**
 * Puts device into deep sleep
 */
void deep_sleep();

/**
 * Called after device is woken up with RTC event (every minute)
 */
void rtc_wakeup();

/**
 * Called after device is woken up with button input
 */
void button_wakeup();

/**
 * Called after device is first turned on or restarted
 */
void boot(String datetime);
