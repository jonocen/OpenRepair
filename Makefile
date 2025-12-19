CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -O3 -Ilibs
LDFLAGS = -lm

prepair:
	mkdir build

# Targets
.PHONY: all clean

all: openrepair.elf

# Main executable
CLI.elf: CLI.o partition_utils.o device_utils.o write_ops.o interactive.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Object files
CLI.o: CLI.c libs/partition_utils.h libs/device_utils.h libs/write_ops.h libs/interactive.h
	$(CC) $(CFLAGS) -c CLI.c -o build/CLI.o

partition_utils.o: libs/partition_utils.c libs/partition_utils.h
	$(CC) $(CFLAGS) -c libs/partition_utils.c -o build/partition_utils.o

device_utils.o: libs/device_utils.c libs/device_utils.h
	$(CC) $(CFLAGS) -c libs/device_utils.c -o build/device_utils.o

write_ops.o: libs/write_ops.c libs/write_ops.h
	$(CC) $(CFLAGS) -c libs/write_ops.c -o build/write_ops.o

interactive.o: libs/interactive.c libs/interactive.h
	$(CC) $(CFLAGS) -c libs/interactive.c -o build/interactive.o

kernelrepair.o: libs/kernelrepair.c libs/kernelrepair.h
	$(CC) $(CFLAGS) -c libs/kernelrepair.c -o build/kernelrepair.o

# Library (optional)
libopenrepair.a: partition_utils.o device_utils.o write_ops.o interactive.o
	ar rcs $@ $^

# Clean build artifacts
clean:
	rm -f *.o openrepair.elf libopenrepair.a
