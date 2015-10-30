/*
 * Firmware of the Persistence of View (POV) circuitery.
 *
 * The IC will be an Atmega32U4.
 *
 * TODO:
 *  - load patterns by UART and save in the EEPROM
 *  - allow for more "depth" of patterns (monochrome, RGB)
 *  - animations
 */
#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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

#define DELAY_QUICK_VALUE 100
#define DELAY_VALUE 1000

// http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_port_pass
struct led_t {
    volatile uint8_t   *ddm;
    unsigned int short dd;
    volatile uint8_t   *portm;
    unsigned int short port;
};

struct led_t leds[] = {
    {.ddm = &DDRC, .dd = DDC7, .portm = &PORTC, .port = PORTC7},
    {.ddm = &DDRC, .dd = DDC6, .portm = &PORTC, .port = PORTC6},
    {.ddm = &DDRB, .dd = DDB6, .portm = &PORTB, .port = PORTB6},
    {.ddm = &DDRB, .dd = DDB5, .portm = &PORTB, .port = PORTB5},
    {.ddm = &DDRB, .dd = DDB4, .portm = &PORTB, .port = PORTB4},
    {.ddm = &DDRD, .dd = DDD7, .portm = &PORTD, .port = PORTD7},
    {.ddm = &DDRD, .dd = DDD6, .portm = &PORTD, .port = PORTD6},
    {.ddm = &DDRD, .dd = DDD4, .portm = &PORTD, .port = PORTD4},
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

/**
 * Simply loop over all the leds
 */
void led_self_test() {
    unsigned int idx;

    for (idx = 0 ; idx < sizeof(leds)/sizeof(struct led_t) ; idx++) {
        set_led_on(idx);
        _delay_ms(DELAY_QUICK_VALUE);
    }

    for (idx = 0 ; idx < sizeof(leds)/sizeof(struct led_t) ; idx++) {
        set_led_off(idx);
        _delay_ms(DELAY_QUICK_VALUE);
    }
}

#define GET_STATE(digit,index) (digit & (1 << index)) > 0 ? ON : OFF
void display_bit_pattern(uint8_t digit, uint8_t size) {
    int index;

    for (index = 0 ; index < size ; index++) {
        set_led(index, GET_STATE(digit, index));
    }
}

unsigned short g_index = 0;

void display_next() {
    // display the row
    display_bit_pattern(pattern[g_index++ % sizeof(pattern)], 8);
}

/**
 */
void init_timer() {
    TCCR0B |= _BV(CS00) | _BV(CS02); // clk/1024
    TIMSK0 |= _BV(TOIE0);            // enable Timer0 Overflow Interrupt
}

ISR(TIMER0_OVF_vect) {
    display_next();
}

int main() {
    // change to 8MHz
    //clock_prescale_set(clock_div_1);


    init_led_system();

    led_self_test();

    init_timer();
    sei();

    while(1); // needed in order to avoid crash (maybe?)

    return 0;
}
