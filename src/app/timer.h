#pragma once

#include <GxEPD2_BW.h>
#include "config.h"

enum timer_gui_state_t {
  DURATION_SETUP_STATE,
  SAVED_DURATIONS_STATE,
  TIMER_COUNTDOWN_STATE,
  TIMER_FINISHED_STATE,
};

typedef struct {
  int8_t set_index;
  int8_t digit_blink_state;
  int8_t menu_index;
  timer_gui_state_t timer_gui_state;
  unsigned long start_timestamp;
  unsigned long timestamp_diff;
  uint32_t seconds;
} TimerState;

extern TimerState state;

/**
 * Main app function - enter app loop
*/
void timer_app_main();

/**
 * Handle user inputs in this app loop
 */
bool process_inputs();

void seconds_to_string(uint32_t seconds, char* mmss_string);

/**
 * Displays timer countdown
*/
void timer_start(uint32_t seconds);

bool handle_saved_duration_state_inputs();

bool handle_duration_setup_inputs();

void display_countdown_page();

void start_timer();

void calc_duration();

bool display_finished_page();

void display_duration_setup();

void display_saved_duration();

void pushToFront(uint32_t array[], uint32_t new_value);
