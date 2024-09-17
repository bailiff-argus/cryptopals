#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "xor.h"
#include "freq.h"

static int key_candidate_cmp(const void *kc_a, const void *kc_b) {
  const key_candidate_t *key_a = kc_a, *key_b = kc_b;
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

enum xor_error XOR_ProduceKeyScores(key_candidate_t keys[256], const uint8_t *xor_encrypted_bytes, size_t bytes_len) {
  freq_table_t freq_table;
  uint8_t *decrypted_bytes;

  decrypted_bytes = calloc(bytes_len + 1, 1);
  if (decrypted_bytes == NULL)
    return XOR_FAIL;

  for (size_t i = 0; i < 256; i++) {
    keys[i].key = (uint8_t)i;
    XOR_XORArrayWithKey(decrypted_bytes, xor_encrypted_bytes, bytes_len, &keys[i].key, 1);
    FREQ_InitializeTable(&freq_table, FREQ_TABLE_LINGUISTIC);
    if (FREQ_UpdateTableWith(&freq_table, decrypted_bytes, bytes_len) == FREQ_OK)
      keys[i].penalty = FREQ_CalcAverageDev(&freq_table, NULL);
    else // FREQ_FAIL means that *nothing* relevant was found in decrypted_bytes
      keys[i].penalty = 1.0/0.0;
  }

  free(decrypted_bytes);
  qsort(keys, 256, sizeof(key_candidate_t), key_candidate_cmp);
  return XOR_OK;
}
