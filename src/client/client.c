#include "CFsocket.h"
#include <stdlib.h>
#include "CLcallback.h"

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
    return fuse_main(ac - 1, &av[1], &CFfuse_ops, &fd);
}
