#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libimage/bmp.h"
#include "libimage/image.h"
#include "libimage/io.h"

void print_usage(void) {
    printf("./readimage <source-image>\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_usage();
        return 0;
    }

    const char *source_path = argv[1];

    struct byte_buffer buffer = {0};
    enum read_status rf_status = read_file(source_path, &buffer);
    if (rf_status != READ_OK) {
        fprintf(stderr, "Can't read file. Exit code: '%d'", rf_status);
    }

    hex_dump(buffer.data, buffer.size);

    return 0;
}
