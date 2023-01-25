#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stdio.h>

void hex_dump(void *data, uint64_t buffer_length);

struct byte_buffer {
    unsigned char *data;
    uint64_t size;
};

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
    const char *file_path, struct byte_buffer *buffer
);

enum write_status {
    WRITE_OK,
    WRITE_ERR,
};

enum write_status write_file(
    const char *file_path, struct byte_buffer *buffer
);
#endif // IO_H
