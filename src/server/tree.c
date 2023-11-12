#include "CFtree.h"

//* Global variables _________________________________________________________*/
node_t *root = NULL;
pthread_mutex_t tree_mutex = PTHREAD_MUTEX_INITIALIZER;




//* Thread safe functions _____________________________________________________*/
void CFtree_lock(void) {
    pthread_mutex_lock(&tree_mutex);
}

void CFtree_unlock(void) {
    pthread_mutex_unlock(&tree_mutex);
}

//* New node _________________________________________________________________*/
node_t *new_node(const char *name, node_t *parent) {
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (!node) {
        return NULL;
    }

    memset(node, 0, sizeof(node_t));
    
    node->name = strdup(strrchr(name, '/'));
    node->parent = parent;
    node->compressed_size = 0;
    
    return node;
}



//* Free tree ________________________________________________________________*/
void free_node(node_t *node) {
    free(node->name);
    free(node);
}

void CFtree_free(node_t *node) {
    if (!node) {
        return;
    }
    for (int i = 0; i < MAX_CHILD; i++) {
        CFtree_free(node->childs[i]);
    }
    free(node);
}



//* Utils for tree____________________________________________________________*/
int add_child_to_parent(node_t *parent, node_t *child) {
    child->parent = parent;
    for (int i = 0; i < MAX_CHILD; i++) {
        if (!parent->childs[i]) {
            parent->childs[i] = child;
            return 0;
        }
    }
    return -1;
}

int remove_child_from_parent(node_t *parent, node_t *child) {
    for (int i = 0; i < MAX_CHILD; i++) {
        if (parent->childs[i] == child) {
            CFtree_free(child);
            parent->childs[i] = NULL;
            return 0;
        }
    }
    return -1;
}

node_t *get_node(const char *path) {
    if (!root) {
        return NULL;
    }
    
    if (strcmp(path, "/") == 0) {
        return root;
    }

    char *path_copy = strdup(path);
    char *token = strtok(path_copy, "/");
    node_t *current = root;
    while (token) {
        int found = 0;
        for (int i = 0; i < MAX_CHILD; i++) {
            if (current->childs[i] && strcmp(&current->childs[i]->name[1], token) == 0) {
                current = current->childs[i];
                found = 1;
                break;
            }
        }
        if (!found) {
            free(path_copy);
            return NULL;
        }
        token = strtok(NULL, "/");
    }
    free(path_copy);
    return current;
}

node_t *get_parent(const char *path) {
    char *path_copy = strdup(path);
    char *last_slash = strrchr(path_copy, '/');
    if (!last_slash) {
        free(path_copy);
        return NULL;
    }

    *last_slash = '\0';
    node_t *parent = get_node(path_copy);
    free(path_copy);
    return parent;
}

char *CFtree_get_fullpath(node_t *node) {
    if (!node) {
        return NULL;
    }

    char *path = strdup(node->name);
    node_t *current = node;
    while (current->parent && strcmp(current->parent->name, "/") != 0) {
        current = current->parent;
        char *tmp = malloc(strlen(path) + strlen(current->name) + 1);
        strcpy(tmp, current->name);
        strcpy(tmp + strlen(current->name), path);
        tmp[strlen(current->name) + strlen(path)] = '\0';
        free(path);
        path = tmp;
    }
    return path;
}

void CFtree_init(void) {
    root = (node_t *)malloc(sizeof(node_t));
    if (!root)
        return;

    memset(root, 0, sizeof(node_t));
    root->name = strdup("/");
    root->compressed_size = 0;
    root->stat.st_mode = S_IFDIR | 0755;
    root->stat.st_nlink = 2;
    root->parent = NULL;
}



//* User side functions ______________________________________________________*/
void CFtree_init(void);

node_t *CFtree_add(const char *path) {
    if (strlen(path) < 2) {
        return NULL;
    }

    if (get_node(path)) {
        return NULL;
    }

    node_t *parent = get_parent(path);
    if (!parent) {
        return NULL;
    }
    
    node_t *file = new_node(path, parent);
    if (!file) {
        return NULL;
    }

    if (add_child_to_parent(parent, file) == -1) {
        return NULL;
    }

    return file;
}

int CFtree_remove(const char *path) {
    node_t *file = get_node(path);
    if (!file) {
        return -1;
    }
    
    return remove_child_from_parent(file->parent, file);
}

int CFtree_rename(const char *old_path, const char *new_path) {
    node_t *file = get_node(old_path);
    if (!file) {
        return -1;
    }

    node_t *new_parent = get_parent(new_path);
    if (!new_parent) {
        return -1;
    }

    if (get_node(new_path)) {
        return -1;
    }

    free(file->name);
    file->name = strdup(strrchr(new_path, '/'));
    return 0;
}



//* Getters user side_________________________________________________________*/
node_t *get_node(const char *);

node_t *CFtree_root(void) {
    return root;
}

node_t *CFtree_get(const char *path) {
    node_t *file = get_node(path);
    if (!file)
        return NULL;

    return file;
}



#include <stdio.h>

//* Print tree ________________________________________________________________*/
void CFtree_print_helper(node_t *node, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%s\n", node->name);
    for (int i = 0; i < MAX_CHILD; i++) {
        if (node->childs[i]) {
            CFtree_print_helper(node->childs[i], depth + 1);
        }
    }
}

void CFtree_print(void) {
    if (!root)
        printf("Empty tree\n");
    else
        CFtree_print_helper(root, 0);
}



#include <unistd.h> // getuid, getgid

//* Create entry ______________________________________________________________*/
/* create a new file with the given path, make sure to set reg or dir in mode */
node_t *CFtree_new_filled(const char *path, mode_t mode)
{
    node_t *file = CFtree_add(path);
    if (!file)
        return NULL;
    
    file->stat.st_mode = mode;
    file->stat.st_nlink = mode & S_IFDIR ? 2 : 1;
    file->stat.st_size = 0;
    file->stat.st_uid = getuid();
    file->stat.st_gid = getgid();
    file->stat.st_atime = time(NULL);
    file->stat.st_mtime = time(NULL);
    file->stat.st_ctime = time(NULL);

    return file;
}
