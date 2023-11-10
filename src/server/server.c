#include "CFsocket.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

void *CFSTREAM_handle(int fd)
{
    return NULL;
}

int server(int ac, char **av)
{
    int fd = CFSTREAM_host(atoi(av[1]));
    if (fd == -1)
        return 1;
    
    while (1) {
        int client_fd = accept(fd, NULL, NULL);
        if (client_fd == -1) {
            perror("accept");
            return 1;
        }

        pthread_t thread;
        pthread_create(&thread, NULL, (void *(*)(void *))CFSTREAM_handle, (void *)(long)client_fd);
        pthread_detach(thread);
    }
    return 0;
}
