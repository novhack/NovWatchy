#include "gui.h"
#include "face.h"
#include "menu.h"
#include "hardware/hardware.h"
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

void showMenu(byte menuIndex, bool partialRefresh) {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);

  const char *menuItems[] = {
    "Timer",
    "About",
    "Vibrate Motor",
    "Show Accelerometer",
    "Set Time",
    "Setup WiFi",
    "Update Firmware",
    "Sync NTP"
  };

  draw_menu(menuItems, 7, menuIndex);
  
  display.display(partialRefresh);
  guiState = MAIN_MENU_STATE;
  alreadyInMenu = false;
}

void showFastMenu(byte menuIndex) {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);

  int16_t x1, y1;
  uint16_t w, h;
  int16_t yPos;

  const char *menuItems[] = {
    "Timer",
    "About",
    "Vibrate Motor",
    "Show Accelerometer",
    "Set Time",
    "Setup WiFi",
    "Update Firmware",
    "Sync NTP"
  };
  
  draw_menu(menuItems, 7, menuIndex);

  display.display(true);

  guiState = MAIN_MENU_STATE;
}


void showWatchFace(bool partialRefresh) {
  display.setFullWindow();
  // At this point it is sure we are going to update
  display.epd2.asyncPowerOn();
  drawWatchFace();
  display.display(partialRefresh); // partial refresh
  guiState = WATCHFACE_STATE;
}

/*
void defaultDrawWatchFace() {
  display.setFont(&DSEG7_Classic_Bold_53);
  display.setCursor(5, 53 + 60);
  if (currentTime.Hour < 10) {
    display.print("0");
  }
  display.print(currentTime.Hour);
  display.print(":");
  if (currentTime.Minute < 10) {
    display.print("0");
  }
  display.println(currentTime.Minute);
}
*/
