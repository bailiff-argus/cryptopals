#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "xor.h"
#include "freq.h"
#include "helper.h"

static int key_candidate_cmp(const void *kc_a, const void *kc_b) {
  const key_candidate_t *key_a = kc_a, *key_b = kc_b;
  return (key_a->penalty > key_b->penalty) - (key_b->penalty - key_a->penalty);
}

static int key_len_candidate_cmp(const void *kc_a, const void *kc_b) {
  const key_len_candidate_t *key_a = kc_a, *key_b = kc_b;
  return (key_a->penalty > key_b->penalty) - (key_b->penalty - key_a->penalty);
}

enum xor_error XOR_XORArrays(uint8_t *dst, const uint8_t *a, const uint8_t *b, size_t len) {
  for (size_t i = 0; i < len; i++)
    dst[i] = a[i] ^ b[i];

  return XOR_OK;
}

enum xor_error XOR_XORArrayWithKey(uint8_t *dst, const uint8_t *arr, size_t arr_len, const uint8_t *key, size_t key_len) {
  size_t i, j; // iterating over arr and key respectively

  for (i = 0, j = 0; i < arr_len; i++, j = (j + 1) % key_len)
    dst[i] = arr[i] ^ key[j];

  return XOR_OK;
}

enum xor_error XOR_ProduceKeyScores(key_candidate_t keys[256], const uint8_t *ciphertext, size_t len) {
  freq_table_t freq_table;
  uint8_t *decrypted_bytes;

  decrypted_bytes = calloc(len + 1, 1);
  if (decrypted_bytes == NULL)
    return XOR_FAIL;

  for (size_t i = 0; i < 256; i++) {
    keys[i].key = (uint8_t)i;
    XOR_XORArrayWithKey(decrypted_bytes, ciphertext, len, &keys[i].key, 1);
    FREQ_InitializeTable(&freq_table, FREQ_TABLE_BYTE);
    if (FREQ_UpdateTableWith(&freq_table, decrypted_bytes, len) == FREQ_OK)
      keys[i].penalty = FREQ_CalcAverageDev(&freq_table, NULL);
    else // FREQ_FAIL means that *nothing* relevant was found in decrypted_bytes
      keys[i].penalty = 1.0/0.0;
  }

  free(decrypted_bytes);
  qsort(keys, 256, sizeof(key_candidate_t), key_candidate_cmp);
  return XOR_OK;
}

enum xor_error XOR_Break(const uint8_t *ciphertext, size_t len, uint8_t *key) {
  key_candidate_t keys[256] = {0};
  enum xor_error err = XOR_OK;

  if ((err = XOR_ProduceKeyScores(keys, ciphertext, len)) != XOR_OK)
    return err;

  *key = keys[0].key;
  return err;
}

static inline uint8_t countSetBits(uint8_t byte) {
  static const uint8_t HALFBYTE_LOOKUP[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, };
  return HALFBYTE_LOOKUP[byte >> 4] + HALFBYTE_LOOKUP[byte & 0x0f];
}

size_t XOR_CalcHammingDistance(const uint8_t *str_a, const uint8_t *str_b, size_t len) {
  size_t hamming_distance = 0;
  for (size_t i = 0; i < len; i++)
    hamming_distance += countSetBits(str_a[i] ^ str_b[i]);
  return hamming_distance;
}

static double calcAverageHammingDistancePerByte(const uint8_t *text, size_t text_size, size_t block_size) {
  size_t iterations = (text_size / block_size - 1);
  double average = 0.0;

  for (size_t i = 0; i < iterations; i++)
    average += (double)XOR_CalcHammingDistance(text + i * block_size, text + (i + 1) * block_size, block_size) / (double)block_size;

  return average / iterations;
}

size_t XOR_GuessXORKeyLength(key_len_candidate_t *keylens, size_t key_minlen, size_t key_maxlen, const uint8_t *ciphertext, size_t len) {
  const size_t num_keys = (key_maxlen - key_minlen + 1);

  for (size_t key_len = key_minlen; key_len <= key_maxlen; key_len++) {
    keylens[key_len - key_minlen].key_len = key_len;
    keylens[key_len - key_minlen].penalty = calcAverageHammingDistancePerByte(ciphertext, len, key_len);
  }

  qsort(keylens, num_keys, sizeof(key_len_candidate_t), key_len_candidate_cmp);
  return keylens[0].key_len;
}
