#ifndef __TIMIMG__H_
#define __TIMIMG__H_

#include <stdint.h>

enum position_t {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

/**
 * Identify if is decreasing or increasing (or unknown)
 */
enum phase_t {
    UNKNOWN,
    INCR,
    DECR
};

enum extreme_t {
    UNDEFINED,
    MIN,
    MAX
};
/**
 * This struct maintains a local memory of the measures in order
 * to be able to identify maximum and minimum and so the wheel
 * position.
 */
struct state_t {
    int8_t x_value; // this datatype MUST coincide with the one measured
    int8_t y_value; // this datatype MUST coincide with the one measured
    enum phase_t x_phase;
    enum phase_t y_phase;
    enum extreme_t x_extreme;
    enum extreme_t y_extreme;
};

void update_state(struct state_t** h, int8_t x_value, int8_t y_value);
#endif //__TIMIMG__H_
