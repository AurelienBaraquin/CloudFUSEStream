#include "CFsocket.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "CFrequest.h"

void *CFSTREAM_handle(int fd)
{
    while (1) {
        CFreq *req = CFreq_recv(fd);
        if (!req)
            break;

        //TODO : Handle request
    }

    CFSTREAM_close(fd);
    return NULL;
}

int server(int ac, char **av)
{
    int fd = CFSTREAM_host(atoi(av[0]));
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
