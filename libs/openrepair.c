#include "openrepair.h"
#include <string.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
PartitionList get_partitions(void) {
    PartitionList partitions = {{{0}}, 0};
    
    FILE *fp = fopen("/proc/partitions", "r");
    if (!fp) {
        perror("fopen /proc/partitions");
        return partitions;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), fp) && partitions.count < 20) {
        char name[256];
        if (sscanf(line, "%*d %*d %*lu %255s", name) == 1) {
            strcpy(partitions.names[partitions.count], name);
            partitions.count++;
        }
    }
    fclose(fp);
    
    return partitions;
}

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

int write_zeros_to_device(const char *device_path, int update_interval) {
    FILE *fp = fopen(device_path, "wb");
    if (!fp) {
        perror("fopen device");
        return -1;
    }
    
    unsigned long long device_size = get_device_size(device_path);
    if (device_size == 0) {
        fclose(fp);
        return -1;
    }
    
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    unsigned long total_blocks = device_size / sizeof(buffer);
    unsigned long interval = total_blocks / update_interval;
    if (interval == 0) interval = 1;
    
    printf("Writing zeros to device, this may take a while.\n");
    
    for (unsigned long i = 0; i < total_blocks; i++) {
        size_t written = fwrite(buffer, sizeof(buffer), 1, fp);
        if (written != 1) {
            printf("Write error at block %lu\n", i);
            fclose(fp);
            return -1;
        }
        
        if (i % interval == 0) {
            double percent = (double)i / total_blocks * 100.0;
            printf("Progress: %.2f%%\n", percent);
            fflush(stdout);
        }
    }
    
    fclose(fp);
    printf("Writing complete.\n");
    return 0;
}

void format_device_interactive(int update_percent) {
    PartitionList partitions = get_partitions();
    
    // Display menu
    for (int i = 0; i < partitions.count; i++) {
        printf("%d %s\n", i, partitions.names[i]);
    }
    
    // Get user choice
    printf("Please select a partition by number: ");
    int choice;
    scanf("%i", &choice);
    
    if (choice < 0 || choice >= partitions.count) {
        printf("Invalid choice\n");
        return;
    }
    
    // Build device path
    char device_path[512];
    snprintf(device_path, sizeof(device_path), "/dev/%s", partitions.names[choice]);
    
    // Show device info
    unsigned long long size = get_device_size(device_path);
    printf("You have selected partition: %s\n", partitions.names[choice]);
    printf("Device size: %.2f GB (%llu bytes)\n", size / (1024.0 * 1024.0 * 1024.0), size);
    
    // Confirmation
    printf("Are you sure you want to format this drive? All data will be lost! (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    
    if (confirm != 'y' && confirm != 'Y') {
        printf("Format cancelled.\n");
        return;
    }
    
    // Write zeros
    write_zeros_to_device(device_path, update_percent);

}
