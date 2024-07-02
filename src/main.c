#include "CFnet.h"
#include <stdio.h>
#include <string.h>

void print_help_message(void)
{
    printf("USAGE\n");
    printf("    ./CFStream host [port]\n");
    printf("    ./CFStream connect [ip] [port] <fuse flags> [mountpoint]\n\n");
    printf("DESCRIPTION\n");
    printf("    host        hostname to connect to\n");
    printf("    port        port to connect to\n");
    printf("    ip          ip to connect to\n");
    printf("    fuse flags  flags to pass to fuse (optional)\n");
    printf("    mountpoint  where to mount the filesystem\n\n");
    printf("FUSE FLAGS\n");
    printf("    -d          Enable debug mode\n");
    printf("    -f          Run in foreground\n");
    printf("    -s          Single-threaded\n");
    printf("    -h          Print help\n");
}

int main(int ac, char **av)
{

    if (ac < 2 || strcmp(av[1], "--help") == 0 || strcmp(av[1], "-h") == 0) {
        print_help_message();
        return 0;
    }

    if (ac < 3) {
        printf("Not enough arguments, try --help\n");
        return 84;
    }

    switch (av[1][0]) {
        case 'h':
            return server(ac - 2, &av[2]);
        case 'c':
            return client(ac - 2, &av[2]);
        default:
            printf("Invalid argument, try --help\n");
            return 84;
    }
}
