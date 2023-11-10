#include "CFrequest.h"
#include "CFsocket.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void CFreq_init(CFreq *self)
{
    self->size = 0;
    self->nsections = 0;
    self->data = NULL;
    self->sections = NULL;
}

CFreq *CFreq_new(void)
{
    CFreq *self = calloc(1, sizeof(CFreq));

    if (!self) {
        return NULL;
    }
    CFreq_init(self);
    return self;
}

void CFreq_free(CFreq *self)
{
    if (!self)
        return;

    int i;

    for (i = 0; i < self->nsections; i++) {
        if (self->sections[i].data)
            free(self->sections[i].data);
    }
    if (self->sections)
        free(self->sections);
    if (self->data)
        free(self->data);
    free(self);
}

void CFreq_add_section(CFreq *req, char *data, size_t size, int label)
{
    req->sections = realloc(req->sections, sizeof(CFsection) * (req->nsections + 1));
    if (!req->sections) {
        perror("realloc");
        exit(1);
    }
    req->sections[req->nsections].label = label;
    req->sections[req->nsections].size = size;
    req->sections[req->nsections].offset = 0; // offset is set when we compile the request
    req->sections[req->nsections].data = malloc(size + 1);
    if (!req->sections[req->nsections].data) {
        perror("malloc");
        exit(1);
    }
    memcpy(req->sections[req->nsections].data, data, size);
    req->sections[req->nsections].data[size] = '\0';
    req->nsections++;
}

/* Compile the request into a single buffer who can be sent to the server */
void CFreq_compile(CFreq *req)
{
    size_t size = 0;
    size_t offset = 0;
    int i;

    for (i = 0; i < req->nsections; i++) {
        req->sections[i].offset = offset;
        offset += req->sections[i].size + sizeof(int) + sizeof(size_t);
        size += req->sections[i].size + sizeof(int) + sizeof(size_t);
    }
    req->size = size;
    // we alloc size, nsections and data space
    req->data = malloc(sizeof(size_t) * 2 + size);
    if (!req->data) {
        perror("malloc");
        exit(1);
    }
    memcpy(req->data, &req->size, sizeof(size_t));
    memcpy(req->data + sizeof(size_t), &req->nsections, sizeof(size_t));
    for (i = 0; i < req->nsections; i++) {
        memcpy(req->data + sizeof(size_t) * 2 + req->sections[i].offset, &req->sections[i].label, sizeof(int));
        memcpy(req->data + sizeof(size_t) * 2 + req->sections[i].offset + sizeof(int), &req->sections[i].size, sizeof(size_t));
        memcpy(req->data + sizeof(size_t) * 2 + req->sections[i].offset + sizeof(int) + sizeof(size_t), req->sections[i].data, req->sections[i].size);
    }
}

/* Return a new CFreq from the given data */
CFreq *CFreq_decompile(char *data)
{
    CFreq *req = CFreq_new();
    if (!req) {
        return NULL;
    }
    size_t size;
    size_t nsections;
    int i;

    memcpy(&size, data, sizeof(size_t));
    memcpy(&nsections, data + sizeof(size_t), sizeof(size_t));
    data += sizeof(size_t) * 2;
    for (i = 0; i < nsections; i++) {
        int label;
        size_t section_size;

        memcpy(&label, data, sizeof(int));
        memcpy(&section_size, data + sizeof(int), sizeof(size_t));
        data += sizeof(int) + sizeof(size_t);
        CFreq_add_section(req, data, section_size, label);
        data += section_size;
    }
    return req;
}

/* Return a index of the wanted section or -1 if error */
int CFreq_find_section(CFreq *req, int label)
{
    int i;

    for (i = 0; i < req->nsections; i++) {
        if (req->sections[i].label == label) {
            return i;
        }
    }
    return -1;
}

int CFreq_get_section(CFreq *req, void *to_fill, size_t size, int label)
{
    int index = CFreq_find_section(req, label);
    if (index == -1) {
        return -1;
    }
    memcpy(to_fill, req->sections[index].data, req->sections[index].size);
    return 0;
}

#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void CFreq_lock(void)
{
    pthread_mutex_lock(&mutex);
}

void CFreq_unlock(void)
{
    pthread_mutex_unlock(&mutex);
}

/* Send the request */
int CFreq_send(CFreq *req, int fd)
{
    if (CFSTREAM_send(fd, req->data, req->size + sizeof(size_t) * 2) == -1) {
        return -1;
    }
    return 0;
}

/* Receive a request */
CFreq *CFreq_recv(int fd)
{
    char *data = CFSTREAM_recv(fd);
    if (!data) {
        return NULL;
    }
    CFreq *req = CFreq_decompile(data);

    free(data);
    return req;
}

int CFreq_send_error(int fd, int error)
{
    CFreq *req = CFreq_new();
    CFreq_add_section(req, (char *)&error, sizeof(int), 0);
    CFreq_compile(req);
    CFreq_send(req, fd);
    CFreq_free(req);
    return 0;
}
