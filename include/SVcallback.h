#pragma once

#include "CFrequest.h"

void Sv_CFuse_getattr(CFreq *req, int fd);
void Sv_CFuse_readdir(CFreq *req, int fd);
void Sv_CFuse_create(CFreq *req, int fd);
void Sv_CFuse_mkdir(CFreq *req, int fd);
void Sv_CFuse_write(CFreq *req, int fd);
void Sv_CFuse_read(CFreq *req, int fd);

typedef struct Sv_CFuse_oper {
    char *call_id;
    void (*func)(CFreq *, int);
} Sv_CFuse_oper;

Sv_CFuse_oper Sv_CFuse_ops[] = {
    {"getattr", Sv_CFuse_getattr},
    {"readdir", Sv_CFuse_readdir},
    {"create", Sv_CFuse_create},
    {"mkdir", Sv_CFuse_mkdir},
    {"write", Sv_CFuse_write},
    {"read", Sv_CFuse_read},
    {NULL, NULL},
};
