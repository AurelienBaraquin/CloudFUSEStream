#include "CFsocket.h"
#include <stdlib.h>

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
    CFreq_add_section(req, "getattr", 8, 0);
    CFreq_add_section(req, (char *)path, strlen(path), 1);
    CFreq_compile(req);

    CFreq_lock();

    CFreq_send(req, fd);
    CFreq_free(req);

    CFreq *res = CFreq_recv(fd);

    CFreq_unlock();

    if (!res)
        return -1;

    int status = atoi(res->sections[0].data);
    if (status != 0)
        return status;
    struct stat *st = (struct stat *)res->sections[1].data;
    memcpy(stbuf, st, sizeof(struct stat));
    CFreq_free(res);

    return status;
}

CFuse_oper CFfuse_ops = {
    // Cl = Client | Sv = Server

    .getattr = Cl_CFuse_getattr,
    // .readdir = CFuse_readdir,
    // .open = CFuse_open,
    // .read = CFuse_read,
};

int client(int ac, char **av)
{
    int fd = CFSTREAM_connect(av[0], atoi(av[1]));
    if (fd == -1)
        return 1;
    return fuse_main(ac - 2, &av[2], &CFfuse_ops, &fd);
}
