#include "CFuse.h"
#include <errno.h>
#include "CFrequest.h"
#include <string.h>
#include "CFsocket.h"

int Cl_CFuse_mkdir(const char *path, mode_t mode)
{
    if (!path)
        return -ENOENT;
    int fd = SPClient_get();

    CFreq *req = CFreq_new();
    CFreq_add_section(req, "mkdir", 5, 0);
    CFreq_add_section(req, (char *)path, strlen(path), 1);
    CFreq_add_section(req, (char *)&mode, sizeof(mode_t), 2);
    CFreq_compile(req);

    CFreq_send(req, fd);
    CFreq_free(req);

    CFreq *res = CFreq_recv(fd);

    int status;
    CFreq_get_section(res, &status, sizeof(int), 0);
    CFreq_free(res);
    SPClient_release(fd);

    return status;
}
