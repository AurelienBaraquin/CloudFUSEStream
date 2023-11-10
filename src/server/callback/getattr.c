#include "CFrequest.h"
#include <sys/stat.h>
#include <string.h>

void Sv_CFuse_getattr(CFreq *req, int fd)
{
    int status = 0;
    struct stat stbuf;

    if (CFreq_find_section(req, 1) == -1) {
        status = -1;
        goto end;
    }

    char *path = req->sections[CFreq_find_section(req, 1)].data;
    if (!path) {
        status = -1;
        goto end;
    }

    if (strcmp(path, "/") == 0) {
        stbuf.st_mode = S_IFDIR | 0755;
        stbuf.st_nlink = 2;
        stbuf.st_size = 0;
    } else {
        stbuf.st_mode = S_IFREG | 0644;
        stbuf.st_nlink = 1;
        stbuf.st_size = 0;
    }

    CFreq *res = CFreq_new();

    CFreq_add_section(res, (char *)&stbuf, sizeof(struct stat), 1);

end:
    CFreq_add_section(res, (char *)&status, sizeof(int), 0);
    CFreq_send(res, fd);
    CFreq_free(res);
}
