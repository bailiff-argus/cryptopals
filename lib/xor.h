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

typedef struct key_len_candidate_t {
  size_t key_len;
  double penalty;
} key_len_candidate_t;


/// @brief Produces the XOR combination of two equal-length buffers a and b and writes it into dst.
enum xor_error XOR_XORArrays(uint8_t *dst, const uint8_t *a, const uint8_t *b, size_t len);

/// @brief Produces the XOR combination of arr and key that's repeated along arr's length, and writes the result into dst.
enum xor_error XOR_XORArrayWithKey(uint8_t *dst, const uint8_t *arr, size_t arr_len, const uint8_t *key, size_t key_len);

/// @brief XOR bytes against every possible single-byte key and rank keys depending on how similar the letter distribution
///        is to what's expected of the English language.
enum xor_error XOR_ProduceKeyScores(key_candidate_t keys[256], const uint8_t *ciphertext, size_t len);

/// @brief Attempt to break a single-byte XOR encryption of ciphertext, which is known to be an English text.
///        Store the encryption key in key param.
///
/// Note that this is a convenience function. It's a thin wrapper around XOR_ProduceKeyScores, and it removes access
/// to the keys array, which may be useful.
enum xor_error XOR_Break(const uint8_t *ciphertext, size_t len, uint8_t *key);

/// @brief Calculate the Hamming distance (number of different bits) between two strings.
size_t XOR_CalcHammingDistance(const uint8_t *str_a, const uint8_t *str_b, size_t len);

/// @brief For a Vigenere-encrypted text, try to figure out the key length.
/// @param keylens Buffer of size (key_maxlen - key_minlen + 1) * sizeof(key_len_candidate_t).
size_t XOR_GuessXORKeyLength(key_len_candidate_t *keylens, size_t key_minlen, size_t key_maxlen, const uint8_t *ciphertext, size_t len);


#endif // !XOR_H
