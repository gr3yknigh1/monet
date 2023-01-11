#ifndef BMP_H
#define BMP_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "readimage/io.h"
#include "readimage/image.h"

typedef enum BMPCompMethod {
    BI_RGB            = 0,
    BI_RLE8           = 1,
    BI_RLE4           = 2,
    BI_BITFIELDS      = 3,
    BI_JPEG           = 4,
    BI_PNG            = 5,
    BI_ALPHABITFIELDS = 6,
    BI_CMYK           = 11,
    BI_CMYKRLE8       = 12,
    BI_CMYKRLE4       = 13,
} BMPCompMethod;

typedef enum BMPDIBHeader {
    BITMAPCOREHEADER    = 12,
    OS22XBITMAPHEADER   = 16,
    BITMAPINFOHEADER    = 40,
    BITMAPV2INFOHEADER  = 54,
    BITMAPV3INFOHEADER  = 56,
    OS22XBITMAPHEADER_L = 64,
    BITMAPV4HEADER      = 108,
    BITMAPV5HEADER      = 124,
} BMPDIBHeader;

typedef struct __attribute__((packed)) BMPHeader {
    uint16_t type;
    uint32_t fileSize;
    uint16_t reserved0;
    uint16_t reserved1;
    uint32_t dataOffset;

    // NOTE: DIB Header
    uint32_t      dibSize;
    int32_t       width;
    int32_t       height;
    uint16_t      colorPlanesCount;
    uint16_t      bitsPerPixel;
    BMPCompMethod compression;
    uint32_t      imageSize;
    int32_t       xResolution;
    int32_t       yResolution;
    uint32_t      colorUsed;
    uint32_t      colorImportant;
} BMPHeader;

typedef struct BMPImage {
    BMPHeader *header;

    uint64_t width, height;
    Color *pixels;
} BMPImage;

typedef enum ReadBMPStatus {
    READBMP_OK  = 0,
    READBMP_ERR = 1,
    READBMP_UNSUPPORTED_DIBHEADER = 10,
    READBMP_UNSUPPORTED_COMPRESSION = 20,
} ReadBMPStatus;

#define COPY_FROM_BUFFER(B, S, F, D) \
    memcpy(&D->F, &B->data[offsetof(S, F)], sizeof(D->F))


ReadBMPStatus BMPImage_Read(const ByteBuffer *buffer, BMPImage **out) {
    BMPHeader *header = malloc(sizeof(BMPHeader));
    BMPImage  *image  = malloc(sizeof(BMPImage ));

    image->header = header;

    COPY_FROM_BUFFER(buffer, BMPHeader, type, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, fileSize, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, reserved0, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, reserved1, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, dataOffset, header);

    COPY_FROM_BUFFER(buffer, BMPHeader, dibSize, header);

    if (header->dibSize != BITMAPINFOHEADER) {
        printf("Error: we suppot only BITMAPINFOHEADER. Got header size of '%d'\n", header->dibSize);
        return READBMP_UNSUPPORTED_DIBHEADER;
    }

    COPY_FROM_BUFFER(buffer, BMPHeader, width, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, height, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, colorPlanesCount, header);

    if (header->colorPlanesCount != 1) {
        return READBMP_ERR;
    }

    COPY_FROM_BUFFER(buffer, BMPHeader, bitsPerPixel, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, compression, header);

    if (header->compression != BI_RGB) {
        return READBMP_UNSUPPORTED_COMPRESSION;
    }

    COPY_FROM_BUFFER(buffer, BMPHeader, imageSize, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, xResolution, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, yResolution, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, colorUsed, header);
    COPY_FROM_BUFFER(buffer, BMPHeader, colorImportant, header);

    image->width = header->width;
    image->height = header->height;

    uint64_t size = image->width * image->height;
    image->pixels = malloc(sizeof(Color) * size);
    memcpy(
        image->pixels,
        &buffer->data[header->dataOffset],
        sizeof(Color) * size
        );

    *out = image;

    return READBMP_OK;
}

void BMPImage_PrintInfo(BMPImage *image) {
    printf("Type \t\t\t: %d\n"        , image->header->type);
    printf("FileSize \t\t: %d\n"      , image->header->fileSize);
    printf("Reserved0 \t\t: %d\n"     , image->header->reserved0);
    printf("Reserved1 \t\t: %d\n"     , image->header->reserved1);
    printf("DataOffset \t\t: %d\n"    , image->header->dataOffset);
    printf("DibSize \t\t: %d\n"       , image->header->dibSize);
    printf("Width \t\t\t: %d\n"       , image->header->width);
    printf("Height \t\t\t: %d\n"      , image->header->height);
    printf("ColorPlanesCount \t: %d\n", image->header->colorPlanesCount);
    printf("BitsPerPixel \t\t: %d\n"  , image->header->bitsPerPixel);
    printf("Compression \t\t: %d\n"   , image->header->compression);
    printf("ImageSize \t\t: %d\n"     , image->header->imageSize);
    printf("XResolution \t\t: %d\n"   , image->header->xResolution);
    printf("YResolution \t\t: %d\n"   , image->header->yResolution);
    printf("ColorUsed \t\t: %d\n"     , image->header->colorUsed);
    printf("ColorImportant \t\t: %d\n", image->header->colorImportant);

    Color c = image->pixels[0];
    printf("First color value: '%x%x%x'\n", c.r, c.g, c.b);
}

void BMPImage_Free(void *ptr) {
    BMPImage *image = (BMPImage*)ptr;
    free(image->header);
    free(image->pixels);
    free(image);
}

#endif // BMP_H
