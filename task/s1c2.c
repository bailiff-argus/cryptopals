#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "helper.h"
#include "encoding.h"
#include "xor.h"

// Fixed XOR: take two equal-length buffers and produce their XOR combination
int main(void) {
  const char hex_a[] = "1c0111001f010100061a024b53535009181c";
  const char hex_b[] = "686974207468652062756c6c277320657965";
  const char hex_result[] = "746865206b696420646f6e277420706c6179";

  size_t hex_len = strlen(hex_a), bytes_len = ENC_CalcHexDecodeBufSize(hex_len);

  uint8_t *bytes_a, *bytes_b, *bytes_xor;
  char *hex_xor;

  int ok;

  EXIT_ON_NULL(bytes_a   = calloc(bytes_len + 1, 1), "malloc fail");
  EXIT_ON_NULL(bytes_b   = calloc(bytes_len + 1, 1), "malloc fail");
  EXIT_ON_NULL(bytes_xor = calloc(bytes_len + 1, 1), "malloc fail");
  EXIT_ON_NULL(hex_xor   = calloc(hex_len + 1, 1),   "malloc fail");

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
