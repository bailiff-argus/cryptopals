#include "lib/encoding.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <encoding.h>
#include <helper.h>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>

#define INPUT_FILE "files/s1c7"

static void decrypt(unsigned char *ciphertext, const uint8_t *key, size_t ciphertext_len) {
  int len = 0, plaintext_len = ciphertext_len;
  unsigned char *plaintext = calloc(plaintext_len + 1, sizeof(unsigned char));
  EVP_CIPHER_CTX *dec_ctx = NULL;

  (void)key;

  dec_ctx = EVP_CIPHER_CTX_new();

  EVP_DecryptInit_ex(dec_ctx, EVP_aes_128_ecb(), NULL, key, NULL);

  EVP_DecryptUpdate(dec_ctx, plaintext, &len, ciphertext, ciphertext_len);
  plaintext_len = len;

  EVP_DecryptFinal_ex(dec_ctx, plaintext + plaintext_len, &len);
  plaintext_len += len;

  printf("%s\n", plaintext);

  EVP_CIPHER_CTX_free(dec_ctx);
  free(plaintext);
}

int main(void) {
  const uint8_t key[16] = "YELLOW SUBMARINE";

  FILE *fp = NULL;

  char *base64 = NULL;

  uint8_t *ciphertext = NULL;
  size_t text_len = 0;

  EXIT_ON_NULL(fp = fopen(INPUT_FILE, "r"), "Opening " INPUT_FILE);
  EXIT_ON_NULL(base64 = calloc(62, sizeof(char)), "allocating base64_buffer");
  EXIT_ON_NULL(ciphertext = calloc(10000, sizeof(uint8_t)), "allocating bytes buffer");

  while (fgets(base64, 62, fp) != NULL) {
    size_t len = 0, tmp_bytes_len = 0;
    while ((base64[len] != '\n') && (base64[len] != '\0'))
      len++;

    base64[len] = '\0';
    tmp_bytes_len = ENC_CalcBase64DecodeBufSize(base64);

    ENC_Base64ToBytes(ciphertext + text_len, (unsigned char *)base64, len);
    text_len += tmp_bytes_len;
  }

  decrypt(ciphertext, key, text_len);

  return 0;
}
