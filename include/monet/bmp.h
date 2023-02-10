#ifndef BMP_H
#define BMP_H

#include <stdint.h>

#include "monet/image.h"
#include "monet/io.h"

enum bi_dib_header {
    BI_BITMAPCOREHEADER = 12,
    BI_OS22XBITMAPHEADER_S = 16,
    BI_BITMAPINFOHEADER = 40,
    BI_BITMAPV2INFOHEADER = 52,
    BI_BITMAPV3INFOHEADER = 56,
    BI_OS22XBITMAPHEADER = 64,
    BI_BITMAPV4HEADER = 108,
    BI_BITMAPV5HEADER = 124,
};

enum bi_compression_method {
    BI_RGB = 0,
    BI_RLE8 = 1,
    BI_RLE4 = 2,
    BI_BITFIELDS = 3,
    BI_JPEG = 4,
    BI_PNG = 5,
    BI_ALPHABITFIELDS = 6,
    BI_CMYK = 11,
    BI_CMYKRLE8 = 12,
    BI_CMYKRLE4 = 13,
};

#pragma pack(push, 1)
struct bmp_header {
    uint16_t type;
    uint32_t file_size;
    uint16_t reserved0;
    uint16_t reserved1;
    uint32_t data_offset;
    enum bi_dib_header dib_size;
    uint32_t width;
    uint32_t height;
    uint16_t planes_count;
    uint16_t depth;
    enum bi_compression_method compression;
    uint32_t image_size;
    uint32_t x_pxl_per_meter;
    uint32_t y_pxl_per_meter;
    uint32_t color_used;
    uint32_t color_important;
};
#pragma pack(pop)

void print_header_info(struct bmp_header *header);

enum read_status buffer_to_bmp(
    const struct byte_buffer *buffer,
    struct image *image, struct bmp_header *header
);

enum read_status read_bmp(
    const char *source_path, struct image *image, struct bmp_header *header
);

enum write_status bmp_to_buffer(
    const struct image *image, const struct bmp_header *header,
    struct byte_buffer *buffer
);

enum write_status write_bmp(
    const char *dest_path, struct image *image, struct bmp_header *header
);
#endif // BMP_H
