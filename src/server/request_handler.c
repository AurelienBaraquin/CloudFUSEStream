#include "CFrequest.h"
#include "CFsocket.h"
#include "SVcallback.h"
#include <string.h>
#include <stdio.h>
#include "color.h"

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

    CFreq_send_error(fd, -1);
    return -1;
}

void *CFStream_handle(int fd)
{
    CFreq *req = NULL;
    while (1) {
        req = CFreq_recv(fd);
        if (!req)
            break;

        printf("%d : "COLOR_YELLOW"%s\t"COLOR_RESET"-> "COLOR_YELLOW"Started\n" COLOR_RESET, fd, req->sections[0].data);

        if (CFrequest_handler(req, fd) == -1) {
            printf(COLOR_RED "%d : %s\t"COLOR_RESET"->"COLOR_RED" Error\n" COLOR_RESET, fd, req->sections[0].data);
            CFreq_send_error(fd, -1);
            CFreq_free(req);
            continue;
        }

        printf("%d : "COLOR_YELLOW"%s\t"COLOR_RESET"->"COLOR_YELLOW" Done\n" COLOR_RESET, fd, req->sections[0].data);
        CFreq_free(req);
    }

    printf("%d\t-> "COLOR_GREEN"Disconnected\n" COLOR_RESET, fd);
    CFStream_close(fd);
    return NULL;
}
