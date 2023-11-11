#include "CFstore.h"
#include <pthread.h>

char *CFstore_getPath(const char *path);

pthread_mutex_t CFstore_mutex = PTHREAD_MUTEX_INITIALIZER;

int CFstore_CFtree_add(node_t *node, unsigned char *data)
{
    pthread_mutex_lock(&CFstore_mutex);
    if (compressBuffer(data, node->stat.st_size, &node->compressed_size) == NULL) {
        pthread_mutex_unlock(&CFstore_mutex);
        return -1;
    }
    char *path = CFstore_getPath(node->name);
    if (CFstore_write(path, data, node->compressed_size) == -1) {
        pthread_mutex_unlock(&CFstore_mutex);
        free(path);
        return -1;
    }
    pthread_mutex_unlock(&CFstore_mutex);
    free(path);
    return 0;
}

unsigned char *CFstore_CFtree_get(node_t *node)
{
    pthread_mutex_lock(&CFstore_mutex);
    unsigned char *buffer = (unsigned char *)malloc(node->compressed_size);
    if (!buffer) {
        pthread_mutex_unlock(&CFstore_mutex);
        return NULL;
    }
    char *path = CFstore_getPath(node->name);
    if (CFstore_read(path, buffer, node->compressed_size) == -1) {
        pthread_mutex_unlock(&CFstore_mutex);
        free(path);
        return NULL;
    }
    buffer = decompressBuffer(buffer, node->compressed_size, (size_t *)&node->stat.st_size, node->compressed_size);
    if (!buffer) {
        pthread_mutex_unlock(&CFstore_mutex);
        free(path);
        return NULL;
    }
    pthread_mutex_unlock(&CFstore_mutex);
    free(path);
    return buffer;
}
