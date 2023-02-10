#ifndef MEMORY_S_H
#define MEMORY_S_H

#include <memory.h>

// NOTE: I can't fix bug related with this question on StackOverflow:
// https://stackoverflow.com/questions/31278172/undefined-reference-to-memcpy-s
// So I've implemented my own memcpy_s

int memcpy_s(void *const dest, size_t const destsz, void const* const src, size_t const count);

#endif // MEMORY_S_H
