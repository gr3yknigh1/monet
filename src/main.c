#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum {
    OK,
    ERR,
};

typedef unsigned char byte;


int read_file(const char *path, byte **buffer, unsigned long *buffer_size) {
    FILE* fstream = fopen(path, "rb");

    if (fstream == NULL) {
        printf("[ERROR]: Can't open file on path: '%s'\n", path);
        return ERR;
    }

    fseek(fstream, 0, SEEK_END);
    long fsize = ftell(fstream);
    fseek(fstream, 0, SEEK_SET);

    byte *fbuffer = malloc(fsize + 1);
    fread(fbuffer, sizeof(char), fsize, fstream);
    fclose(fstream);
    fbuffer[fsize] = '\0';

    *buffer = fbuffer;
    *buffer_size = fsize;
    return OK;
}


void hex_dump(void* data, unsigned long buffer_length) {
    byte* buffer = (byte*)data;

    for (unsigned int i = 0; i < buffer_length; i += 16) {
        printf("%06x: ", i);

        for (unsigned int j = 0; j < 16; j++) {
            if (i + j < buffer_length) {
                printf("%02x ", buffer[i + j]);
            }
            else {
                printf("   ");
            }
        }

        printf(" ");
        for (unsigned int j = 0; j < 16; j++) {
            if (i + j < buffer_length) {
                printf("%c", isprint(buffer[i + j]) ? buffer[i+j] : '.');
            }
        }
        printf("\n");
    }
}


typedef struct Buffer {
    byte* data;
    unsigned long size;
} Buffer;


bool is_ascii(char c) {
    return c >= 0 && c <= 127;
}


bool is_binary(Buffer* buf) {
    return !is_ascii(buf->data[0]);
}


int main(void) {
    const char* file_path = "data/image.png";

    Buffer buffer = {
        .data = NULL,
        .size = 0,
    };

    if (read_file(file_path, &buffer.data, &buffer.size) != OK) {
        printf("[ERROR]: Error on file reading! '%s'\n", file_path);
        return ERR;
    }

    printf("Reading: '%s'\n", file_path);
    printf("Type: %s\n", is_binary(&buffer) ? "Binary" : "Text");
    printf("Hex dump of '%s':\n", file_path);
    hex_dump(buffer.data, buffer.size);

    return OK;
}
