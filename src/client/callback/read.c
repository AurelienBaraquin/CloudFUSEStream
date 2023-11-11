#include "CFuse.h"
#include <errno.h>
#include "CFrequest.h"
#include <string.h>
#include "CFsocket.h"

int Cl_CFuse_read(const char *path, char *buf, size_t size, off_t offset,
                   struct fuse_file_info *fi)
{
    if (!path)
        return -ENOENT;
    int fd = SPClient_get();

    CFreq *req = CFreq_new();
    CFreq_add_section(req, "read", 4, 0);
    CFreq_add_section(req, (char *)path, strlen(path), 1);
    CFreq_add_section(req, (char *)&size, sizeof(size_t), 2);
    CFreq_add_section(req, (char *)&offset, sizeof(off_t), 3);
    CFreq_compile(req);

    CFreq_send(req, fd);
    CFreq_free(req);

    CFreq *res = CFreq_recv(fd);

    int status;
    CFreq_get_section(res, &status, sizeof(int), 0);
    if (status < 0) {
        CFreq_free(res);
        SPClient_release(fd);
        return status;
    }

    memcpy(buf, res->sections[1].data, status);
    CFreq_free(res);
    SPClient_release(fd);

    return status;
}
