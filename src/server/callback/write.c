#include "CFrequest.h"
#include <string.h>
#include <stdio.h>
#include "CFtree.h"
#include "CFstore.h"

void Sv_CFuse_write(CFreq *req, int fd)
{
    if (req->nsections != 5) {
        CFreq_send_error(fd, -1);
        return;
    }

    char *path = req->sections[1].data;

    CFreq *res = CFreq_new();

    CFtree_lock();
    node_t *node = CFtree_get(path);
    if (node == NULL) {
        CFreq_send_error(fd, -ENOENT);
        RETURN_UNLOCK_TREE;
    }

    size_t size;
    off_t offset;
    char *buf = req->sections[4].data;
    CFreq_get_section(req, &size, sizeof(size_t), 2);
    CFreq_get_section(req, &offset, sizeof(off_t), 3);

    unsigned char *data = NULL;
    if (node->stat.st_size != 0) {
        data = CFstore_CFtree_get(node);
        if (!data) {
            CFreq_send_error(fd, -ENOMEM);
            RETURN_UNLOCK_TREE;
        }
    }

    data = realloc(data, offset + size);
    if (!data) {
        CFreq_send_error(fd, -ENOMEM);
        RETURN_UNLOCK_TREE;
    }

    memcpy(data + offset, buf, size);
    node->stat.st_size = offset + size;
    node->stat.st_mtime = time(NULL);

    CFstore_CFtree_set(node, data);
    CFtree_unlock();

    int status = size;
    CFreq_add_section(res, (char *)&status, sizeof(int), 0);
    CFreq_compile(res);
    CFreq_send(res, fd);
    CFreq_free(res);

    free(data);
}
