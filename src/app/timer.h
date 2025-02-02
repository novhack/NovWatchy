#pragma once

#include <GxEPD2_BW.h>
#include "config.h"

enum timer_app_state_t {
  DURATION_SETUP_STATE,
  SAVED_DURATIONS_STATE,
  TIMER_COUNTDOWN_STATE,
  TIMER_FINISHED_STATE,
};

extern timer_app_state_t timer_app_state;

extern int8_t set_index;
extern int8_t digit_blink_state;

extern int8_t menu_index;


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
