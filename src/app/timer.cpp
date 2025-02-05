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
#include "utils.h"

TimerState state = {
  SET_MINUTE, 1, 0, SAVED_DURATIONS_STATE, 0, 0, 0,
};

menu_item_t timer_menu_items[TIMER_SAVED_SETTINGS_COUNT];

void timer_app_main() {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);

  // Prevents misfiring of inputs when the app first draws
  delay(200);

  set_buttons_pins_as_inputs();

  // Reset state variables
  state.set_index = SET_MINUTE;
  state.digit_blink_state = 1;
  state.menu_index = 0;
  state.timer_gui_state = SAVED_DURATIONS_STATE;

  char mmss_strings[TIMER_SAVED_SETTINGS_COUNT][6];
  for (int i = 0; i < TIMER_SAVED_SETTINGS_COUNT; i++) {
    seconds_to_HH_mm_string(timer_state.timer_saved_settings[i], mmss_strings[i]);
    timer_menu_items[i] = { mmss_strings[i], nullptr, false, false };
  }

  TimerState old_state;
  PersistentTimerState old_timer_state;

  int loop_delay = 50;
  while(1) {
    // Handle inputs
    switch (state.timer_gui_state) {
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
        loop_delay = 100;
        break;
      case TIMER_FINISHED_STATE:
        if (!display_finished_page()) return;
        break;
    }

    // Only redraw if app state changed
    if (
      !compare_struct(&old_state, &state, sizeof(TimerState)) ||
      !compare_struct(&old_timer_state, &timer_state, sizeof(PersistentTimerState)))
    {
      display.display(true); // partial refresh
    }

    // If both top buttons are pressed exit the app
    if (are_top_buttons_pressed()) {
      return;
    }

    // Store current app state
    copy_struct(&old_state, &state, sizeof(TimerState));
    copy_struct(&old_timer_state, &timer_state, sizeof(PersistentTimerState));

    delay(loop_delay);
  }
}

bool handle_duration_setup_inputs() {
  // Process inputs
  if (is_back_button_pressed()) {
    if (state.set_index != SET_HOUR) {
      state.set_index--;
    } else {
      return false;
    }
  }

  if (is_menu_button_pressed()) {
    if (state.set_index == SET_HOUR) {
      state.set_index++;
    } else {
      // Start timer and show countdown page
      state.seconds = (timer_state.timer_hour * 60 + timer_state.timer_minute) * 60;
      pushToFront(timer_state.timer_saved_settings, state.seconds);
      start_timer();
      state.timer_gui_state = TIMER_COUNTDOWN_STATE;
    }
  }

  state.digit_blink_state = 1 - state.digit_blink_state;

  if (is_down_button_pressed()) {
    state.digit_blink_state = 1;
    switch (state.set_index) {
      case SET_HOUR:
        timer_state.timer_hour == 23 ? (timer_state.timer_hour = 0) : timer_state.timer_hour++;
        break;
      case SET_MINUTE:
        timer_state.timer_minute == 59 ? (timer_state.timer_minute = 0) : timer_state.timer_minute++;
        break;
      default:
        break;
    }
  }

  if (is_up_button_pressed()) {
    state.digit_blink_state = 1;
    switch (state.set_index) {
      case SET_HOUR:
        timer_state.timer_hour == 0 ? (timer_state.timer_hour = 23) : timer_state.timer_hour--;
        break;
      case SET_MINUTE:
        timer_state.timer_minute == 0 ? (timer_state.timer_minute = 59) : timer_state.timer_minute--;
        break;
      default:
        break;
    }
  }
  return true;
}

bool handle_saved_duration_state_inputs() {
  if (is_back_button_pressed()) {
    state.timer_gui_state = DURATION_SETUP_STATE;
  }
  else if (is_menu_button_pressed()) {
    state.seconds = timer_state.timer_saved_settings[state.menu_index];
    pushToFront(timer_state.timer_saved_settings, state.seconds);
    start_timer();
    state.timer_gui_state = TIMER_COUNTDOWN_STATE;
  }
  else if (is_up_button_pressed()) {
    state.menu_index = state.menu_index == 0 ? TIMER_SAVED_SETTINGS_COUNT - 1 : state.menu_index - 1;
  }
  else if (is_down_button_pressed()) {
    state.menu_index = state.menu_index == TIMER_SAVED_SETTINGS_COUNT - 1 ? 0 : state.menu_index + 1;
  }
  return true;
}

void display_duration_setup() {
  display.fillRect(0, 0, DISPLAY_WIDTH, 75, GxEPD_BLACK);
  draw_time_digits(timer_state.timer_hour, timer_state.timer_minute, 11, state.digit_blink_state, state.set_index);
}

void display_saved_duration() {
  display.fillRect(0, 75, DISPLAY_WIDTH, 125, GxEPD_BLACK);
  draw_menu(timer_menu_items, state.menu_index, 75, 5);
}

void start_timer() {
  state.start_timestamp = rtc.getTimestamp();
}

void display_countdown_page() {
  display.fillScreen(GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(20, 65);
  display.println("Remaining time:");

  uint32_t diff_seconds = state.seconds - state.timestamp_diff;
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
  state.timestamp_diff = rtc.getTimestamp() - state.start_timestamp;
  if (state.timestamp_diff >= state.seconds) {
    state.timer_gui_state = TIMER_FINISHED_STATE;
  }
}

void pushToFront(uint32_t array[], uint32_t new_value) {
    // Shift all elements to the right by one position
    for (int i = TIMER_SAVED_SETTINGS_COUNT - 1; i > 0; i--) {
        array[i] = array[i - 1];
    }
    // Place the new value at the front
    array[0] = new_value;
}
