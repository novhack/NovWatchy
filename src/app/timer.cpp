#include "timer.h"

#include <Arduino.h>

#include "config.h"
#include "gui/gen_menu.h"
#include "gui/menu_items.h"
#include "gui/time_digits.h"
#include <Fonts/FreeMonoBold9pt7b.h>
#include "hardware/button.h"
#include "hardware/hardware.h"
#include "hardware/motor.h"
#include "hardware/rtc_sram.h"

int8_t set_index = SET_MINUTE;
int8_t digit_blink_state = 1;

int8_t menu_index = 0;
timer_app_state_t timer_app_state = SAVED_DURATIONS_STATE;

unsigned long start_timestamp;
unsigned long timestamp_diff;

uint32_t seconds = 0;

menu_item_t timer_menu_items[TIMER_SAVED_SETTINGS_COUNT];

void timer_app_main() {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);

  set_buttons_pins_as_inputs();

  // Reset state variables
  set_index = SET_MINUTE;
  digit_blink_state = 1;
  menu_index = 0;
  timer_app_state = SAVED_DURATIONS_STATE;

  char mmss_strings[TIMER_SAVED_SETTINGS_COUNT][6];
  for (int i = 0; i < TIMER_SAVED_SETTINGS_COUNT; i++) {
    seconds_to_string(uint32_t(seconds - timestamp_diff), mmss_strings[i]);
    timer_menu_items[i] = { mmss_strings[i], nullptr, false, false };
  }

  while(1) {
    // Handle inputs
    switch (timer_app_state) {
      case DURATION_SETUP_STATE:
        // Set timer duration, if false is returned the app loop should be broken
        if (!handle_duration_setup_inputs()) return;
        display_duration_setup();
        break;
      case SAVED_DURATIONS_STATE:
        // Select duration from list of saved durations
        if (!handle_saved_duration_state_inputs()) return;
        display_duration_setup();
        display_saved_duration();
        break;
      case TIMER_COUNTDOWN_STATE:
        calc_duration();
        display_countdown_page();
        delay(500);
        break;
      case TIMER_FINISHED_STATE:
        if (!display_finished_page()) return;
        break;
    }

    display.display(true); // partial refresh

    // If both top buttons are pressed exit the app
    if (are_top_buttons_pressed()) {
      return;
    }
  }
}

bool handle_duration_setup_inputs() {
  // Process inputs
  if (is_back_button_pressed()) {
    if (set_index != SET_HOUR) {
      set_index--;
    } else {
      return false;
    }
  }

  if (is_menu_button_pressed()) {
    if (set_index == SET_HOUR) {
      set_index++;
    } else {
      // Start timer and show countdown page
      seconds = (timer_hour * 60 + timer_minute) * 60;
      pushToFront(timer_saved_settings, seconds);
      start_timer();
      timer_app_state = TIMER_COUNTDOWN_STATE;
    }
  }

  digit_blink_state = 1 - digit_blink_state;

  if (is_down_button_pressed()) {
    digit_blink_state = 1;
    switch (set_index) {
      case SET_HOUR:
        timer_hour == 23 ? (timer_hour = 0) : timer_hour++;
        break;
      case SET_MINUTE:
        timer_minute == 59 ? (timer_minute = 0) : timer_minute++;
        break;
      default:
        break;
    }
  }

  if (is_up_button_pressed()) {
    digit_blink_state = 1;
    switch (set_index) {
      case SET_HOUR:
        timer_hour == 0 ? (timer_hour = 23) : timer_hour--;
        break;
      case SET_MINUTE:
        timer_minute == 0 ? (timer_minute = 59) : timer_minute--;
        break;
      default:
        break;
    }
  }
  return true;
}

bool handle_saved_duration_state_inputs() {
  if (is_back_button_pressed()) {
    timer_app_state = DURATION_SETUP_STATE;
  }
  else if (is_menu_button_pressed()) {
    seconds = timer_saved_settings[menu_index];
    pushToFront(timer_saved_settings, seconds);
    start_timer();
    timer_app_state = TIMER_COUNTDOWN_STATE;
  }
  else if (is_up_button_pressed()) {
    menu_index = menu_index == 0 ? TIMER_SAVED_SETTINGS_COUNT - 1 : menu_index - 1;
  }
  else if (is_down_button_pressed()) {
    menu_index = menu_index == TIMER_SAVED_SETTINGS_COUNT - 1 ? 0 : menu_index + 1;
  }
  return true;
}

void display_duration_setup() {
  display.fillRect(0, 0, DISPLAY_WIDTH, 75, GxEPD_BLACK);
  draw_time_digits(timer_hour, timer_minute, 11, digit_blink_state, set_index);
}

void display_saved_duration() {
  display.fillRect(0, 75, DISPLAY_WIDTH, 125, GxEPD_BLACK);
  draw_menu(timer_menu_items, menu_index, 75, 5);
}

void start_timer() {
  start_timestamp = rtc.getTimestamp();
}

void display_countdown_page() {
  display.fillScreen(GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(20, 65);
  display.println("Remaining time:");

  uint32_t diff_seconds = seconds - timestamp_diff;
  uint8_t minutes = diff_seconds / 60;
  uint8_t seconds = diff_seconds % 60;
  draw_time_digits(minutes, seconds, 75);
}

bool display_finished_page() {
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(15, 95);
  display.println("Timer finished!");
  display.display(true);
  motor_vibrate(200, 10);
  return false;
}

void calc_duration() {
  timestamp_diff = rtc.getTimestamp() - start_timestamp;
  if (timestamp_diff >= seconds) {
    timer_app_state = TIMER_FINISHED_STATE;
  }
}

void seconds_to_string(uint32_t seconds, char* mmss_string) {
  uint32_t minutes = seconds / 60;
  sprintf(mmss_string, "%02d:%02d", minutes, seconds % 60);
}

void pushToFront(uint32_t array[], uint32_t new_value) {
    // Shift all elements to the right by one position
    for (int i = TIMER_SAVED_SETTINGS_COUNT - 1; i > 0; i--) {
        array[i] = array[i - 1];
    }
    // Place the new value at the front
    array[0] = new_value;
}
