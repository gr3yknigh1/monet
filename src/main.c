#include <stdio.h>

#include "readimage/io.h"
#include "readimage/image.h"
#include "readimage/bmp.h"

int main(void) {

    const char *filePath = "./data/image.bmp";

    ByteBuffer *buffer = NULL;
    if (ByteBuffer_ReadFile(filePath, &buffer) != READFILE_OK) {
        printf("Error on reading file from '%s'.\n", filePath);
        return 1;
    }

    BMPImage *image = NULL;
    int status = BMPImage_Read(buffer, &image);
    if (status != READBMP_OK) {
        printf("Error on reading BMP image from '%s'.\n", filePath);
        printf("Status code: %d\n", status);
        return 1;
    }
    BMPImage_PrintInfo(image);
    BMPImage_Free(image);
    return 0;
}
