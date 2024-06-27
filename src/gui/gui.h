#pragma once

#define WATCHFACE_STATE 0
#define MAIN_MENU_STATE 1
#define APP_STATE       2
#define FW_UPDATE_STATE 3

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

/**
 * Handler of menu button press
 */
void menu_button_handler();

/**
 * Handler of back button press
 */
void back_button_handler();

/**
 * Handler of up button press
 */
void up_button_handler();

/**
 * Handler of down button press
 */
void down_button_handler();
