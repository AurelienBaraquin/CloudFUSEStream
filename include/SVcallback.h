#pragma once

#include "CFrequest.h"

void Sv_CFuse_getattr(CFreq *req, int fd);
void Sv_CFuse_readdir(CFreq *req, int fd);
void Sv_CFuse_create(CFreq *req, int fd);
void Sv_CFuse_write(CFreq *req, int fd);
void Sv_CFuse_read(CFreq *req, int fd);
