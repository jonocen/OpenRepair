#include "partition_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//lib for getting partition list and find the device names
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
        if (sscanf(line, "%*d %*d %*d %255s", name) == 1) {
            strncpy(partitions.names[partitions.count], name, 256);
            partitions.names[partitions.count][255] = '\0';
            partitions.count++;
        }
    }
    fclose(fp);
    
    return partitions;
}
