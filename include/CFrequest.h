#pragma once

#include <stddef.h>
#include <sys/types.h>

typedef struct section_s {
    int label;
    size_t size;
    off_t offset;
    char *data;
} CFsection;

/* Contains the request data, which is a list of sections which each section pointing to a data buffer in the data field */
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
