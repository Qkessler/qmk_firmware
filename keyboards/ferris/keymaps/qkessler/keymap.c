
#include "keycodes.h"
#include QMK_KEYBOARD_H

enum tap_dance_codes {
    TD_EXCL,
    TD_MINUS,
    TD_LAYERS,
};

/*
        ┌──┐                        ┌──┐
    ┌──┐│0C│┌──┐                ┌──┐│4C│┌──┐
    │0B│└──┘│0D│┌──┐        ┌──┐│4B│└──┘│4D│
┌──┐└──┘┌──┐└──┘│0E│        │4A│└──┘┌──┐└──┘┌──┐
│0A│┌──┐│1C│┌──┐└──┘        └──┘┌──┐│5C│┌──┐│4E│
└──┘│1B│└──┘│1D│┌──┐        ┌──┐│5B│└──┘│5D│└──┘
┌──┐└──┘┌──┐└──┘│1E│        │5A│└──┘┌──┐└──┘┌──┐
│1A│┌──┐│2C│┌──┐└──┘        └──┘┌──┐│6C│┌──┐│5E│
└──┘│2B│└──┘│2D│┌──┐        ┌──┐│6B│└──┘│6D│└──┘
┌──┐└──┘    └──┘│2E│        │6A│└──┘    └──┘┌──┐
│2A│            └──┘        └──┘            │6E│
└──┘                                        └──┘
              ┌──┐┌──┐    ┌──┐┌──┐
              │3A││3B│    │7A││7B│
              └──┘└──┘    └──┘└──┘
*/
;

#define LALT_E MT(MOD_LALT, KC_E)
#define LGUI_U MT(MOD_LGUI, KC_U)
#define RGUI_H MT(MOD_RGUI, KC_H)
#define R_ALT_T MT(MOD_RALT, KC_T)
#define MARK_W LALT(LSFT(KC_2))
#define MEH_SPC MT(MOD_MEH, KC_SPACE)

#define SYMBOL_LAYER 1
#define NUMBER_LAYER 2

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0]            = LAYOUT(KC_QUOT, TD(TD_MINUS), TD(TD_EXCL), KC_P, KC_Y,    KC_F, KC_G, KC_C, KC_R, KC_L,
                            KC_A, KC_O, LALT_E, LGUI_U, KC_I, KC_D, RGUI_H, R_ALT_T, KC_N, KC_S,
                            KC_SCLN, KC_Q, KC_J, KC_K, KC_X, KC_B, KC_M, KC_W, KC_V, KC_Z,
                            CTL_T(KC_ESC), LSFT_T(KC_BSPC), MEH_SPC, TD(TD_LAYERS)),
    [SYMBOL_LAYER] = LAYOUT(KC_EXLM, KC_AT, LSFT(KC_3), LSFT(KC_4), KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, KC_SCLN, KC_SLSH,
                            KC_TAB, KC_EQL, LALT(KC_RBRC), LALT(LSFT(KC_RBRC)), KC_GRAVE, KC_BSLS, LSFT(KC_LBRC), LSFT(KC_RBRC), KC_PIPE, KC_ENT,
                            KC_TILD, MARK_W, LALT(KC_LBRC), LALT(LSFT(KC_LBRC)), KC_DQUO, KC_LBRC, KC_LPRN, KC_RPRN, KC_RBRC, KC_TRNS,
                                                TO(0), LSFT_T(KC_BSPC),   MEH_SPC, TO(NUMBER_LAYER)),
    [NUMBER_LAYER] = LAYOUT(KC_ESC, KC_MPLY, KC_MPRV, KC_MNXT, LSFT(KC_MINS),    KC_MINS, KC_7, KC_8, KC_9, KC_SLSH,
                            KC_TAB, KC_EQL, KC_VOLD, KC_VOLU, KC_PLUS,    KC_EQL, KC_4, KC_5, KC_6, KC_ENT,
                            QK_BOOT, KC_PAST, KC_BRID, KC_BRIU, KC_PDOT,   KC_0, KC_1, KC_2, KC_3, KC_TRNS,
                                                TO(0), LSFT_T(KC_BSPC),   RALT_T(KC_SPC), KC_TRNS),
};

extern bool g_suspend_state;
#define GET_TAP_KC(dual_role_key) dual_role_key & 0xFF
uint16_t last_keycode  = KC_NO;
uint8_t  last_modifier = 0;

typedef struct {
    bool    is_press_action;
    uint8_t step;
} tap;

enum { SINGLE_TAP = 1, SINGLE_HOLD, DOUBLE_TAP, DOUBLE_HOLD, DOUBLE_SINGLE_TAP, MORE_TAPS };

static tap dance_state = {.is_press_action = true, .step = 0};

uint8_t td_step(tap_dance_state_t *state);
void    on_td_minus(tap_dance_state_t *state, void *user_data);
void    td_minus_finished(tap_dance_state_t *state, void *user_data);
void    td_minus_reset(tap_dance_state_t *state, void *user_data);
void    on_td_excl(tap_dance_state_t *state, void *user_data);
void    td_excl_finished(tap_dance_state_t *state, void *user_data);
void    td_excl_reset(tap_dance_state_t *state, void *user_data);
void    td_layer_toggle_finished(tap_dance_state_t *state, void *user_data);
void    td_layer_toggle_reset(tap_dance_state_t *state, void *user_data);

uint8_t td_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}

void on_td_minus(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_COMM);
        tap_code16(KC_COMM);
        tap_code16(KC_COMM);
    }
    if (state->count > 3) {
        tap_code16(KC_COMM);
    }
}

void td_minus_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = td_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_COMM);
            break;
        case SINGLE_HOLD:
            register_code16(KC_MINS);
            break;
        case DOUBLE_TAP:
            register_code16(KC_COMM);
            register_code16(KC_COMM);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_COMM);
            register_code16(KC_COMM);
    }
}

void td_minus_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_COMM);
            break;
        case SINGLE_HOLD:
            unregister_code16(KC_MINS);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_COMM);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_MINS);
            break;
    }
    dance_state.step = 0;
}

void on_td_excl(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_DOT);
        tap_code16(KC_DOT);
        tap_code16(KC_DOT);
    }
    if (state->count > 3) {
        tap_code16(KC_DOT);
    }
}

void td_excl_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = td_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            register_code16(KC_DOT);
            break;
        case SINGLE_HOLD:
            register_code16(KC_EXLM);
            break;
        case DOUBLE_TAP:
            register_code16(KC_DOT);
            register_code16(KC_DOT);
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_DOT);
            register_code16(KC_DOT);
    }
}

void td_excl_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP:
            unregister_code16(KC_DOT);
            break;
        case SINGLE_HOLD:
            unregister_code16(KC_EXLM);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_DOT);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_DOT);
            break;
    }
    dance_state.step = 0;
}

void td_layer_toggle_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = td_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP:
            layer_on(SYMBOL_LAYER);
            break;
        case SINGLE_HOLD:
            layer_on(NUMBER_LAYER);
            break;
        case DOUBLE_TAP:
            layer_on(NUMBER_LAYER);
            break;
    }
}

void td_layer_toggle_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_HOLD:
            layer_off(NUMBER_LAYER);
            break;
    }
    dance_state.step = 0;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_EXCL]      = ACTION_TAP_DANCE_FN_ADVANCED(on_td_excl, td_excl_finished, td_excl_reset),
    [TD_MINUS]      = ACTION_TAP_DANCE_FN_ADVANCED(on_td_minus, td_minus_finished, td_minus_reset),
    [TD_LAYERS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_layer_toggle_finished, td_layer_toggle_reset),
};
