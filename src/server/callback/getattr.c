#include "CFrequest.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void Sv_CFuse_getattr(CFreq *req, int fd)
{
    if (req->nsections != 2) {
        // TODO : send error
        return;
    }

    char *path = req->sections[1].data;
    printf("path = %s\n", path);

    struct stat stbuf;
    stbuf.st_mode = S_IFREG | 0644;
    stbuf.st_nlink = 1;
    stbuf.st_size = 0;

    CFreq *res = CFreq_new();
    int status = 0;
    CFreq_add_section(res, (char *)&status, sizeof(int), 0);
    CFreq_add_section(res, (char *)&stbuf, sizeof(struct stat), 1);
    CFreq_compile(res);

    CFreq_send(res, fd);
    CFreq_free(res);
}
