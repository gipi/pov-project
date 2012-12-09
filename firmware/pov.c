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

int main() {
    return 0;
}
