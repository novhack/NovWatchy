#pragma once

// Versioning
#define FIRMWARE_NAME "NovWatchy"
#define WATCHY_LIB_VER "0.1.0"

// Watch 20
#define UP_BTN_PIN 35
#define BATT_ADC_PIN 34
#define UP_BTN_MASK  GPIO_SEL_35
#define RTC_TYPE 2 //PCF8563

//display
#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 200

// wifi
#define WIFI_AP_TIMEOUT 60
#define WIFI_AP_SSID    "Watchy AP"

// menu
#define MENU_HEIGHT     25
#define MENU_LENGTH     7

// set time
#define SET_HOUR   0
#define SET_MINUTE 1
#define SET_YEAR   2
#define SET_MONTH  3
#define SET_DAY    4
#define HOUR_12_24 24

// BLE OTA
#define BLE_DEVICE_NAME        "Watchy BLE OTA"
#define WATCHFACE_NAME         "Watchy 7 Segment"
#define SOFTWARE_VERSION_MAJOR 1
#define SOFTWARE_VERSION_MINOR 0
#define SOFTWARE_VERSION_PATCH 0
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0
