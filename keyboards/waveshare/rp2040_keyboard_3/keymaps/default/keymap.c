// Copyright 2025 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    SPAM_SPC = SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case SPAM_SPC:
        if (record->event.pressed) {
            // when keycode SPAM_SPC is pressed
            SEND_STRING("SPAM SPC pressed!");
        } else {
            // when keycode SPAM_SPC is released
        }
        break;
    }
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(SPAM_SPC, KC_C, KC_V)
};
