#include "timer.h"

#include <Arduino.h>

#include "config.h"
#include "gui/fonts/DSEG7_Classic_Bold_53.h"
#include <Fonts/FreeMonoBold9pt7b.h>
#include "hardware/button.h"
#include "hardware/hardware.h"

void timer_app_main() {
  display.setFullWindow();

  set_buttons_pins_as_inputs();

  int8_t hour = 0;
  int8_t minute = 0;

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
        timer_start((hour * 60 + minute) * 60);
        // After timer finished break the app loop
        break;
      }
    }

    blink = 1 - blink;

    if (is_down_button_pressed()) {
      blink = 1;
      switch (set_index) {
        case SET_HOUR:
          hour == 23 ? (hour = 0) : hour++;
          break;
        case SET_MINUTE:
          minute == 59 ? (minute = 0) : minute++;
          break;
        default:
          break;
      }
    }

    if (is_up_button_pressed()) {
      blink = 1;
      switch (set_index) {
        case SET_HOUR:
          hour == 0 ? (hour = 23) : hour--;
          break;
        case SET_MINUTE:
          minute == 0 ? (minute = 59) : minute--;
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
    if (hour < 10) {
      display.print("0");
    }
    display.print(hour);

    display.setTextColor(GxEPD_WHITE);
    display.print(":");

    display.setCursor(108, 80);
    if (set_index == SET_MINUTE) { // blink minute digits
      display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
    }
    if (minute < 10) {
      display.print("0");
    }
    display.print(minute);

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
    display.println("Show timer: ");
    display.println(seconds);
    display.println(timestamp_diff);

    if (timestamp_diff >= seconds) {
      display.println("Timer finished!");
      pinMode(VIB_MOTOR_PIN, OUTPUT);
      bool motorOn = false;
      for (int i = 0; i < 20; i++) {
        motorOn = !motorOn;
        digitalWrite(VIB_MOTOR_PIN, motorOn);
        if (is_back_button_pressed()) {
          break;
        }
        delay(100);
      }
      break;
    }
    display.display(true);
    delay(500);
  }
}