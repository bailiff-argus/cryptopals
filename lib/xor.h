#ifndef XOR_H
#define XOR_H

#include <stdint.h>
#include <stddef.h>

enum xor_error {
  XOR_OK,
};

/// @brief Produces the XOR combination of two equal-length buffers a and b and writes it into dst.
enum xor_error XOR_XORArrays(uint8_t *dst, const uint8_t *a, const uint8_t *b, size_t len);

#endif // !XOR_H
