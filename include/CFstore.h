#pragma once

#include "CFtree.h"

unsigned char* compressBuffer(unsigned char* inBuffer, size_t inLength, size_t* outLength);
unsigned char* decompressBuffer(unsigned char* inBuffer, size_t inLength, size_t* outLength, size_t estCompressedLength);

int CFstore_init(char *path);

int CFstore_write(const char *path, const unsigned char *data, size_t size);
int CFstore_remove(const char *path);
int CFstore_read(const char *path, unsigned char *buffer, size_t size);

int CFstore_CFtree_set(node_t *node, unsigned char *data);
unsigned char *CFstore_CFtree_get(node_t *node);
