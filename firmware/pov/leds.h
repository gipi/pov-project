#ifndef __LEDS_H__
#define __LEDS_H__

#include <avr/io.h>
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

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
#if 0
    s _ _ _ X X _ _ _,
    s _ _ X X X X _ _,
    s _ X X X X X X _,
    s X X X X X X X X,
    s _ _ X X X X _ _,
    s _ _ X X X X _ _,
    s _ _ X X X X _ _,
    s _ _ X X X X _ _,
#else
    s X _ _ _ _ _ _ _,
    s _ X _ _ _ _ _ _,
    s _ _ X _ _ _ _ _,
    s _ _ _ X _ _ _ _,
    s _ _ _ _ X _ _ _,
    s _ _ _ _ _ X _ _,
    s _ _ _ _ _ _ X _,
    s _ _ _ _ _ _ _ X,
    s _ _ _ _ _ _ X _,
    s _ _ _ _ _ X _ _,
    s _ _ _ _ X _ _ _,
    s _ _ _ X _ _ _ _,
    s _ _ X _ _ _ _ _,
    s _ X _ _ _ _ _ _,
#endif
};

#undef X
#undef _
#undef s

#define DELAY_QUICK_VALUE 100
#define DELAY_VALUE 1000
#define GET_STATE(digit,index) (digit & (1 << index)) > 0 ? ON : OFF

// http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_port_pass
struct led_t {
    volatile uint8_t   *ddm;
    unsigned int short dd;
    volatile uint8_t   *portm;
    unsigned int short port;
};

const struct led_t leds[] = {
    {.ddm = &DDRC, .dd = DDC7, .portm = &PORTC, .port = PORTC7},
    {.ddm = &DDRC, .dd = DDC6, .portm = &PORTC, .port = PORTC6},
    {.ddm = &DDRB, .dd = DDB6, .portm = &PORTB, .port = PORTB6},
    {.ddm = &DDRB, .dd = DDB5, .portm = &PORTB, .port = PORTB5},
    {.ddm = &DDRB, .dd = DDB4, .portm = &PORTB, .port = PORTB4},
    {.ddm = &DDRD, .dd = DDD7, .portm = &PORTD, .port = PORTD7},
    {.ddm = &DDRD, .dd = DDD6, .portm = &PORTD, .port = PORTD6},
    {.ddm = &DDRD, .dd = DDD4, .portm = &PORTD, .port = PORTD4},
};

enum led_state_t {
    ON,
    OFF
};

void init_led_system();
void set_led(unsigned short idx, enum led_state_t state);
void set_led_on (unsigned int idx);
void set_led_off(unsigned int idx);
void led_self_test();
void display_bit_pattern(uint8_t digit, uint8_t size);
void display_next();

#ifdef __cplusplus
}
#endif

#endif
