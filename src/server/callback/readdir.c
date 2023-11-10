#include "CFrequest.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "CFtree.h"

void Sv_CFuse_readdir(CFreq *req, int fd)
{
    if (req->nsections != 2) {
        CFreq_send_error(fd, -1);
        return;
    }

    char *path = req->sections[1].data;

    CFreq *res = CFreq_new();

    CFtree_lock();
    node_t *node = CFtree_get(path);
    if (!node) {
        CFreq_send_error(fd, -ENOENT);
        RETURN_UNLOCK_TREE;
    }

    int nentries = 0;
    for (int i = 0; i < MAX_CHILD; i++) {
        if (!node->childs[i])
            continue;
        CFreq_add_section(res, &node->childs[i]->name[1], strlen(node->childs[i]->name), 2 + nentries * 2);
        CFreq_add_section(res, (char *)&node->childs[i]->stat, sizeof(struct stat), 3 + nentries * 2);
        nentries++;
    }

    CFtree_unlock();

    CFreq_add_section(res, (char *)&nentries, sizeof(int), 1);
    int status = 0;
    CFreq_add_section(res, (char *)&status, sizeof(int), 0);
    CFreq_compile(res);
    CFreq_send(res, fd);
}
