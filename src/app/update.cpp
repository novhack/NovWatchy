
#include "update.h"
#include "config.h"
#include "gui/gui.h"
#include "hardware/hardware.h"
#include <Fonts/FreeMonoBold9pt7b.h>
#include "hardware/ble.h"
#include "hardware/wifi.h"
#include "gui/menu.h"
#include "hardware/rtc_sram.h"

void showUpdateFW() {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(0, 30);
  display.println("Please visit");
  display.println("watchy.sqfmi.com");
  display.println("with a Bluetooth");
  display.println("enabled device");
  display.println(" ");
  display.println("Press menu button");
  display.println("again when ready");
  display.println(" ");
  display.println("Keep USB powered");
  display.display(false); // full refresh

  gui_state = FW_UPDATE_STATE;
}

void updateFWBegin() {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(0, 30);
  display.println("Bluetooth Started");
  display.println(" ");
  display.println("Watchy BLE OTA");
  display.println(" ");
  display.println("Waiting for");
  display.println("connection...");
  display.display(false); // full refresh

  BLE BT;
  BT.begin("Watchy BLE OTA");
  int prevStatus = -1;
  int currentStatus;

  while (1) {
    currentStatus = BT.updateStatus();
    if (prevStatus != currentStatus || prevStatus == 1) {
      if (currentStatus == 0) {
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(0, 30);
        display.println("BLE Connected!");
        display.println(" ");
        display.println("Waiting for");
        display.println("upload...");
        display.display(false); // full refresh
      }
      if (currentStatus == 1) {
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(0, 30);
        display.println("Downloading");
        display.println("firmware:");
        display.println(" ");
        display.print(BT.howManyBytes());
        display.println(" bytes");
        display.display(true); // partial refresh
      }
      if (currentStatus == 2) {
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(0, 30);
        display.println("Download");
        display.println("completed!");
        display.println(" ");
        display.println("Rebooting...");
        display.display(false); // full refresh

        delay(2000);
        esp_restart();
      }
      if (currentStatus == 4) {
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(0, 30);
        display.println("BLE Disconnected!");
        display.println(" ");
        display.println("exiting...");
        display.display(false); // full refresh
        delay(1000);
        break;
      }
      prevStatus = currentStatus;
    }
    delay(100);
  }

  // turn off radios
  WiFi.mode(WIFI_OFF);
  btStop();
  show_menu(menuIndex, false);
}
