#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "encoding.h"
#include "helper.h"
#include "xor.h"

#define INPUT_FILE "files/s1c6"

static void selftest(void) {
  const char selftest_a[] = "this is a test";
  const char selftest_b[] = "wokka wokka!!!";
  if (XOR_CalcHammingDistance((uint8_t *)selftest_a, (uint8_t *)selftest_b, strlen(selftest_a)) != 37)
    exit(EXIT_FAILURE);
}


int main(void) {
  FILE *fp = NULL;
  char *base64 = NULL;

  key_len_candidate_t *keylens = NULL;
  const size_t key_minlen = 1, key_maxlen = 40, keylen_count = (key_maxlen - key_minlen + 1);
  size_t key_len = 0;

  uint8_t *ciphertext = NULL, *text = NULL, *decrypt_buf = NULL, *key = NULL;
  size_t text_len = 0;

  selftest();

  // should stat the file to get some of these sizes, but we're taking the simple way
  // since everything is known beforehand
  EXIT_ON_NULL(fp = fopen(INPUT_FILE, "r"), "Opening " INPUT_FILE);
  EXIT_ON_NULL(base64 = calloc(62, sizeof(char)), "allocating base64_buffer");
  EXIT_ON_NULL(ciphertext = calloc(65 * 45, sizeof(uint8_t)), "allocating bytes buffer");
  EXIT_ON_NULL(keylens = calloc(keylen_count, sizeof(key_len_candidate_t)), "allocating keylens buffer");

  // turn the encoded message into bytes
  while (fgets(base64, 62, fp) != NULL) {
    size_t len = 0, tmp_bytes_len = 0;
    while ((base64[len] != '\n') && (base64[len] != '\0'))
      len++;

    base64[len] = '\0';
    tmp_bytes_len = ENC_CalcBase64DecodeBufSize(base64);

    ENC_Base64ToBytes(ciphertext + text_len, (unsigned char *)base64, len);
    text_len += tmp_bytes_len;
  }

  fclose(fp);
  EXIT_ON_NULL(text = calloc(text_len + 1, sizeof(uint8_t)), "allocating decrypted text");

  // guess key length
  key_len = XOR_GuessXORKeyLength(keylens, key_minlen, key_maxlen, ciphertext, text_len);
  for (size_t i = 0; i < keylen_count; i++)
    printf("%lu: %f\n", keylens[i].key_len, keylens[i].penalty);

  EXIT_ON_NULL(key = calloc(key_len + 1, sizeof(uint8_t)), "allocating key buffer");
  EXIT_ON_NULL(decrypt_buf = calloc((text_len / key_len + 1), sizeof(uint8_t)), "allocating decryption buffer");

  // run through each key_len'th byte with offsets of [0, key_len) and solve each as single-byte xor
  for (size_t offset = 0; offset < key_len; offset++) {
    size_t decrypt_len = 0;
    for (size_t i = offset; i < text_len; i += key_len)
      decrypt_buf[decrypt_len++] = ciphertext[i];

    XOR_Break(decrypt_buf, decrypt_len, &key[offset]);
  }

  printf("key: %s\n", key);

  XOR_XORArrayWithKey(text, ciphertext, text_len, key, key_len);
  printf("\n%s\n", text);

  free(base64);
  free(ciphertext);
  free(keylens);
  free(text);
  free(key);
  free(decrypt_buf);
  return 0;
}
