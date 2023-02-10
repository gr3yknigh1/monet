#include "monet/image.h"

#include <memory.h>
#include <stdlib.h>

struct color *get_pixel(const struct image *image, uint64_t w, uint64_t h) {
    return image->pixels + (image->width * h + w);
}

struct image rotate_image(const struct image *source, enum rotation rotation) {
    size_t pixel_buffer_size =
        sizeof(struct color) * source->width * source->height;
    struct image rotated_image = (struct image) {
        .pixels = malloc(pixel_buffer_size),
        .width  = source->height,
        .height = source->width,
    };

    for (uint64_t h = 0; h < source->height; h++) {
        for (uint64_t w = 0; w < source->width; w++) {
            const struct color *src = get_pixel(source, w, h);
            struct color *dest;
            switch (rotation) {
            case ROTATION_LEFT:
                dest = get_pixel(&rotated_image, source->height - 1 - h, w);
                break;
            case ROTATION_RIGHT:
                dest = get_pixel(&rotated_image, h, source->width - 1 - w);
                break;
            }
            *dest = *src;
        }
    }

    return rotated_image;
}
