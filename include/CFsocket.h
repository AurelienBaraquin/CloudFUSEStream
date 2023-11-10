#pragma once

#include <sys/types.h>

int CFSTREAM_host(int port);
int CFSTREAM_connect(const char *host, int port);
int CFSTREAM_send(int fd, const char *request, size_t len);
char *CFSTREAM_recv(int fd);
int CFSTREAM_close(int fd);
