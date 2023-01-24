#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libimage/bmp.h"
#include "libimage/image.h"

void print_usage(void) {
    printf("./readimage <source-image>\n");
}

int main(int argc, char **argv) {
    if (argc < 3) {
        print_usage();
        return 0;
    }

    const char *source_path = argv[1];
    const char *dest_path = argv[2];

    struct image source_image = {0};
    struct bmp_header header = {0};

    enum read_status r_status = read_bmp(source_path, &source_image, &header);
    if (r_status != READ_OK) {
        fprintf(
            stderr, "Error while reading image '%s', status code: '%d'\n",
            source_path, r_status
        );
        return 1;
    }

    struct image rotated_image = rotate_image(&source_image, ROTATION_LEFT);
    header.width = rotated_image.width;
    header.height = rotated_image.height;

    enum write_status w_status = write_bmp(dest_path, &rotated_image, &header);
    if (w_status != WRITE_OK) {
        fprintf(
            stderr,
            "Error while writing image out to path '%s'. Status code: '%d'\n",
            dest_path, w_status
        );
        return 1;
    }

    free(source_image.pixels);
    free(rotated_image.pixels);

    return 0;
}
