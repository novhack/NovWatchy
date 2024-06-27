#pragma once

/**
 * Runs every time device is woken up from the deep sleep
 */
void gui_setup();

/**
 * Called after device is woken up with RTC event (every minute)
 */
void rtc_wakeup();

/**
 * Called after device is woken up with button input
 */
void button_wakeup();

