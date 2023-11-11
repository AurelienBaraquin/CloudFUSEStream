#include "CFsocket.h"

SPoolClient pool;

void SPClient_init(char *ip, int port)
{
    for (int i = 0; i < MAX_SOCKETS_CLIENT; i++) {
        pool.socket_fd[i] = CFStream_connect(ip, port);
        pthread_mutex_init(&pool.socket_mutex[i], NULL);
        pthread_mutex_init(&pool.lock_fd[i], NULL);
        pool.nb_clients_in_queue[i] = 0;
    }
}

int SPClient_get(void)
{
    int min_queue_index = -1;
    int min_queue_size = __INT32_MAX__;

    for (int i = 0; i < MAX_SOCKETS_CLIENT; i++) {
        if (pthread_mutex_trylock(&pool.socket_mutex[i]) == 0) {
            return pool.socket_fd[i];
        }

        if (pool.nb_clients_in_queue[i] < min_queue_size) {
            min_queue_size = pool.nb_clients_in_queue[i];
            min_queue_index = i;
        }
    }

    if (min_queue_index != -1) {
        pool.nb_clients_in_queue[min_queue_index]++;
        pthread_mutex_lock(&pool.lock_fd[min_queue_index]);
        pthread_mutex_lock(&pool.socket_mutex[min_queue_index]);
        pool.nb_clients_in_queue[min_queue_index]--;
        return pool.socket_fd[min_queue_index];
    }

    return -1;
}

#include <stdio.h>

void SPClient_release(int socket_fd)
{
    int socket_index = -1;
    for (int i = 0; i < MAX_SOCKETS_CLIENT; i++) {
        if (pool.socket_fd[i] == socket_fd) {
            socket_index = i;
            break;
        }
    }

    if (socket_index == -1) {
        fprintf(stderr, "Erreur : Descripteur de fichier non trouvé dans la pool.\n");
        return;
    }

    pthread_mutex_unlock(&pool.socket_mutex[socket_index]);

    if (pool.nb_clients_in_queue[socket_index] > 0) {
        pthread_mutex_unlock(&pool.lock_fd[socket_index]);
    }
}
