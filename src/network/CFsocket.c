#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int CFStream_host(int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY,
    };

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return -1;
    }

    if (listen(fd, 100) == -1) {
        perror("listen");
        return -1;
    }

    return fd;
}

int CFStream_connect(const char *host, int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = inet_addr(host),
    };

    if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        return 1;
    }

    return fd;
}

int CFStream_send(int fd, const char *request, size_t len)
{
    if (write(fd, &len, sizeof(len)) == -1) {
        perror("write");
        return -1;
    }

    if (write(fd, request, len) == -1) {
        perror("write");
        return -1;
    }

    return 0;
}

char *CFStream_recv(int fd)
{
    size_t len;
    if (read(fd, &len, sizeof(len)) <= 0) {
        perror("read");
        return NULL;
    }

    char *buf = malloc(len + 1);
    if (!buf) {
        perror("malloc");
        return NULL;
    }

    if (read(fd, buf, len) == -1) {
        perror("read");
        return NULL;
    }

    buf[len] = '\0';

    return buf;
}

int CFStream_close(int fd)
{
    if (close(fd) == -1) {
        perror("close");
        return -1;
    }

    return 0;
}
