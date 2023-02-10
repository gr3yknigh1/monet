#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "monet/bmp.h"
#include "monet/io.h"
#include "monet/memory_s.h"

void print_header_info(struct bmp_header *header) {
    printf("Type \t\t\t: %d\n", header->type);
    printf("FileSize \t\t: %d\n", header->file_size);
    printf("DataOffset \t\t: %d\n", header->data_offset);
    printf("DibSize \t\t: %d\n", header->dib_size);
    printf("Width \t\t\t: %d\n", header->width);
    printf("Height \t\t\t: %d\n", header->height);
    printf("ColorPlanesCount \t: %d\n", header->planes_count);
    printf("BitsPerPixel \t\t: %d\n", header->depth);
    printf("Compression \t\t: %d\n", header->compression);
    printf("ImageSize \t\t: %d\n", header->image_size);
    printf("XResolution \t\t: %d\n", header->x_pxl_per_meter);
    printf("YResolution \t\t: %d\n", header->y_pxl_per_meter);
}

uint64_t get_image_padding(const struct image *image) {
    return (4 - (sizeof(struct color) * image->width) % 4) % 4;
}

size_t get_pixel_array_size(const struct image *image) {
    return image->width * image->height * sizeof(struct color);
}

enum read_status buffer_to_bmp(
    const struct byte_buffer *buffer, struct image *image,
    struct bmp_header *header
) {

    if (buffer->size < sizeof(struct bmp_header) + sizeof(struct image)) {
        return READ_INVALID_BITS;
    }

    memcpy_s(header, sizeof(struct bmp_header), buffer->data, sizeof(struct bmp_header));

    if (header->compression != BI_RGB) {
        return READ_UNSUPPORTED_COMP;
    } else if (header->dib_size != BI_BITMAPINFOHEADER || header->planes_count != 1) {
        return READ_INVALID_HEADER;
    }

    image->width = header->width;
    image->height = header->height;

    size_t pixel_array_size = get_pixel_array_size(image);
    size_t pixel_row_size = sizeof(struct color) * image->width;
    size_t buffer_padding = get_image_padding(image);
    size_t buffer_row_size = pixel_row_size + buffer_padding;
    image->pixels = malloc(pixel_array_size);

    for (uint64_t i = 0; i < image->height; i++) {
        memcpy_s(
            image->pixels + i * image->width,
            pixel_row_size,
            buffer->data + header->data_offset + i * buffer_row_size,
            pixel_row_size
        );
    }

    return READ_OK;
}

enum read_status read_bmp(
    const char *source_path, struct image *image, struct bmp_header *header
) {
    struct byte_buffer buffer = {0};

    enum read_status rf_status = read_file(source_path, &buffer);
    if (rf_status != READ_OK) {
        return rf_status;
    }

    enum read_status ri_status = buffer_to_bmp(&buffer, image, header);

    free(buffer.data);

    return ri_status;
}

enum write_status bmp_to_buffer(
    const struct image *image, const struct bmp_header *header,
    struct byte_buffer *buffer
) {
    size_t padding = get_image_padding(image);
    size_t pixel_array_size = get_pixel_array_size(image);
    buffer->size = sizeof(struct bmp_header) + pixel_array_size + padding * image->height;
    buffer->data = malloc(buffer->size);

    memcpy_s(buffer->data, sizeof(struct bmp_header), header, sizeof(struct bmp_header));

    size_t pixel_row_size = sizeof(struct color) * image->width;
    size_t buffer_row_size = pixel_row_size + padding;

    for (uint64_t i = 0; i < image->height; i++) {
        memcpy_s(
            buffer->data + header->data_offset + i * buffer_row_size,
            pixel_row_size,
            image->pixels + i * image->width,
            pixel_row_size
        );
    }

    return WRITE_OK;
}

enum write_status write_bmp(
    const char *dest_path, struct image *image, struct bmp_header *header
) {
    struct byte_buffer buffer = {
        .data = NULL,
        .size = 0,
    };

    if (bmp_to_buffer(image, header, &buffer) != WRITE_OK) {
        return WRITE_ERR;
    }

    if (write_file(dest_path, &buffer) != WRITE_OK) {
        return WRITE_ERR;
    }

    free(buffer.data);

    return WRITE_OK;
}
