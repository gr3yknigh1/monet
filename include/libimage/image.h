#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

struct color {
    uint8_t b, g, r;
};

struct image {
    uint64_t width, height;
    struct color *pixels;
};

struct color *get_pixel(const struct image *image, uint64_t w, uint64_t h);

enum rotation {
    ROTATION_LEFT,
    ROTATION_RIGHT,
};

struct image rotate_image(const struct image *source, enum rotation rotation);


void read_image(const char *path, struct image *image);


#endif // IMAGE_H
