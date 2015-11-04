CPPFLAGS += -g -Wall -Wpedantic

BIN += analyze
BIN += min_max

all: $(BIN)

min_max: min_max.o ../firmware/pov/timing.c

clean:
	rm -f *.o $(BIN)
