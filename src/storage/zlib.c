#include <zlib.h>
#include <stdlib.h>

unsigned char* compressBuffer(unsigned char* inBuffer, size_t inLength, size_t* outLength)
{
    unsigned long estCompressedSize = compressBound(inLength);
    unsigned char* outBuffer = (unsigned char*)malloc(estCompressedSize);

    if (compress(outBuffer, &estCompressedSize, inBuffer, inLength) != Z_OK) {
        free(outBuffer);
        return NULL;
    }

    *outLength = estCompressedSize;
    return outBuffer;
}

unsigned char* decompressBuffer(unsigned char* inBuffer, size_t inLength, size_t estDecompressedSize)
{
    unsigned char* outBuffer = (unsigned char*)malloc(estDecompressedSize);

    if (uncompress(outBuffer, &estDecompressedSize, inBuffer, inLength) != Z_OK) {
        free(outBuffer);
        return NULL;
    }

    return outBuffer;
}
