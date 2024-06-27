#include "state.h"
#include "hardware/rtc_sram.h"

uint8_t get_gui_state() {
    return gui_state;
}

void set_gui_state(uint8_t new_state) {
    gui_state = new_state;
}
