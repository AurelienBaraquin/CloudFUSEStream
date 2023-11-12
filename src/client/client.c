#include "CFsocket.h"
#include <stdlib.h>
#include "CLcallback.h"

CFuse_oper CFfuse_ops = {
    .getattr = Cl_CFuse_getattr,
    .readdir = Cl_CFuse_readdir,
    .create = Cl_CFuse_create,
    .mkdir = Cl_CFuse_mkdir,
    // .open = Cl_CFuse_open,
    .read = Cl_CFuse_read,
    .write = Cl_CFuse_write,
    .unlink = Cl_CFuse_unlink,
};

int client(int ac, char **av)
{
    SPClient_init(av[0], atoi(av[1]));
    return fuse_main(ac - 1, &av[1], &CFfuse_ops, NULL);
}
