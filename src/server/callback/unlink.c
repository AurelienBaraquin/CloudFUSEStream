#include "CFrequest.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "CFtree.h"
#include "CFstore.h"

void Sv_CFuse_unlink(CFreq *req, int fd)
{
    if (req->nsections != 2) {
        CFreq_send_error(fd, -1);
        return;
    }

    char *path = req->sections[1].data;

    CFreq *res = CFreq_new();

    CFtree_lock();
    if (CFstore_CFtree_remove(CFtree_get(path)) != 0) {
        CFreq_send_error(fd, -ENOENT);
        RETURN_UNLOCK_TREE;
    }

    CFtree_unlock();

    int status = 0;
    CFreq_add_section(res, (char *)&status, sizeof(int), 0);
    CFreq_compile(res);
    CFreq_send(res, fd);
    CFreq_free(res);
}
