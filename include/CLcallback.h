#pragma once

#include "CFuse.h"

int Cl_CFuse_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi);
