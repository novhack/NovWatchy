#include <Arduino.h>
#include <Wire.h>

#include "hardware.h"
#include "config.h"
#include "settings.h"
#include "motor.h"
#include "rtc_sram.h"
#include "bma.h"
#include "wifi.h"

#include "gui/gui.h"
#include "gui/menu.h"
#include "gui/state.h"
#include "app/about.h"
#include "app/accelerometer.h"
#include "app/buzz.h"
#include "app/ntp.h"
#include "app/set_time.h"
#include "app/timer.h"
#include "app/update.h"
#include "app/weather.h"

// Default declarations run at first boot
GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display(WatchyDisplay{});
WatchyRTC rtc = WatchyRTC();
tmElements_t current_time = {
  .Second = 0, .Minute = 0, .Hour = 0, .Wday = 0, .Day = 0, .Month = 0, .Year = 0
};
esp_sleep_wakeup_cause_t wake_up_reason = ESP_SLEEP_WAKEUP_UNDEFINED;

void hardware_setup(String datetime) {
  wake_up_reason = esp_sleep_get_wakeup_cause();
  Wire.begin(SDA, SCL); // Init I2C
  rtc.init();

  // Init the display since it's almost guaranteed we will use it
  display.epd2.initWatchy();

  switch (wake_up_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: // RTC Alarm
      rtc_wakeup();
      break;
    case ESP_SLEEP_WAKEUP_EXT1: // Button press
      button_wakeup();
      break;
    default: // Device restart
      boot(datetime);
      break;
  }
}

void rtc_wakeup() {
  rtc.read(current_time);
  switch (get_gui_state()) {
    case WATCHFACE_STATE:
      showWatchFace(true); // partial updates on tick
      if (settings.vibrateOClock) {
        if (current_time.Minute == 0) {
          // The RTC wakes us up once per minute
          motor_vibrate(75, 4);
        }
      }
      break;
    case MAIN_MENU_STATE:
      // Return to watchface if in menu for more than one tick
      if (alreadyInMenu) {
        set_gui_state(WATCHFACE_STATE);
        showWatchFace(false);
      } else {
        alreadyInMenu = true;
      }
      break;
  }
}

void button_wakeup() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
  int8_t gui_state = get_gui_state();

  // Menu Button
  if (wakeupBit & MENU_BTN_MASK) {
    if (gui_state == WATCHFACE_STATE) { // enter menu state if coming from watch face
      show_menu(menuIndex, true);
    } else if (gui_state == MAIN_MENU_STATE) { // if already in menu, then select menu item
      set_gui_state(APP_STATE);
      switch (menuIndex) {
        case 0:
          timer_app_main();
          show_menu(menuIndex, true);
          break;
        case 1:
          about_app_main();
          break;
        case 2:
          showBuzz();
          show_menu(menuIndex, false);
          break;
        case 3:
          accelerometer_app_main();
          show_menu(menuIndex, true);
          break;
        case 4:
          set_time_app_main();
          show_menu(menuIndex, true);
          break;
        case 5:
          setupWifi();
          break;
        case 6:
          showUpdateFW();
          break;
        case 7:
          showSyncNTP();
          break;
        default:
          break;
      }
    } else if (gui_state == FW_UPDATE_STATE) {
      updateFWBegin();
    }
  }
  // Back Button
  else if (wakeupBit & BACK_BTN_MASK) {
    if (gui_state == MAIN_MENU_STATE) { // exit to watch face if already in menu
      rtc.read(current_time);
      showWatchFace(false);
    } else if (gui_state == APP_STATE) {
      show_menu(menuIndex, true); // exit to menu if already in app
    } else if (gui_state == FW_UPDATE_STATE) {
      show_menu(menuIndex, false); // exit to menu if already in app
    } else if (gui_state == WATCHFACE_STATE) {
      return;
    }
  }
  // Up Button
  else if (wakeupBit & UP_BTN_MASK) {
    if (gui_state == MAIN_MENU_STATE) { // increment menu index
      menuIndex--;
      if (menuIndex < 0) {
        menuIndex = MENU_LENGTH - 1;
      }
      show_menu(menuIndex, true);
    } else if (gui_state == WATCHFACE_STATE) {
      return;
    }
  }
  // Down Button
  else if (wakeupBit & DOWN_BTN_MASK) {
    if (gui_state == MAIN_MENU_STATE) { // decrement menu index
      menuIndex++;
      if (menuIndex > MENU_LENGTH - 1) {
        menuIndex = 0;
      }
      show_menu(menuIndex, true);
    } else if (gui_state == WATCHFACE_STATE) {
      return;
    }
  }
}

void boot(String datetime) {
  rtc.config(datetime);
  bma_setup();
  gmtOffset = settings.gmtOffset;
  rtc.read(current_time);
  rtc.read(bootTime);
  showWatchFace(false); // full update on reset
  motor_vibrate(75, 4);
  // For some reason, seems to be enabled on first boot
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
}

float get_battery_voltage() {
    return analogReadMilliVolts(BATT_ADC_PIN) / 1000.0f * 2.0f;
}

uint8_t get_board_revision() {
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  if(chip_info.model == CHIP_ESP32) { // Revision 1.0 - 2.0
    Wire.beginTransmission(0x68); // v1.0 has DS3231
    if (Wire.endTransmission() == 0) {
      return 10;
    }
    delay(1);
    Wire.beginTransmission(0x51); // v1.5 and v2.0 have PCF8563
    if (Wire.endTransmission() == 0) {
        pinMode(35, INPUT);
        if (digitalRead(35) == 0) {
          return 20; // In v2.0, pin 35 is BTN 3 and has a pulldown
        } else {
          return 15; // In v1.5, pin 35 is the battery ADC
        }
    }
  }
  return -1;
}

void deep_sleep() {
  display.hibernate();

  // Resets the alarm flag in the RTC
  rtc.clearAlarm();

  // Set GPIOs 0-39 to input to avoid power leaking out
  const uint64_t ignore = 0b11110001000000110000100111000010;
  for (int i = 0; i < GPIO_NUM_MAX; i++) {
    // Ignore some GPIOs due to resets
    if ((ignore >> i) & 0b1) {
      continue;
    }
    pinMode(i, INPUT);
  }

  // Enable deep sleep wake on RTC interrupt
  esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_INT_PIN, 0);
  // Enable deep sleep wake on button press
  esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  esp_deep_sleep_start();
}
