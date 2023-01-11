#include <stdio.h>

#include "readimage/io.h"
#include "readimage/image.h"
#include "readimage/bmp.h"

int PrintUsage() {
    printf("usage: rimg [-v | --version] [-h | --help]\n");
    printf("            <image_path>                  \n");
    return 0;
}

int ReadImage(const char *filePath) {

    ByteBuffer *buffer = NULL;
    {
        int status = ByteBuffer_ReadFile(filePath, &buffer);
        if (status != READFILE_OK) {
            if (status == READFILE_FILENOTEXITS) {
                printf("Error: file not exits! '%s'", filePath);
            } else {
                printf("Error on reading file from '%s'.\n", filePath);
            }
            return 1;
        }
    }

    BMPImage *image = NULL;
    {
        int status = BMPImage_Read(buffer, &image);
        if (status != READBMP_OK) {
            printf("Error on reading BMP image from '%s'.\n", filePath);
            printf("Status code: %d\n", status);
            return 1;
        }
    }
    BMPImage_PrintInfo(image);
    BMPImage_Free(image);

    return 0;
}

int main(int argc, char **argv) {

    if (argc == 1) {
        return PrintUsage();
    } else if (argc >= 2) {
        return ReadImage(argv[1]);
    }

    return 1;
}
