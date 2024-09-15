#include <stdint.h>
#include <stddef.h>

#include "xor.h"

enum xor_error XOR_XORArrays(uint8_t *dst, const uint8_t *a, const uint8_t *b, size_t len) {
  for (size_t i = 0; i < len; i++)
    dst[i] = a[i] ^ b[i];

  return XOR_OK;
}
