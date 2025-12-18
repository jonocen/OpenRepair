#ifndef PARTITION_UTILS_H
#define PARTITION_UTILS_H

typedef struct {
    char names[20][256];
    int count;
} PartitionList;

/**
Info is in partition_utils.c :)
 */
PartitionList get_partitions(void);

#endif
