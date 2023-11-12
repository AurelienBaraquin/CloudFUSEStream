#include "CFsocket.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "CFtree.h"
#include "CFstore.h"
#include "color.h"

void *CFStream_handle(int fd);

int server(int ac, char **av)
{
    int fd = CFStream_host(atoi(av[0]));
    CFtree_init();
    CFstore_init("./store");
    if (fd == -1)
        return 1;
    
    while (1) {
        int client_fd = accept(fd, NULL, NULL);
        if (client_fd == -1) {
            perror("accept");
            return 1;
        }
        printf("%d\t-> "COLOR_GREEN"Connected\n" COLOR_RESET, client_fd);
        pthread_t thread;
        pthread_create(&thread, NULL, (void *(*)(void *))CFStream_handle, (void *)(long)client_fd);
        pthread_detach(thread);
    }
    return 0;
}
