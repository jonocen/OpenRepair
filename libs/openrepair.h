#ifndef OPENREPAIR_H
#define OPENREPAIR_H

#include <stdio.h>

typedef struct {
    char names[20][256];
    int count;
} PartitionList;

/**
 * Get list of available partitions from /proc/partitions
 * Returns a PartitionList with device names and count
 */
PartitionList get_partitions(void);

/**
 * Get the device size in bytes using ioctl BLKGETSIZE64
 * Returns the size in bytes, or 0 on error
 */
unsigned long long get_device_size(const char *device_path);

/**
 * Write zeros to a device
 * update_interval: how often to print progress (e.g., 10 = print every 10%)
 * Returns 0 on success, -1 on error
 */
int write_zeros_to_device(const char *device_path, int update_interval, int input_buf);

/**
 * Format a device interactively (prompts user)
 */
void format_device_interactive(int update_percent, int buffer_size);

#endif
