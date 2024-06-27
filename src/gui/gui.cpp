#include "gui.h"
#include <GxEPD2_BW.h>

#include "face.h"
#include "menu.h"
#include "state.h"

#include "settings.h"
#include "hardware/hardware.h"
#include "hardware/motor.h"
#include "hardware/rtc_sram.h"
#include "hardware/wifi.h"

#include "app/about.h"
#include "app/accelerometer.h"
#include "app/buzz.h"
#include "app/ntp.h"
#include "app/set_time.h"
#include "app/timer.h"
#include "app/update.h"
#include "app/weather.h"

void gui_setup() {
  switch (wake_up_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: // RTC Alarm
      rtc_wakeup();
      break;
    case ESP_SLEEP_WAKEUP_EXT1: // Button press
      button_wakeup();
      break;
    default: // Device restart
      showWatchFace(false); // Ffull update on reset
      break;
  }
}

void rtc_wakeup() {
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
  // Menu Button
  if (wake_up_button_bit & MENU_BTN_MASK) {
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
  else if (wake_up_button_bit & BACK_BTN_MASK) {
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
  else if (wake_up_button_bit & UP_BTN_MASK) {
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
  else if (wake_up_button_bit & DOWN_BTN_MASK) {
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