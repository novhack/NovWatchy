#include "menu_items.h"

#include "app/about.h"
#include "app/accelerometer.h"
#include "app/buzz.h"
#include "app/ntp.h"
#include "app/set_time.h"
#include "app/timer.h"
#include "app/update.h"
#include "app/weather.h"
#include "hardware/wifi.h"

menu_item_t menu_items[] = {
    {"Timer", timer_app_main, false, false},
    {"About", about_app_main, false, false},
    {"Vibrate Motor", showBuzz, true, false},
    {"Show Accelerometer", accelerometer_app_main, true, true},
    {"Set Time", set_time_app_main, true, true},
    {"Setup WiFi", setupWifi, false, false},
    {"Update Firmware", showUpdateFW, false, false},
    {"Sync NTP", showSyncNTP, false, false}
};