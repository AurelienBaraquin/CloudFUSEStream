#include "CFrequest.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "CFtree.h"

void Sv_CFuse_mkdir(CFreq *req, int fd)
{
    if (req->nsections != 3) {
        CFreq_send_error(fd, -1);
        return;
    }

    char *path = req->sections[1].data;

    CFreq *res = CFreq_new();

    CFtree_lock();
    if (CFtree_get(path) != NULL) {
        CFreq_send_error(fd, -EEXIST);
        RETURN_UNLOCK_TREE;
    }

    mode_t mode;
    CFreq_get_section(req, &mode, sizeof(mode_t), 2);

    node_t *node = CFtree_new_filled(path, mode | __S_IFDIR);
    if (!node) {
        CFreq_send_error(fd, -ENOMEM);
        RETURN_UNLOCK_TREE;
    }

    CFtree_unlock();

    int status = 0;
    CFreq_add_section(res, (char *)&status, sizeof(int), 0);
    CFreq_compile(res);
    CFreq_send(res, fd);
    CFreq_free(res);
}
