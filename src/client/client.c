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

    CFSTREAM_close(fd);

    return 0;
}
