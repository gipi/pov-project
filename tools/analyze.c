/**
 * Rebuild the measurements from a dumper EEPROM.
 */
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "./../firmware/pov/dump.h"

void usage(char* progname) {
    fprintf(stderr, "%s: <eeprom file>\n", progname);
    exit(1);
}

void dump(struct _acc_history mem) {
#if 0
    fprintf(stdout, "min: %d max: %d\n", mem.min, mem.max);
#endif

    for (unsigned int idx = 0 ; idx < EEPROM_BUF_SIZE ; idx++) {
        fprintf(stdout, "%d\n", mem.buf[idx]);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        usage(argv[0]);
    }

    char* filename = strdup(argv[1]);

    int eeprom_fd = open(filename, O_RDONLY);

    if (eeprom_fd < 0) {
        perror("opening file");
        exit(1);
    }

    struct _acc_history* mem = mmap(NULL, EEPROM_SIZE, PROT_READ, MAP_PRIVATE, eeprom_fd, 0);

    dump(*mem);

    munmap(mem, EEPROM_SIZE);

    return 0;
}
