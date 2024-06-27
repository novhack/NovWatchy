#include "rtc_sram.h"
#include "gui/gui.h"

// Initial declarations of persistant memory
// Only run after hard reset
uint8_t gui_state = WATCHFACE_STATE;
int menuIndex = 0;
int weatherIntervalCounter = -1;
long gmtOffset = 0;
bool alreadyInMenu = true;
tmElements_t bootTime = {
  .Second = 0, 
  .Minute = 0,
  .Hour = 0,
  .Wday = 0,
  .Day = 0,
  .Month = 0,
  .Year = 0,
};
uint32_t lastIPAddress = 0;
char lastSSID[30] = "";
bool WIFI_CONFIGURED = false;
bool BLE_CONFIGURED = false;
weatherData currentWeather = {
  .temperature = 0,
  .weatherConditionCode = 0,
  .isMetric = true,
  .weatherDescription = "",
  .external = false,
  .sunrise =  {
    .Second = 0, 
    .Minute = 0,
    .Hour = 0,
    .Wday = 0,
    .Day = 0,
    .Month = 0,
    .Year = 0,
  },
  .sunset =  {
    .Second = 0, 
    .Minute = 0,
    .Hour = 0,
    .Wday = 0,
    .Day = 0,
    .Month = 0,
    .Year = 0,
  },
};
