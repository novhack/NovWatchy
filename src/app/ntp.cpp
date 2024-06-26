#include "ntp.h"
#include <NTPClient.h>
#include "hardware/hardware.h"
#include "hardware/wifi.h"
#include <Fonts/FreeMonoBold9pt7b.h>
#include "gui/menu.h"
#include "gui/gui.h"
#include "settings.h"
#include "hardware/rtc_sram.h"

void showSyncNTP() {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(0, 30);
  display.println("Syncing NTP... ");
  display.print("GMT offset: ");
  display.println(gmtOffset);
  display.display(false); // full refresh
  if (connectWiFi()) {
    if (syncNTP()) {
      display.println("NTP Sync Success\n");
      display.println("Current Time Is:");

      rtc.read(current_time);

      display.print(tmYearToCalendar(current_time.Year));
      display.print("/");
      display.print(current_time.Month);
      display.print("/");
      display.print(current_time.Day);
      display.print(" - ");

      if (current_time.Hour < 10) {
        display.print("0");
      }
      display.print(current_time.Hour);
      display.print(":");
      if (current_time.Minute < 10) {
        display.print("0");
      }
      display.println(current_time.Minute);
    } else {
      display.println("NTP Sync Failed");
    }
  } else {
    display.println("WiFi Not Configured");
  }
  display.display(true); // full refresh
  delay(3000);
  show_menu(menuIndex, false);
}

bool syncNTP() { // NTP sync - call after connecting to WiFi and
                         // remember to turn it back off
  return syncNTP(gmtOffset, settings.ntpServer.c_str());
}

bool syncNTP(long gmt) {
  return syncNTP(gmt, settings.ntpServer.c_str());
}

bool syncNTP(long gmt, String ntpServer) {
  // NTP sync - call after connecting to
  // WiFi and remember to turn it back off
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, ntpServer.c_str(), gmt);
  timeClient.begin();
  if (!timeClient.forceUpdate()) {
    return false; // NTP sync failed
  }
  tmElements_t tm;
  breakTime((time_t)timeClient.getEpochTime(), tm);
  rtc.set(tm);
  return true;
}
