#ifndef IO_H
#define IO_H

#include <stdint.h>

typedef unsigned char byte;

typedef struct ByteBuffer {
    byte *data;
    uint64_t size;
} ByteBuffer;

typedef enum ReadFileStatus {
    READFILE_OK  = 0,
    READFILE_ERR = 1,
} ReadFileStatus;

ReadFileStatus ByteBuffer_ReadFile(const char *filePath, ByteBuffer **out);

#endif // IO_H
