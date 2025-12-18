#include "write_ops.h"
#include "device_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

//this is a easter egg :)

int write_zeros_to_device(const char *device_path, int update_interval, int input_buf) {
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
    
    // Allocate buffer on heap to avoid stack overflow
    char *buffer = malloc(input_buf);
    if (!buffer) {
        perror("malloc buffer");
        fclose(fp);
        return -1;
    }
    
    setvbuf(fp, NULL, _IOFBF, input_buf);
    memset(buffer, 0, input_buf);
    unsigned long long total_blocks = device_size / input_buf;
    unsigned long long remaining_bytes = device_size % input_buf;
    unsigned long long interval = (total_blocks > 0) ? total_blocks / update_interval : 1;
    if (interval == 0) interval = 1;
    
    printf("Writing zeros to device, this may take a while.\n");
    
    // Write full blocks
    for (unsigned long long i = 0; i < total_blocks; i++) {
        size_t written = fwrite(buffer, input_buf, 1, fp);
        if (written != 1) {
            printf("Write error at block %llu\n", i);
            free(buffer);
            fclose(fp);
            return -1;
        }
        
        if (i % interval == 0) {
            double percent = (double)i / total_blocks * 100.0;
            printf("Progress: %.2f%%\n", percent);
            fflush(stdout);
        }
    }
    
    // Write remaining bytes
    if (remaining_bytes > 0) {
        size_t written = fwrite(buffer, remaining_bytes, 1, fp);
        if (written != 1) {
            printf("Write error at final block\n");
            free(buffer);
            fclose(fp);
            return -1;
        }
    }
    
    // Ensure all data is flushed to device
    if (fsync(fileno(fp)) < 0) {
        perror("fsync");
    }
    
    free(buffer);
    fclose(fp);
    printf("Progress: 100.00%%\n");
    printf("Writing complete.\n");
    return 0;
}
