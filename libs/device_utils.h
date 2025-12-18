#ifndef DEVICE_UTILS_H
#define DEVICE_UTILS_H

/**
 * Get the device size in bytes using ioctl BLKGETSIZE64
 * Returns the size in bytes, or 0 on error
 */
unsigned long long get_device_size(const char *device_path);

#endif
