CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Ilibs
LDFLAGS = -lm

# Targets
.PHONY: all clean

all: openrepair.elf

# Main executable
openrepair.elf: main.o openrepair.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Object files
main.o: main.c libs/openrepair.h
	$(CC) $(CFLAGS) -c main.c

openrepair.o: libs/openrepair.c libs/openrepair.h
	$(CC) $(CFLAGS) -c libs/openrepair.c

# Library (optional)
libopenrepair.a: openrepair.o
	ar rcs $@ $^

# Clean build artifacts
clean:
	rm -f *.o openrepair.elf libopenrepair.a
