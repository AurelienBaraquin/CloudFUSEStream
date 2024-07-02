#include <stdlib.h>
#include <string.h>
#include <dirent.h>

static char *pathToStore = "/store";

int CFstore_init(char *path)
{
    // pathToStore = getenv("CFUSE_PATH"); // TODO : see if necessary
    // if (!pathToStore) ...
    // try to open the directory
    DIR *fd;
    if ((fd = opendir(path)) == NULL)
        return -1;
    closedir(fd);
    pathToStore = path;
    return 0;
}

char *CFstore_getPath(const char *path)
{
    if (!pathToStore)
        return NULL;
    if (!path)
        return NULL;
    char *fullPath = (char *)malloc(strlen(pathToStore) + strlen(path) + 1);
    if (!fullPath)
        return NULL;
    strcpy(fullPath, pathToStore);
    strcat(fullPath, path);
    return fullPath;
}
