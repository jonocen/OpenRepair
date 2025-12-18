CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -O3 -Ilibs
LDFLAGS = -lm

# Targets
.PHONY: all clean

all: openrepair.elf

# Main executable
openrepair.elf: main.o partition_utils.o device_utils.o write_ops.o interactive.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Object files
main.o: main.c libs/partition_utils.h libs/device_utils.h libs/write_ops.h libs/interactive.h
	$(CC) $(CFLAGS) -c main.c

partition_utils.o: libs/partition_utils.c libs/partition_utils.h
	$(CC) $(CFLAGS) -c libs/partition_utils.c

device_utils.o: libs/device_utils.c libs/device_utils.h
	$(CC) $(CFLAGS) -c libs/device_utils.c

write_ops.o: libs/write_ops.c libs/write_ops.h
	$(CC) $(CFLAGS) -c libs/write_ops.c

interactive.o: libs/interactive.c libs/interactive.h
	$(CC) $(CFLAGS) -c libs/interactive.c

# Library (optional)
libopenrepair.a: partition_utils.o device_utils.o write_ops.o interactive.o
	ar rcs $@ $^

# Clean build artifacts
clean:
	rm -f *.o openrepair.elf libopenrepair.a
