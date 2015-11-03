#ifndef __DUMP_H__
#define __DUMP_H__

#define EEPROM_SIZE 1024 // for the ATMega32U4
#define EEPROM_BUF_SIZE (EEPROM_SIZE - 2) // for the ATMega32U4

/**
 * Register in the EEPROM the accelerometer data.
 */
struct _acc_history {
    int8_t min;
    int8_t max;
    //uint16_t idx;
    int8_t buf[EEPROM_BUF_SIZE];
} acc_history;

#endif
