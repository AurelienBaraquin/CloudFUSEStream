#include "CFrequest.h"
#include "CFsocket.h"
#include "Svcallback.h"
#include <string.h>
#include <stdio.h>

int CFrequest_handler(CFreq *req, int fd)
{
    if (req->nsections == 0 || CFreq_find_section(req, 0) == -1)
        return -1;
    
    char *call_id = req->sections[CFreq_find_section(req, 0)].data;

    if (strcmp(call_id, "getattr") == 0)
        Sv_CFuse_getattr(req, fd);

    return 0;
}

void *CFSTREAM_handle(int fd)
{
    CFreq *req = NULL;
    while (1) {
        req = CFreq_recv(fd);
        if (!req)
            break;

        if (CFrequest_handler(req, fd) == -1) {
            // TODO : send error
            CFreq_free(req);
            continue;
        }

        CFreq_free(req);
    }

    printf("Connection closed : %d\n", fd);
    CFSTREAM_close(fd);
    return NULL;
}