#include "monet/io.h"

// NOTE: for `access` function and file existing checking
#ifdef WIN32
# include <io.h>
# define F_OK 0 // NOTE: defining `F_OK` like in `unistd.h`
# define access _access
#else
# include <unistd.h>
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void hex_dump(void *data, unsigned long buffer_length) {
    unsigned char *buffer = (unsigned char *)data;

    for (unsigned int i = 0; i < buffer_length; i += 16) {
        printf("%06x: ", i);

        for (unsigned int j = 0; j < 16; j++) {
            if (i + j < buffer_length) {
                printf("%02x ", buffer[i + j]);
            } else {
                printf("   ");
            }
        }

        printf(" ");
        for (unsigned int j = 0; j < 16; j++) {
            if (i + j < buffer_length) {
                printf("%c", isprint(buffer[i + j]) ? buffer[i + j] : '.');
            }
        }
        printf("\n");
    }
}

enum read_status read_file(
    const char *file_path, struct byte_buffer *buffer
) {
    if (access(file_path, F_OK) != 0) {
        return READ_FILE_NOT_FOUND;
    }

    FILE *fstream = fopen(file_path, "r");

    if (fstream == NULL) {
        return READ_ERR;
    }

    fseek(fstream, 0, SEEK_END);
    unsigned long size = ftell(fstream);
    fseek(fstream, 0, SEEK_SET);

    unsigned char *data = malloc(size);

    fread(data, sizeof(char), size, fstream);
    fclose(fstream);

    buffer->data = data;
    buffer->size = size;

    return READ_OK;
}

enum write_status write_file(const char *file_path, struct byte_buffer *buffer) {
    FILE *fstream = fopen(file_path, "w");

    if (fstream == NULL) {
        return WRITE_ERR;
    }

    for (unsigned long i = 0; i < buffer->size; i++) {
        fputc(buffer->data[i], fstream);
    }

    fclose(fstream);
    return WRITE_OK;
}

