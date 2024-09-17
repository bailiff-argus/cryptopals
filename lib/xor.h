#ifndef XOR_H
#define XOR_H

#include <stdint.h>
#include <stddef.h>

enum xor_error {
  XOR_OK,
  XOR_FAIL,
};

typedef struct key_candidate_t {
  uint8_t key;    // the byte used for XORing
  double penalty; // the "anti-score" of the key, lower is generally better
} key_candidate_t;

/// @brief Produces the XOR combination of two equal-length buffers a and b and writes it into dst.
enum xor_error XOR_XORArrays(uint8_t *dst, const uint8_t *a, const uint8_t *b, size_t len);

/// @brief Produces the XOR combination of arr and key that's repeated along arr's length, and writes the result into dst.
enum xor_error XOR_XORArrayWithKey(uint8_t *dst, const uint8_t *arr, size_t arr_len, const uint8_t *key, size_t key_len);

/// @brief XOR bytes against every possible single-byte key and rank keys depending on how similar the letter distribution
///        is to what's expected of the English language.
enum xor_error XOR_ProduceKeyScores(key_candidate_t keys[256], const uint8_t *xor_encrypted_bytes, size_t bytes_len);

#endif // !XOR_H
