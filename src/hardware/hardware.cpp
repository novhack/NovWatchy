#include "hardware.h"
#include <Wire.h>
#include "settings.h"
#include "motor.h"
#include "rtc_sram.h"
#include "bma.h"

// Default declarations run at first boot
GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display(WatchyDisplay{});
WatchyRTC rtc = WatchyRTC();
tmElements_t current_time = {
  .Second = 0, .Minute = 0, .Hour = 0, .Wday = 0, .Day = 0, .Month = 0, .Year = 0
};
esp_sleep_wakeup_cause_t wake_up_reason = ESP_SLEEP_WAKEUP_UNDEFINED;
uint64_t wake_up_button_bit = 0;

void hardware_setup(String datetime) {
  wake_up_reason = esp_sleep_get_wakeup_cause();
  Wire.begin(SDA, SCL); // Init I2C
  rtc.init();

  // Init the display since it's almost guaranteed we will use it
  display.epd2.initWatchy();

  switch (wake_up_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: // RTC Alarm
      rtc.read(current_time);
      break;
    case ESP_SLEEP_WAKEUP_EXT1: // Button press
      wake_up_button_bit = esp_sleep_get_ext1_wakeup_status();
      break;
    default: // Device restart
      boot(datetime);
      break;
  }
}

void boot(String datetime) {
  rtc.config(datetime);
  bma_setup();
  gmtOffset = settings.gmtOffset;
  rtc.read(current_time);
  rtc.read(bootTime);
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
