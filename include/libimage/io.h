#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stdio.h>

void hex_dump(unsigned char *data, size_t size);

enum read_status {
    READ_OK,
    READ_ERR,
    READ_FILE_NOT_FOUND,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_UNSUPPORTED_COMP,
};

enum read_status read_file(
    const char *path, unsigned char *dest, size_t *size
);

enum write_status {
    WRITE_OK,
    WRITE_ERR,
};

enum write_status write_file(
    const char *path, const unsigned char *src, const size_t size
);
#endif // IO_H
