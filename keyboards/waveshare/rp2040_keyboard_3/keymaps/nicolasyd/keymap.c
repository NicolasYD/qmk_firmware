// Copyright 2025 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    SPAM_SPC = SAFE_RANGE,
};

static bool space_repeat_active = false;
static uint16_t space_repeat_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SPAM_SPC:
            if (record->event.pressed) {
                space_repeat_active = true;
                space_repeat_timer = timer_read();
                tap_code(KC_SPC);
            } else {
                space_repeat_active = false;
            }
            return false;
    }

    return true;
}

void matrix_scan_user(void) {
    if (space_repeat_active &&
        timer_elapsed(space_repeat_timer) >= 100) {
        space_repeat_timer = timer_read();
        tap_code(KC_SPC);
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(SPAM_SPC, KC_C, KC_V)
};
