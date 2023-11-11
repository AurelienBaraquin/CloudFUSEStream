#include "CFrequest.h"
#include "CFsocket.h"
#include "SVcallback.h"
#include <string.h>
#include <stdio.h>

int CFrequest_handler(CFreq *req, int fd)
{
    if (req->nsections == 0 || CFreq_find_section(req, 0) == -1)
        return -1;
    
    char *call_id = req->sections[0].data;

    for (int i = 0; i < (int)(sizeof(Sv_CFuse_ops) / sizeof(Sv_CFuse_oper)); i++) {
        if (strcmp(call_id, Sv_CFuse_ops[i].call_id) == 0) {
            Sv_CFuse_ops[i].func(req, fd);
            return 0;
        }
    }

    return 0;
}

void *CFStream_handle(int fd)
{
    CFreq *req = NULL;
    while (1) {
        req = CFreq_recv(fd);
        if (!req)
            break;

        if (CFrequest_handler(req, fd) == -1) {
            CFreq_send_error(fd, -1);
            CFreq_free(req);
            continue;
        }

        CFreq_free(req);
    }

    printf("Connection closed : %d\n", fd);
    CFStream_close(fd);
    return NULL;
}
