
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../firmware/pov/timing.h"

void usage(char* progname) {
    fprintf(stderr, "usage: %s [-h]\n\nRead a dump from an EEPROM and log state\n", progname);
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc > 1 && !strcmp(argv[1], "-h")) {
        usage(argv[0]);
    }

    char* line = NULL;
    size_t n = 0;
    ssize_t ret = 0;
    struct state_t* state = NULL;


    // read a line (containing the '\n')
    while ((ret = getline(&line, &n, stdin)) != -1){
        // remove the '\n'
        line[strlen(line) - 1] = '\t';
        fprintf(stdout, "%s", line);
        int8_t value;
        sscanf(line, "%hhd", &value);

        update_state(&state, value, value);

        fprintf(stdout, "%0+hhd\t%d\t%d\n", state->x_value, state->x_phase, state->x_extreme);
    }
    return 0;
}
