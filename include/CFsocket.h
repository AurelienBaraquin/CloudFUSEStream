#pragma once

#include <sys/types.h>

int CFSTREAM_host(int port);
int CFSTREAM_connect(const char *host, int port);
int CFSTREAM_send(int fd, const char *request, size_t len);
char *CFSTREAM_recv(int fd);
int CFSTREAM_close(int fd);

#include <pthread.h>

#define MAX_SOCKETS_CLIENT 1

typedef struct {
    int socket_fd[MAX_SOCKETS_CLIENT];
    pthread_mutex_t socket_mutex[MAX_SOCKETS_CLIENT];
    pthread_mutex_t lock_fd[MAX_SOCKETS_CLIENT];
    int nb_clients_in_queue[MAX_SOCKETS_CLIENT];
} SPoolClient;

void SPClient_init(char *ip, int port);
int SPClient_get(void);
void SPClient_release(int index);
