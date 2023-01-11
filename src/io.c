#include "readimage/io.h"

#include <stdio.h>
#include <stdlib.h>

ReadFileStatus ByteBuffer_ReadFile(const char *filePath, ByteBuffer **out) {
    FILE *fstream = fopen(filePath, "r");
    if (fstream == NULL) {
        return READFILE_ERR;
    }

    fseek(fstream, 0, SEEK_END);
    uint64_t fileSize = ftell(fstream);
    fseek(fstream, 0, SEEK_SET);

    ByteBuffer *buffer = malloc(sizeof(ByteBuffer));
    buffer->data = malloc(sizeof(byte) * fileSize + 1);
    buffer->size = fileSize;

    fread(buffer->data, sizeof(byte), fileSize, fstream);
    fclose(fstream);

    buffer->data[fileSize] = '\0';
    *out = buffer;

    return READFILE_OK;
}
