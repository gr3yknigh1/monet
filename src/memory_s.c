#include "libimage/memory_s.h"

int memcpy_s(void *const dest, size_t const destsz, void const *const src, size_t const count) {
    if (dest == NULL || src == NULL || count > destsz) {
        return 1;
    }

    for (size_t i = 0; i < count; i++) {
        ((char*)dest)[i] = ((char*)src)[i];
    }

    return 0;
}

