CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -O3 -Ilibs
LDFLAGS = -lm

.PHONY: all cli clean prepair

all: cli

cli: CLI.elf

prepair:
	mkdir -p build

CLI.elf: build/CLI.o build/partition_utils.o build/device_utils.o build/write_ops.o build/interactive.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/CLI.o: CLI.c libs/partition_utils.h libs/device_utils.h libs/write_ops.h libs/interactive.h | prepair
	$(CC) $(CFLAGS) -c CLI.c -o $@

build/partition_utils.o: libs/partition_utils.c libs/partition_utils.h | prepair
	$(CC) $(CFLAGS) -c libs/partition_utils.c -o $@

build/device_utils.o: libs/device_utils.c libs/device_utils.h | prepair
	$(CC) $(CFLAGS) -c libs/device_utils.c -o $@

build/write_ops.o: libs/write_ops.c libs/write_ops.h | prepair
	$(CC) $(CFLAGS) -c libs/write_ops.c -o $@

build/interactive.o: libs/interactive.c libs/interactive.h | prepair
	$(CC) $(CFLAGS) -c libs/interactive.c -o $@

# Optional library (builds only from linked objects)
libopenrepair.a: build/partition_utils.o build/device_utils.o build/write_ops.o build/interactive.o
	ar rcs $@ $^

clean:
	rm -rf build CLI.elf libopenrepair.a
