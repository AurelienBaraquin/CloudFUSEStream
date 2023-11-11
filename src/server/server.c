#include "CFsocket.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "CFtree.h"
#include "CFstore.h"

void *CFStream_handle(int fd);

int server(int ac, char **av)
{
    int fd = CFStream_host(atoi(av[0]));
    CFtree_init();
    CFtree_new_filled("/test", __S_IFREG | 0644);
    CFtree_new_filled("/dir", __S_IFDIR | 0755);
    CFstore_init("./store");
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
        pthread_create(&thread, NULL, (void *(*)(void *))CFStream_handle, (void *)(long)client_fd);
        pthread_detach(thread);
    }
    return 0;
}
