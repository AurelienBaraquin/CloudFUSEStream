#include "CFuse.h"
#include <errno.h>
#include "CFrequest.h"
#include <string.h>

int Cl_CFuse_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
    int fd = *(int *)fuse_get_context()->private_data;
    if (!path)
        return -ENOENT;
    
    CFreq *req = CFreq_new();
    CFreq_add_section(req, "getattr", 7, 0);
    CFreq_add_section(req, (char *)path, strlen(path), 1);
    CFreq_compile(req);

    CFreq_lock();

    CFreq_send(req, fd);
    CFreq_free(req);

    CFreq *res = CFreq_recv(fd);
    CFreq_unlock();

    int status;
    CFreq_get_section(res, &status, sizeof(int), 0);

    if (status != 0) {
        CFreq_free(res);
        CFreq_unlock();
        return status;
    }
    
    CFreq_get_section(res, stbuf, sizeof(struct stat), 1);
    CFreq_free(res);

    return status;
}
