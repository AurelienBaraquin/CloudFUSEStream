#include "CFsocket.h"

SPoolClient socket_pool;

void SPClient_init(char *ip, int port)
{
    for (int i = 0; i < MAX_SOCKETS_CLIENT; i++) {
        socket_pool.socket_fd[i] = CFSTREAM_connect(ip, port);
        pthread_mutex_init(&socket_pool.socket_mutex[i], NULL);
        socket_pool.locked[i] = 0;
    }
}

int SPClient_get(void)
{
    for (int i = 0; i < MAX_SOCKETS_CLIENT; i++) {
        if (!socket_pool.locked[i]) {
            socket_pool.locked[i] = 1;
            return i;
        }
    }
    return -1;
}

void SPClient_release(int index)
{
    socket_pool.locked[index] = 0;
}
