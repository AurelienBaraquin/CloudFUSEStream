#include "CFstore.h"
#include "CFtree.h" // only for node_t struct
#include <pthread.h>

char *CFstore_getPath(const char *path);

pthread_mutex_t CFstore_mutex = PTHREAD_MUTEX_INITIALIZER;

