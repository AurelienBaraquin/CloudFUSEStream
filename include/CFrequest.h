#pragma once

#include <stddef.h>
#include <sys/types.h>

typedef struct section_s {
    int label;
    size_t size;
    off_t offset;
    char *data;
} CFsection;

// contains the request data, which is a list of sections which each section pointing to a data buffer in the data field
typedef struct CFreq_s {
    size_t size;
    CFsection *sections;
    char *data;
} CFreq;