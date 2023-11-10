#include "CFuse.h"
#include <errno.h>
#include "CFrequest.h"
#include <string.h>

int Cl_CFuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags)
{
    int fd = *(int *)fuse_get_context()->private_data;
    if (!path)
        return -ENOENT;

    CFreq *req = CFreq_new();
    CFreq_add_section(req, "readdir", 7, 0);
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

    int nentries;
    CFreq_get_section(res, &nentries, sizeof(int), 1);

    for (int i = 0; i < nentries; i++) {
        char name[256];
        CFreq_get_section(res, name, 256, 2 + i * 2);
        struct stat stbuf;
        CFreq_get_section(res, &stbuf, sizeof(struct stat), 3 + i * 2);
        filler(buf, name, &stbuf, 0, FUSE_FILL_DIR_PLUS);
    }

    CFreq_free(res);

    return status;
}
