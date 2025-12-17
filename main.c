#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "openrepair.h"
//CLI
int main(int argc, char *argv[]) {
    int update_interval = 100;
    int opt;
    while ((opt = getopt(argc, argv, "u:h")) != -1) {
        switch (opt) {
            case 'u':
                update_interval = atoi(optarg);
                if (update_interval <= 0 || update_interval > 100) {
                    printf("Invalid update percentage. Using default 100%%\n");
                    update_interval = 100;
                }
                break;
            case 'h':
                printf("OpenRepair - A tool for repairing Unix-Like systems and preparing drives\n");
                printf("Usage: %s [-u update_interval] [-h]\n", argv[0]);
                printf("Options:\n");
                printf("  -u <percent>  Set progress update interval (default: 100)\n");
                printf("  -h            Show this help message\n");
                return 0;
            default:
                printf("Usage: %s [-u update_percentage] [-h]\n", argv[0]);
                return 1;
        }
    }
    printf("Do you want to format a partition? (y/n): ");
    char response;
    scanf(" %c", &response);
    
    if (response == 'y' || response == 'Y') {
        format_device_interactive(update_interval);
    }
    
    return 0;
}
