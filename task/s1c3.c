#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encoding.h"
#include "lib/xor.h"
#include "xor.h"

int main(void) {
  key_candidate_t keys[256];
  const char hex[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  uint8_t real_key = 'X';

  int key_found = 0;
  uint8_t *bytes, *decrypted_bytes;
  size_t hex_len = strlen(hex), bytes_len = ENC_CalcHexDecodeBufSize(hex_len);

  bytes = malloc(bytes_len);
  if (bytes == NULL)
    return 1;

  decrypted_bytes = calloc(bytes_len + 1, 1);
  if (decrypted_bytes == NULL)
    return 1;


  ENC_HexToBytes(bytes, (unsigned char *)hex, hex_len);
  XOR_ProduceKeyScores(keys, bytes, bytes_len);

  for (size_t i = 0; i < 10; i++) {
    key_found |= (keys[i].key == real_key);

    XOR_XORArrayWithKey(decrypted_bytes, bytes, bytes_len, &keys[i].key, 1);
    printf("%c: dev=%f -> ", keys[i].key, keys[i].penalty);
    for (size_t j = 0; j < bytes_len; j++) {
      uint8_t byte = (decrypted_bytes[j] == '\n' || decrypted_bytes[j] == '\r') ? ' ' : decrypted_bytes[j];
      printf("%c", byte);
    }

    printf("\n");
  }

  free(bytes);
  free(decrypted_bytes);
  return (key_found == 1) ? 0 : 1;
}
