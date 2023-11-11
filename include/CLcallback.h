#pragma once

#include "CFuse.h"

int Cl_CFuse_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi);
int Cl_CFuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags);
int Cl_CFuse_create(const char *path, mode_t mode, struct fuse_file_info *fi);
int Cl_CFuse_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int Cl_CFuse_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
