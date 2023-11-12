#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int CFstore_write(const char *path, const unsigned char *data, size_t size)
{
    if (!path)
        return -1;
    FILE *fd = fopen(path, "w");
    if (!fd)
        return -1;
    fwrite(data, size, 1, fd);
    fclose(fd);
    return 0;
}

int CFstore_remove(const char *path)
{
    if (!path)
        return -1;
    if (remove(path) == -1)
        return -1;
    return 0;
}

int CFstore_read(const char *path, unsigned char *buffer, size_t size)
{
    if (!path)
        return -1;
    FILE *fd = fopen(path, "r");
    if (!fd)
        return -1;
    fread(buffer, size, 1, fd);
    fclose(fd);
    return 0;
}
