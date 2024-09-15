#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "encoding.h"
#include "encoding.h"
#include "xor.h"

#define attempt_malloc(var, size_bytes) {        \
if ((var = calloc(size_bytes + 1, 1)) == NULL) { \
    perror("malloc fail:");                      \
    return -1;                                   \
  }                                              \
}

// Fixed XOR: take two equal-length buffers and produce their XOR combination
int main(void) {
  const char hex_a[] = "1c0111001f010100061a024b53535009181c";
  const char hex_b[] = "686974207468652062756c6c277320657965";
  const char hex_result[] = "746865206b696420646f6e277420706c6179";

  size_t hex_len = strlen(hex_a), bytes_len = ENC_CalcHexDecodeBufSize(hex_len);

  uint8_t *bytes_a, *bytes_b, *bytes_xor;
  char *hex_xor;

  int ok;

  attempt_malloc(bytes_a, bytes_len);
  attempt_malloc(bytes_b, bytes_len);
  attempt_malloc(bytes_xor, bytes_len);
  attempt_malloc(hex_xor, hex_len);

  ENC_HexToBytes(bytes_a, (unsigned char *)hex_a, hex_len);
  ENC_HexToBytes(bytes_b, (unsigned char *)hex_b, hex_len);
  XOR_XORArrays(bytes_xor, bytes_a, bytes_b, bytes_len);
  ENC_BytesToHex(hex_xor, bytes_xor, bytes_len);

  printf("%s\n%s\n", hex_result, hex_xor);
  ok = (ENC_HexesMatch(hex_result, hex_xor, hex_len) == ENC_OK);

  free(bytes_a);
  free(bytes_b);
  free(bytes_xor);
  free(hex_xor);

  return ok ? 0 : 1;
}
