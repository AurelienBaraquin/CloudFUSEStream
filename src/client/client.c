#include "CFsocket.h"
#include <stdlib.h>

#include "CFrequest.h"
#include <stdio.h>
#include <string.h>

int client(int ac, char **av)
{
    int fd = CFSTREAM_connect(av[0], atoi(av[1]));
    if (fd == -1)
        return 1;

    CFreq *req = CFreq_new();
    CFreq_add_section(req, "test", 4, 30);
    CFreq_compile(req);
    CFreq_send(req, fd);
    CFreq_free(req);

    CFreq *res = CFreq_recv(fd);
    printf("label 30 : %s", res->sections[CFreq_find_section(res, 30)].data);
    printf("label 31 : %s", res->sections[CFreq_find_section(res, 31)].data);
    CFreq_free(res);

    CFSTREAM_close(fd);

    return 0;
}
