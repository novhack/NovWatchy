#include "timer.h"

#include <Arduino.h>

#include "config.h"
#include "gui/fonts/DSEG7_Classic_Bold_53.h"
#include <Fonts/FreeMonoBold9pt7b.h>
#include "hardware/button.h"
#include "hardware/hardware.h"
#include "hardware/motor.h"
#include "hardware/rtc_sram.h"

void timer_app_main() {
  display.setFullWindow();

  set_buttons_pins_as_inputs();

  int8_t set_index = SET_HOUR;
  int8_t blink = 0;

  while(1) {
    // Process inputs
    if (is_back_button_pressed()) {
      if (set_index != SET_HOUR) {
        set_index--;
      } else {
        break;
      }
    }

    if (is_menu_button_pressed()) {
      if (set_index == SET_HOUR) {
        set_index++;
      } else {
        // Start timer and show countdown screen
        timer_start((timer_hour * 60 + timer_minute) * 60);
        // After timer finished break the app loop
        break;
      }
    }

    blink = 1 - blink;

    if (is_down_button_pressed()) {
      blink = 1;
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
      blink = 1;
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

    // Display selection screen
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&DSEG7_Classic_Bold_53);

    display.setCursor(5, 80);
    if (set_index == SET_HOUR) { // blink hour digits
      display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
    }
    if (timer_hour < 10) {
      display.print("0");
    }
    display.print(timer_hour);

    display.setTextColor(GxEPD_WHITE);
    display.print(":");

    display.setCursor(108, 80);
    if (set_index == SET_MINUTE) { // blink minute digits
      display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
    }
    if (timer_minute < 10) {
      display.print("0");
    }
    display.print(timer_minute);

    display.display(true); // partial refresh
  }
}

void timer_start(uint32_t seconds) {
  display.setFullWindow();

  unsigned long start_timestamp = rtc.getTimestamp();

  while(1) {
    if (are_top_buttons_pressed()) {
      break;
    }
    unsigned long timestamp_diff = rtc.getTimestamp() - start_timestamp;

    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(5, 80);
    display.println("Remaining time: ");

    // Maximum amount of minutes is 1440 (HH:mm input format)
    char mmss_string[8] = "0000:00";
    seconds_to_string(uint32_t(seconds - timestamp_diff), mmss_string);
    display.println(mmss_string);

    if (timestamp_diff >= seconds) {
      display.fillScreen(GxEPD_BLACK);
      display.println("Timer finished!");
      display.display(true);
      motor_vibrate(200, 10);
      break;
    }
    display.display(true);
    delay(500);
  }
}

void seconds_to_string(uint32_t seconds, char* mmss_string) {
  uint32_t minutes = seconds / 60;
  sprintf(mmss_string, "%02d:%02d", minutes, seconds % 60);
}
