#pragma once

#include <stddef.h>

unsigned char* compressBuffer(unsigned char* inBuffer, size_t inLength, size_t* outLength);
unsigned char* decompressBuffer(unsigned char* inBuffer, size_t inLength, size_t* outLength, size_t estCompressedLength);

int CFstore_init(char *path);
