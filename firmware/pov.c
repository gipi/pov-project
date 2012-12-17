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
#include <shift_register/sr.h>

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
    s _ _ X X X X _ _,
    s _ X _ _ _ _ X _,
    s X _ _ _ _ _ _ X,
    s X _ _ _ _ _ _ X,
    s X X X X X X X X,
    s X _ _ _ _ _ _ X,
    s X _ _ _ _ _ _ X,
    s X _ _ _ _ _ _ X,

};

#undef X
#undef _
#undef s

#define DELAY_VALUE 1000

// http://bildr.org/2011/02/74hc595/
int SER_Pin = 2;   //pin 14 on the 75HC595
int RCLK_Pin = 1;  //pin 12 on the 75HC595
int SRCLK_Pin = 0; //pin 11 on the 75HC595

void display_bit_pattern(uint8_t digit, uint8_t size) {
        clearRegisters();
        int index;
        for (index = 0 ; index < size ; index++) {
            setRegisterPin(index, (digit & (1 << index)) > 0 ? HIGH : LOW);
        }
        writeRegisters();
}

int main() {
    // change to 8MHz
    clock_prescale_set(clock_div_1);
    sr_init(SER_Pin, RCLK_Pin, SRCLK_Pin);

    unsigned int index = 0;
    unsigned int size = sizeof(pattern);

    while (1) {
        if (index > size)
            index = 0;

        // display the row
        display_bit_pattern(pattern[index++], 8);

        _delay_ms(DELAY_VALUE);
    }
    return 0;
}
