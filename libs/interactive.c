#include "interactive.h"
#include "partition_utils.h"
#include "device_utils.h"
#include "write_ops.h"
#include <stdio.h>
#include <string.h>

void format_device_interactive(int update_percent, int buffer_size) {
    // Validate buffer size
    if (buffer_size <= 0 || buffer_size > 1048576) {
        printf("Invalid buffer size. Using default 4096 bytes.\n");
        buffer_size = 4096;
    }
    
    // Validate update interval
    if (update_percent <= 0 || update_percent > 100) {
        printf("Invalid update interval. Using default 100.\n");
        update_percent = 100;
    }
    
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
    write_zeros_to_device(device_path, update_percent, buffer_size);


    //disclamer: i added all this comments for clarity, you can remove them later
    //Lg. Jonocen
    printf("Formatting completed successfully.\n");
}
