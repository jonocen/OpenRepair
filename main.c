#include <stdio.h>
#include <sys/vfs.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Parse command line arguments
    int update_percent = 10;  // Default to 10%
    int opt;
    
    while ((opt = getopt(argc, argv, "u:")) != -1) {
        switch (opt) {
            case 'u':
                update_percent = atoi(optarg);
                if (update_percent <= 0 || update_percent > 100) {
                    printf("Invalid update percentage. Using default 10%%\n");
                    update_percent = 100;
                }
                break;
            case 'h':
                printf("A Basic tool for repairing Unix Like systems and to prepare drives.\n", argv[0]);
                printf("Usage: %s [-u is for setting the update interval. (basic interval is 100)]\n", argv[0]);
                return 0;
            default:
                printf("Usage: %s [-u update_percentage]\n", argv[0]);
                return 1;
        }
    }
    
    // Ask user if they want to format a partition
    printf("Do you want to format a partition? (y/n): ");
    char response;
    scanf(" %c", &response);
    
    if (response == 'y') {
        // Define structure to hold partition menu data
        struct menu_struct {
            char names[20][256];  // Array to store partition names
            double space[20];      // Array to store space info (unused currently)
        };
        
        struct statfs filesystem;
        struct menu_struct menu;
        
        // Open /proc/partitions to read available partitions
        FILE *partitions = fopen("/proc/partitions", "r");
        if (!partitions) {
            perror("fopen");
            return 1;
        }
        
        // Parse partition list from /proc/partitions
        char lines[256];
        int count = 0;
        while (fgets(lines, sizeof(lines), partitions) && count < 20) {
            char name[256];
            // Extract device name from each line (skip major, minor, blocks)
            if (sscanf(lines, "%*d %*d %*lu %255s", name) == 1) {
                strcpy(menu.names[count], name);
                count++;
            }
        }
        fclose(partitions);
        
        // Display menu of available partitions
        for (int i = 0; i < count; i++) {
            printf("%d %s\n", i, menu.names[i]);
        }
        
        // Get user's partition choice
        printf("please select a partition by number: ");
        int choice;
        scanf("%i", &choice);
        
        // Validate choice (note: this only checks if choice is 0, not bounds)
        if (!choice) {
            perror("Invalid choice");
            return 1;
        }
        
        // Build full device path
        char name[512];
        snprintf(name, sizeof(name), "/dev/%s", menu.names[choice]);
        
        // Try to open the device for writing
        FILE *to_format = fopen(name, "wb");
        if (!to_format) {
            perror("fopen");
            return 1;
        } else {
            printf("You have selected partition: %s\n", menu.names[choice]);
        }
        
        // Final confirmation before formatting
        printf("are you sure you want to format this drive?, All data will be lost! (y/n): ");
        char confirm;
        scanf(" %c", &confirm);
        
        if (confirm != 'y' && confirm != 'Y') {
            printf("Format cancelled.\n");
            return 0;
        }
        
        // Prepare buffer and get device size
        char bs[4096];
        
        // Get actual device size in bytes
        int fd = open(name, O_RDONLY);
        unsigned long long device_size_bytes = 0;
        if (fd >= 0) {
            ioctl(fd, BLKGETSIZE64, &device_size_bytes);
            close(fd);
        }
        unsigned long blocks = device_size_bytes / sizeof(bs);
        memset(bs, 0, sizeof(bs));  // Fill buffer with zeros
        // Write zeros to device
        unsigned long interval = blocks / update_percent;
        printf("Writing zeros to device, this may take a while.\n");
        
        for (unsigned long i = 0; i < blocks; i++) {
            size_t written = fwrite(bs, sizeof(bs), 1, to_format);
            if (written != 1) {
                printf("Write error at block %lu\n", i);
                printf("do you still want to continue? (y/n): ");
                response = '\0';
                scanf(" %c", &response);
                if (response != 'y' && response != 'Y') {
                    printf("Aborting write operation.\n");
                    break;
                }
                
            }
            if (i % interval == 0) {
                double percent = (double)i / blocks * 100.0;
                printf("Progress: %.2f%%\n", percent);
                fflush(stdout);  // Force output to display immediately
            }
        }
        printf("Writing complete.\n");
    } else {
        return 0;
    }
    return 0;
}
int repair_debian_kernel(){
    //being implemented
    return 0;
}
