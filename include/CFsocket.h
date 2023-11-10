#pragma once

#include <sys/types.h>

int CFSTREAM_host(int port);
int CFSTREAM_connect(const char *host, int port);
int CFSTREAM_send(int fd, const char *request, size_t len);
char *CFSTREAM_recv(int fd);
int CFSTREAM_close(int fd);

// TODO : Socket Pool as SPool
/*
    Define a pool size for the server (depending of the performance of the server, ex : 20)
    Define a pool size for the client (It's not necessary to have a big pool size for the client, 3 is enough I think)
*/

#include <pthread.h>

#define MAX_SOCKETS_CLIENT 3

typedef struct {
    int socket_fd[MAX_SOCKETS_CLIENT];
    pthread_mutex_t socket_mutex[MAX_SOCKETS_CLIENT];
    char locked[MAX_SOCKETS_CLIENT];
} SPoolClient;

void SPClient_init(char *ip, int port);
int SPClient_get(void);
void SPClient_release(int index);
