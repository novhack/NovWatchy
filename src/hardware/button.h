#pragma once 

/**
 * Configures pins connected to buttons as inputs
*/
void set_buttons_pins_as_inputs();

/**
 * Returns true if back button is pressed
*/
bool is_back_button_pressed();

/**
 * Returns true if menu button is pressed
*/
bool is_menu_button_pressed();

/**
 * Returns true if up button is pressed
*/
bool is_up_button_pressed();

/**
 * Returns true if down button is pressed
*/
bool is_down_button_pressed();

bool are_top_buttons_pressed();

bool are_bottom_buttons_pressed();
