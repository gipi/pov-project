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
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#include "timing.h"

// define __DUMP__ only if you want to save into the EEPROM
// the value read. Note that the EEPROM has only a finite number
// of erase/write operation so be wise with this option.
#ifdef __DUMP__
#include "dump.h"
#endif //__DUMP__

static long baud = 57600;
sensors_event_t event; 

void loginit() {
    Serial1.begin(baud);
}

void logme(const char* fmt, ...) {
    va_list ap;
    char tmp[64];

    va_start(ap, fmt);
    vsnprintf(tmp, 64, fmt, ap);
    va_end(ap);

    Serial1.println(tmp);
}

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

// set as output all the leds
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

#define GET_STATE(digit,index) (digit & (1 << index)) > 0 ? ON : OFF
void display_bit_pattern(uint8_t digit, uint8_t size) {
    int index;

    for (index = 0 ; index < size ; index++) {
        set_led(index, GET_STATE(digit, index));
    }
}

volatile uint8_t g_index = 0;

void display_next() {
    // display the row
    display_bit_pattern(pattern[g_index++ % sizeof(pattern)], 8);
}

// DO NOT INITIALIZE HERE OTHERWISE FAIL
// SINCE (?) CONSTRUCTOR IS CALLED BEFORE MAIN()
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

/************* EEPROM **********************/
#ifdef __DUMP__
void dump_acc_history() {
    logme();
}
#endif

void update_acc_register() {
#ifdef __DUMP__
    // we need signed (maybe use abs())
    int8_t actual_x_value = event.acceleration.x;

    if (actual_x_value < acc_history.min ) {
        acc_history.min = actual_x_value;
    }
    if (actual_x_value > acc_history.max ) {
        acc_history.max = actual_x_value;
    }

#warning "EEPROM writing enabled"
    // save the data into the eeprom
    EEPROM.write(0, acc_history.min);
    EEPROM.write(1, acc_history.max);
    EEPROM.write(2 + acc_history.idx, actual_x_value);

    // update the index (1024 is the EEPROM size in the ATMEGA32U4)
    acc_history.idx = (acc_history.idx + 1) % (1024 - 2);
#endif //__DUMP__
}

struct state_t* state = NULL;

void calibrate_timing() {
    int8_t actual_x_value = event.acceleration.x;
    int8_t actual_y_value = event.acceleration.y;

    update_state(&state, actual_x_value, actual_y_value);
}

void timer1_ovf_callback() {
    calibrate_timing();

    logme("x=%hhd y=%hhd", state->x_value, state->y_value);

    //if (state->x_extreme != UNDEFINED || state->y_extreme != UNDEFINED)
    if (state->x_extreme != UNDEFINED) {
        set_led_on(0);
        delay(5);
        set_led_off(0);
        //display_next();
    }

    update_acc_register();
}

ISR(TIMER1_OVF_vect) {
    timer1_ovf_callback();

    TCNT1 = 0;
}

/**
 * The Timer1 is a 16bits counter.
 *
 * An overflow at 16MHz without prescaler is given by
 *
 *  16000000/65536 = 244.14Hz
 *
 * (We are not using the Timer0 counter since is used by Wire, maybe)
 */
void init_timer() {
    logme("init_timer()");

    cli();

    TCCR1A = 0;
    TCNT1 = 0;
    TCCR1B |= _BV(CS10); // without prescaler
    TCCR1B &= ~_BV(CS11);
    TCCR1B &= ~_BV(CS12);
    TIMSK1 |= _BV(TOIE1);            // enable Timer1 Overflow Interrupt

    sei();
}

void setup() {
    //clock_prescale_set(clock_div_1);
    //delay(3000);// give me time to open the serial console
    // YOU MUST HAVE THE SERIAL OPEN MOTHERFUCKER
    loginit();

    logme("src:%s", __FILE__);

    logme("setup()");

    init_led_system();

    led_self_test();

    if(!accel.begin()){
        while(1)
            logme("No ADXL345!! check your wiring please.");
    }
    /* Set the range to whatever is appropriate for your project */
    accel.setRange(ADXL345_RANGE_16_G);

    //init_timer();
}

void loop() {
    logme("loop()");
    while(1) {
        /* Get a new sensor event */ 
        accel.getEvent(&event);
        timer1_ovf_callback();
        delay(10);
    }
}
