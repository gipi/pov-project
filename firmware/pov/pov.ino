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

#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#include "log.h"
#include "timing.h"
#include "leds.h"

// define __DUMP__ only if you want to save into the EEPROM
// the value read. Note that the EEPROM has only a finite number
// of erase/write operation so be wise with this option.
#ifdef __DUMP__
#include "dump.h"
#endif //__DUMP__

sensors_event_t event; 

// set as output all the leds

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
        _delay_ms(2);
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
    }
}
