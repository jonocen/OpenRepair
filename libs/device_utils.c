#include "device_utils.h"
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

unsigned long long get_device_size(const char *device_path) {
    int fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 0;
    }
    
    unsigned long long size = 0;
    if (ioctl(fd, BLKGETSIZE64, &size) < 0) {
        perror("ioctl BLKGETSIZE64");
        close(fd);
        return 0;
    }
    
    close(fd);
    return size;
}
