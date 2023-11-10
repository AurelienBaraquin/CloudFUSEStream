#include "CFrequest.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "CFtree.h"

void Sv_CFuse_getattr(CFreq *req, int fd)
{
    if (req->nsections != 2) {
        // TODO : send error
        return;
    }

    char *path = req->sections[1].data;

    CFtree_lock();
    node_t *node = CFtree_get(path);
    if (!node) {
        CFreq_send_error(fd, -ENOENT);
        RETURN_UNLOCK_TREE;
    }

    struct stat stbuf;
    stbuf.st_mode = node->stat.st_mode;
    stbuf.st_nlink = node->stat.st_nlink;
    stbuf.st_uid = node->stat.st_uid;
    stbuf.st_gid = node->stat.st_gid;
    stbuf.st_size = node->stat.st_size;
    stbuf.st_atime = node->stat.st_atime;
    stbuf.st_mtime = node->stat.st_mtime;
    stbuf.st_ctime = node->stat.st_ctime;

    CFtree_unlock();

    CFreq *res = CFreq_new();
    int status = 0;
    CFreq_add_section(res, (char *)&status, sizeof(int), 0);
    CFreq_add_section(res, (char *)&stbuf, sizeof(struct stat), 1);
    CFreq_compile(res);

    CFreq_send(res, fd);
    CFreq_free(res);
}
