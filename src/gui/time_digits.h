#pragma once

#include "hardware/hardware.h"
#include "gui/fonts/DSEG7_Classic_Bold_53.h"
#include "config.h"

#define FONT_HEIGHT 53

/**
 * Draws 52 pixels heigh 7 segment HH:mm formatted time
 */
void draw_time_digits(
  uint16_t hours,
  uint16_t minutes,
  uint8_t y_offset = 0,
  uint8_t blink_state = 1,
  uint8_t blink_index = SET_HOUR
) {
  display.setTextColor(GxEPD_WHITE);
  display.setFont(&DSEG7_Classic_Bold_53);

  display.setCursor(5, FONT_HEIGHT + y_offset);
  if (blink_index == SET_HOUR) { // blink hours digits
    display.setTextColor(blink_state ? GxEPD_WHITE : GxEPD_BLACK);
  }
  if (hours < 10) {
    display.print("0");
  }
  display.print(hours);

  display.setTextColor(GxEPD_WHITE);
  display.print(":");

  display.setCursor(108, FONT_HEIGHT + y_offset);
  if (blink_index == SET_MINUTE) { // blink minutes digits
    display.setTextColor(blink_state ? GxEPD_WHITE : GxEPD_BLACK);
  }
  if (minutes < 10) {
    display.print("0");
  }
  display.print(minutes);
}
