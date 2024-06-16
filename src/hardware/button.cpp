#include "button.h"

#include <Arduino.h>
#include <config.h>

void set_buttons_pins_as_inputs() {
    pinMode(DOWN_BTN_PIN, INPUT);
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(MENU_BTN_PIN, INPUT);
    pinMode(BACK_BTN_PIN, INPUT);
}

bool is_back_button_pressed() {
    return digitalRead(BACK_BTN_PIN) == 1;
}

bool is_menu_button_pressed() {
    return digitalRead(MENU_BTN_PIN) == 1;
}

bool is_up_button_pressed() {
    return digitalRead(UP_BTN_PIN) == 1;
}

bool is_down_button_pressed() {
    return digitalRead(DOWN_BTN_PIN) == 1;
}

bool are_top_buttons_pressed() {
    return is_back_button_pressed() && is_up_button_pressed();
}

bool are_bottom_buttons_pressed() {
    return is_menu_button_pressed() && is_down_button_pressed();
}
