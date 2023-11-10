#include "CFsocket.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "CFrequest.h"
#include <string.h>
#include "Svcallback.h"

int CFrequest_handler(CFreq *req, int fd)
{
    if (req->nsections == 0 || CFreq_find_section(req, 0) == -1)
        return -1;
    
    char *call_id = req->sections[CFreq_find_section(req, 0)].data;

    if (strcmp(call_id, "getattr") == 0)
        Sv_CFuse_getattr(req, fd);

    return 0;
}

void *CFSTREAM_handle(int fd)
{
    CFreq *req = NULL;
    while (1) {
        req = CFreq_recv(fd);
        if (!req)
            break;

        if (CFrequest_handler(req, fd) == -1) {
            // TODO : send error
            CFreq_free(req);
            continue;
        }

        CFreq_free(req);
    }

    printf("Connection closed : %d\n", fd);
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
        printf("New connection : %d\n", client_fd);
        pthread_t thread;
        pthread_create(&thread, NULL, (void *(*)(void *))CFSTREAM_handle, (void *)(long)client_fd);
        pthread_detach(thread);
    }
    return 0;
}
