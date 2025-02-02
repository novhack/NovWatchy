#include "button.h"

#include <Arduino.h>
#include <config.h>

void set_buttons_pins_as_inputs() {
    pinMode(DOWN_BTN_PIN, INPUT);
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(MENU_BTN_PIN, INPUT);
    pinMode(BACK_BTN_PIN, INPUT);
}

const int debounceDelay = 50;

unsigned long lastDebounceTimeBack = 0;
bool lastButtonStateBack = LOW;

unsigned long lastDebounceTimeMenu = 0;
bool lastButtonStateMenu = LOW;

unsigned long lastDebounceTimeUp = 0;
bool lastButtonStateUp = LOW;

unsigned long lastDebounceTimeDown = 0;
bool lastButtonStateDown = LOW;

bool debounceButton(int pin, unsigned long &lastDebounceTime, bool &lastButtonState) {
    bool currentButtonState = digitalRead(pin);
    if (currentButtonState != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (currentButtonState == HIGH) {
            lastButtonState = currentButtonState;
            return true;
        }
    }

    lastButtonState = currentButtonState;
    return false;
}

bool is_back_button_pressed() {
    return debounceButton(BACK_BTN_PIN, lastDebounceTimeBack, lastButtonStateBack);
}

bool is_menu_button_pressed() {
    return debounceButton(MENU_BTN_PIN, lastDebounceTimeMenu, lastButtonStateMenu);
}

bool is_up_button_pressed() {
    return debounceButton(UP_BTN_PIN, lastDebounceTimeUp, lastButtonStateUp);
}

bool is_down_button_pressed() {
    return debounceButton(DOWN_BTN_PIN, lastDebounceTimeDown, lastButtonStateDown);
}

bool are_top_buttons_pressed() {
    return is_back_button_pressed() && is_up_button_pressed();
}

bool are_bottom_buttons_pressed() {
    return is_menu_button_pressed() && is_down_button_pressed();
}
