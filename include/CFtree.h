#pragma once

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#define MAX_CHILD 100

typedef struct node {
    char *name;
    unsigned char *content;
    size_t compressed_size;
    struct stat stat;
    struct node *parent;
    struct node *childs[MAX_CHILD];
} node_t;

/*  type    |   name     |   content |   size    |   parent  |   childs
    reg     |     Y      |     Y     |     Y     |     Y     |      N
    dir     |     Y      |     N     |     N     |     Y     |      Y
    root    |   ("/")    |     N     |     N     |     N     |      Y
*/

// strrchr: find the last occurence of any char in a string

/* Don't forget to fill stat struct after creating a node */
node_t *CFtree_add(const char *path);
int CFtree_remove(const char *path);
int CFtree_rename(const char *old_path, const char *new_path);
node_t *CFtree_get(const char *path);
void CFtree_print(void);
void CFtree_free(node_t *node);

node_t *CFtree_root(void);

void CFtree_init(void);

void CFtree_lock(void);
void CFtree_unlock(void);

node_t *CFtree_new_filled(const char *path, mode_t mode);

#define RETURN_CFtree_unlock(x) do { CFtree_unlock(); return x; } while(0)
