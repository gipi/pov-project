#include "log.h"
#include <stdio.h>
#include <Arduino.h>

/*
 * THIS FILE MUST BE COMPILED BY C++ OTHERWISE
 * Arduino.h DOES NOT INCLUDE Serial1
 */

static long baud = 57600;

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

