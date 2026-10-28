// Copyright 2025 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    SPAM_SPC = SAFE_RANGE,
};

static bool space_repeat_active = false;
static uint16_t space_repeat_timer = 0;

static bool space_is_down = false;
static uint16_t space_down_timer = 0;

#define SPACE_INTERVAL 80
#define SPACE_HOLD_TIME 20

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SPAM_SPC:
            if (record->event.pressed) {
                // Start repeating
                space_repeat_active = true;
                space_repeat_timer = timer_read();

                // Press Space immediately
                register_code(KC_SPC);
                space_is_down = true;
                space_down_timer = timer_read();
            } else {
                // Stop repeating
                space_repeat_active = false;

                // Ensure Space is released
                if (space_is_down) {
                    unregister_code(KC_SPC);
                    space_is_down = false;
                }
            }
            return false;
    }

    return true;
}

void matrix_scan_user(void) {
    if (!space_repeat_active) {
        return;
    }

    uint16_t now = timer_read();

    // Release Space after the hold duration.
    if (space_is_down &&
        timer_elapsed(space_down_timer) >= SPACE_HOLD_TIME) {
        unregister_code(KC_SPC);
        space_is_down = false;
    }

    // Start the next Space press.
    if (!space_is_down &&
        timer_elapsed(space_repeat_timer) >= SPACE_INTERVAL) {
        register_code(KC_SPC);
        space_is_down = true;
        space_down_timer = now;
        space_repeat_timer = now;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(SPAM_SPC, KC_C, KC_V)
};
