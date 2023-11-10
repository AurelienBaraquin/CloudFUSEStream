#pragma once

#include <stddef.h>
#include <sys/types.h>

typedef struct section_s {
    int label;
    size_t size;
    off_t offset;
    char *data;
} CFsection;

/* Contains the request data, which is a list of sections which each section pointing to a data buffer in the data field.
    Request format:
    - Client : section[0] = call_id | section[1] = path
    - Server : section[0] = status
 */
typedef struct CFreq_s {
    size_t size;
    size_t nsections;
    CFsection *sections;
    char *data;
} CFreq;

void CFreq_init(CFreq *self);
CFreq *CFreq_new(void);
void CFreq_free(CFreq *self);
void CFreq_add_section(CFreq *req, char *data, size_t size, int label);
void CFreq_compile(CFreq *req);
CFreq *CFreq_decompile(char *data);
int CFreq_find_section(CFreq *req, int label);
int CFreq_send(CFreq *req, int fd);
CFreq *CFreq_recv(int fd);

void CFreq_lock(void);
void CFreq_unlock(void);
