#include "CFsocket.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "CFtree.h"

void *CFSTREAM_handle(int fd);

int server(int ac, char **av)
{
    int fd = CFSTREAM_host(atoi(av[0]));
    CFtree_init();
    CFtree_new_filled("/test", S_IFREG | 0644);
    if (fd == -1)
        return 1;
    
    while (1) {
        int client_fd = accept(fd, NULL, NULL);
        if (client_fd == -1) {
            perror("accept");
            return 1;
        }
        printf("New connection : %d\n", client_fd);
        pthread_t thread;
        pthread_create(&thread, NULL, (void *(*)(void *))CFSTREAM_handle, (void *)(long)client_fd);
        pthread_detach(thread);
    }
    return 0;
}
