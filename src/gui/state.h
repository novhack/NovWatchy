#pragma once

#include <Arduino.h>

#define WATCHFACE_STATE 0
#define MAIN_MENU_STATE 1
#define APP_STATE       2
#define FW_UPDATE_STATE 3

extern RTC_DATA_ATTR uint8_t gui_state;

uint8_t get_gui_state();
void set_gui_state(uint8_t new_state);
