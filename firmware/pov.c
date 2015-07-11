/*
 * Firmware of the Persistence of View (POV) circuitery.
 *
 * The IC will be an ATTINY85 connected to a shift register
 * that turn on/off the leds.
 *
 * TODO:
 *  - load patterns by UART and save in the EEPROM
 *  - allow for more "depth" of patterns (monochrome, RGB)
 *  - animations
 */
#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include <include/io.h>

/*
 * From "making a glyph from bit patterns" chapter of "Expert C
 * programming" by Peter Van Der Linden.
 *
 * The variable named "pattern" contains the bits pattern to be
 * used for our POV firmware. Using these #defines we can present
 * the programmer with a very immediate way to understand it.
 */
#define X )*2+1
#define _ )*2
#define s ((((((((0 /* For building pattern 8 bit wide */

const static unsigned char pattern[] = {
    s _ _ _ X X _ _ _,
    s _ X X _ _ X X _,
    s X _ _ _ _ _ _ X,
    s _ _ X _ _ X _ _,
    s X X X X X X X X,
    s X _ _ _ _ _ _ X,
    s X _ X _ X _ X _,
    s _ X _ X _ X _ X,
};

#undef X
#undef _
#undef s

#define DELAY_VALUE 1000

// http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_port_pass
struct led_t {
    volatile uint8_t   *ddm;
    unsigned int short dd;
    volatile uint8_t   *portm;
    unsigned int short port;
};

struct led_t leds[] = {
    {.ddm = &DDRC, .dd = DDC7, .portm = &PORTC, .port = PORTC7}
};

// set as output all the leds
void init_led_system() {
    unsigned int idx = 0;

    for (idx = 0 ; idx < sizeof(leds) ; idx++) {
        struct led_t led = leds[idx];
        *led.ddm |= _BV(led.dd);
    }
}

enum led_state_t {
    ON,
    OFF
};


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


int main() {
    // change to 8MHz
    //clock_prescale_set(clock_div_1);

    unsigned int index = 0;
    unsigned int size = sizeof(pattern);

    init_led_system();

    while (1) {
        if (index > size)
            index = 0;

        // display the row
        //display_bit_pattern(pattern[index++], 8);
        //
        set_led_on(0);
        _delay_ms(DELAY_VALUE);
        set_led_off(0);
        _delay_ms(DELAY_VALUE);
    }
    return 0;
}
