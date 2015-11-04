#include <stdlib.h>
#include "timing.h"



enum extreme_t is_local_extreme(int8_t old_value, int8_t new_value, enum phase_t phase) {
    if ((new_value > old_value) && phase == DECR) {
        return MIN;
    }

    if ( (new_value < old_value) && phase == INCR) {
        return MAX;
    }

    return UNKNOWN;
}


/**
 * THIS MACROS MAKE SENSE ONLY INSIDE update_state()!!!
 */
// http://www.cprogramming.com/tutorial/cpreprocessor.html
#define PHASE_IF_UNKNOWN(z, h) \
    if ((*h)->z##_phase == UNKNOWN) { \
        if ((*h)->z##_value > z##_value) {           \
            (*h)->z##_phase = DECR;                  \
        } else if ((*h)->z##_value < z##_value) {    \
            (*h)->z##_phase = INCR;                  \
        }                                         \
    } while(0)

#define UPDATE(z, h) \
    if ((*h)->z##_phase != UNKNOWN) {                   \
        enum extreme_t extreme = is_local_extreme((*h)->z##_value, z##_value, (*h)->z##_phase); \
        (*h)->z##_extreme = extreme;                    \
        if (extreme != UNDEFINED) {                  \
            (*h)->z##_phase = (extreme == MAX ? DECR : INCR); \
        }                                                  \
        (*h)->z##_value = z##_value;                          \
    } while(0)

/**
 * The first time pass a NULL pointer for history, the struct will be instantiated
 * for you.
 *
 * MAYBE: change to return 'struct state_t* h' so we avoid '(*h)->' in the MACROs.
 */
void update_state(struct state_t** h, int8_t x_value, int8_t y_value) {
    // if it's the first time then allocates the history
    // we are using the NULL as distinguer to avoid convoluted logic
    // or to save extra data
    if (*h == NULL) {
        struct state_t* tmp_h = malloc(sizeof(struct state_t));
        tmp_h->x_phase = UNKNOWN;
        tmp_h->y_phase = UNKNOWN;
        tmp_h->x_value = x_value;
        tmp_h->y_value = y_value;

        *h = tmp_h;

        return;
    }

    // this is the 2nd call, we set phase DECR or INCR
    PHASE_IF_UNKNOWN(x, h);
    PHASE_IF_UNKNOWN(y, h);

    // here the normal flow
    UPDATE(x, h);
    UPDATE(y, h);
}
