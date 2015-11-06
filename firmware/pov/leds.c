#include "leds.h"
#include <util/delay.h>

volatile uint8_t g_index = 0;

/**
 * Here we set the direction for the various pins
 */
void init_led_system() {
    logme("--> init_led_system()");
    unsigned int idx = 0;

    for (idx = 0 ; idx < sizeof(leds) ; idx++) {
        struct led_t led = leds[idx];
        *led.ddm |= _BV(led.dd);
    }
    logme("<-- init_led_system()\n");
}

void set_led(unsigned short idx, enum led_state_t state) {
    struct led_t led = leds[idx];

    switch(state) {
        case ON:
            *led.portm |= _BV(led.port);
            break;
        case OFF:
            *led.portm &= ~_BV(led.port);
            break;
    }
}

void set_led_on (unsigned int idx) {
    set_led(idx, ON);
}

void set_led_off(unsigned int idx) {
    set_led(idx, OFF);
}

/**
 * Simply loop over all the leds
 */
void led_self_test() {
    logme("--> led_self_test()");
    unsigned int idx;

    unsigned int size = sizeof(leds)/sizeof(struct led_t);

    for (idx = 0 ; idx < size ; idx++) {
        set_led_on(idx);
        delay(DELAY_QUICK_VALUE);
    }

    for (idx = 0 ; idx < sizeof(leds)/sizeof(struct led_t) ; idx++) {
        set_led_off(idx);
        delay(DELAY_QUICK_VALUE);
    }
}

void display_bit_pattern(uint8_t digit, uint8_t size) {
    int index;

    for (index = 0 ; index < size ; index++) {
        set_led(index, GET_STATE(digit, index));
    }
}


void display_next() {
    // display the row
    display_bit_pattern(pattern[g_index++ % sizeof(pattern)], 8);
}
