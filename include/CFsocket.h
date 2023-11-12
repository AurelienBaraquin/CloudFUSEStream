#pragma once

#include <sys/types.h>

int CFStream_host(int port);
int CFStream_connect(const char *host, int port);
int CFStream_send(int fd, const char *request, size_t len);
char *CFStream_recv(int fd);
int CFStream_close(int fd);

#include <pthread.h>

#define MAX_SOCKETS_CLIENT 3

typedef struct {
    int socket_fd[MAX_SOCKETS_CLIENT];
    pthread_mutex_t socket_mutex[MAX_SOCKETS_CLIENT];
    pthread_mutex_t lock_fd[MAX_SOCKETS_CLIENT];
    int nb_clients_in_queue[MAX_SOCKETS_CLIENT];
} SPoolClient;

void SPClient_init(char *ip, int port);
int SPClient_get(void);
void SPClient_release(int index);
