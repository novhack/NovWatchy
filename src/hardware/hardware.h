#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include "display.h"
#include "rtc.h"

extern GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display;
extern WatchyRTC rtc;
extern tmElements_t current_time;

// Filled up after every deep sleep wake up
extern esp_sleep_wakeup_cause_t wake_up_reason;
extern uint64_t wake_up_button_bit;

/**
 * Runs every time device is woken up from the deep sleep
 */
void hardware_setup(String datetime = "");

/**
 * Called after device is first turned on or restarted
 */
void boot(String datetime);

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
