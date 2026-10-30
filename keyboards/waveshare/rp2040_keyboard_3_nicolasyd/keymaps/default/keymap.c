// Copyright 2025 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    SPAM_SPC = SAFE_RANGE,
    DECREASE_SPC_INTERVAL,
    INCREASE_SPC_INTERVAL,
};

static bool space_repeat_active = false;
static uint16_t space_repeat_timer = 0;

static bool space_is_down = false;
static uint16_t space_down_timer = 0;

#define SPACE_HOLD_TIME 15
#define SPACE_INTERVAL_DEFAULT 60
#define SPACE_INTERVAL_MIN 30
#define SPACE_INTERVAL_MAX 130
#define SPACE_INTERVAL_STEP 10

static uint16_t space_interval = SPACE_INTERVAL_DEFAULT;

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

        case DECREASE_SPC_INTERVAL:
            if (record->event.pressed) {
                if (space_interval >= SPACE_INTERVAL_MIN + SPACE_INTERVAL_STEP) {
                    space_interval -= SPACE_INTERVAL_STEP;
                } else {
                    space_interval = SPACE_INTERVAL_MIN;
                }
            }
            return false;

        case INCREASE_SPC_INTERVAL:
            if (record->event.pressed) {
                if (space_interval <= SPACE_INTERVAL_MAX - SPACE_INTERVAL_STEP) {
                    space_interval += SPACE_INTERVAL_STEP;
                } else {
                    space_interval = SPACE_INTERVAL_MAX;
                }
            }
            return false;
    }

    return true;
}

void matrix_scan_user(void) {
    if (space_interval == SPACE_INTERVAL_DEFAULT) {
        rgblight_sethsv_noeeprom(21, rgblight_get_sat(), rgblight_get_val());
    } else if (space_interval == SPACE_INTERVAL_MIN) {
        rgblight_sethsv_noeeprom(0, rgblight_get_sat(), rgblight_get_val());
    } else if (space_interval == SPACE_INTERVAL_MAX) {
        rgblight_sethsv_noeeprom(85, rgblight_get_sat(), rgblight_get_val());
    } else {
        rgblight_sethsv_noeeprom(169, rgblight_get_sat(), rgblight_get_val());
    }

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
        timer_elapsed(space_repeat_timer) >= space_interval) {
        register_code(KC_SPC);
        space_is_down = true;
        space_down_timer = now;
        space_repeat_timer = now;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(SPAM_SPC, DECREASE_SPC_INTERVAL, INCREASE_SPC_INTERVAL)
};
