#include "libimage/io.h"

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

void hex_dump(unsigned char *data, size_t size) {

    for (unsigned int i = 0; i < size; i += 16) {
        printf("%06x: ", i);

        for (unsigned int j = 0; j < 16; j++) {
            if (i + j < size) {
                printf("%02x ", data[i + j]);
            } else {
                printf("   ");
            }
        }

        printf(" ");
        for (unsigned int j = 0; j < 16; j++) {
            if (i + j < size) {
                printf("%c", isprint(data[i + j]) ? data[i + j] : '.');
            }
        }
        printf("\n");
    }
}

enum read_status read_file(
    const char *path, unsigned char *dest, size_t *size
) {
    if (access(path, F_OK) != 0) {
        return READ_FILE_NOT_FOUND;
    }

    FILE *fstream = fopen(path, "r");

    if (fstream == NULL) {
        return READ_ERR;
    }

    fseek(fstream, 0, SEEK_END);
    *size = ftell(fstream);
    fseek(fstream, 0, SEEK_SET);

    dest = malloc(*size);

    fread(dest, sizeof(char), *size, fstream);
    fclose(fstream);

    return READ_OK;
}

enum write_status write_file(
    const char *path, const unsigned char *src, const size_t size
) {
    FILE *fstream = fopen(path, "w");

    if (fstream == NULL) {
        return WRITE_ERR;
    }

    // NOTE: Can't pass MSan check:
    // "SUMMARY: MemorySanitizer: use-of-uninitialized-value"
    for (unsigned long i = 0; i < size; i++) {
        fputc(src[i], fstream);
    }

    fclose(fstream);
    return WRITE_OK;
}

