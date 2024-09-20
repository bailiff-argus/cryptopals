#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encoding.h"
#include "helper.h"
#include "xor.h"

int main(void) {
  key_candidate_t keys[256];
  const char hex[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  uint8_t real_key = 'X';
  int ok = 0;

  uint8_t *bytes, *decrypted_bytes;
  size_t hex_len = strlen(hex), bytes_len = ENC_CalcHexDecodeBufSize(hex_len);

  EXIT_ON_NULL(bytes = malloc(bytes_len), "bytes malloc failed");
  EXIT_ON_NULL(decrypted_bytes = calloc(bytes_len + 1, 1), "decrypted_bytes malloc failed");

  ENC_HexToBytes(bytes, (unsigned char *)hex, hex_len);
  XOR_ProduceKeyScores(keys, bytes, bytes_len);

  for (size_t i = 0; i < 10; i++) {
    XOR_XORArrayWithKey(decrypted_bytes, bytes, bytes_len, &keys[i].key, 1);
    printf("%c: dev=%f -> ", keys[i].key, keys[i].penalty);
    for (size_t j = 0; j < bytes_len; j++) {
      uint8_t byte = (decrypted_bytes[j] == '\n' || decrypted_bytes[j] == '\r') ? ' ' : decrypted_bytes[j];
      printf("%c", byte);
    }

    printf("\n");
  }

  ok = (keys[0].key == real_key);

  free(bytes);
  free(decrypted_bytes);
  return ok ? 0 : 1;
}
