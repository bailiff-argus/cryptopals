#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "encoding.h"

// Reencode given hexstring into base64 and make sure the result is correct
int main(void) {
  const char hexstring[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  const char correct_base64[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  char *base64 = NULL;
  uint8_t *bytes = NULL;
  size_t hex_len = strlen(hexstring), bytes_len = ENC_CalcHexDecodeBufSize(hex_len), base64_len = ENC_CalcBase64EncodeBufSize(bytes_len);
  int ok;

  bytes = malloc(bytes_len);
  if (bytes == NULL) {
    perror("bytes allocation fail:");
    return 1;
  }

  base64 = calloc(base64_len + 1, 1);
  if (base64 == NULL) {
    perror("base64 allocation fail:");
    return 1;
  }

  ENC_HexToBytes(bytes, (unsigned char *)hexstring, hex_len);
  ENC_BytesToBase64(base64, bytes, bytes_len);

  printf("Comparing:\n%s\n%s\n", base64, correct_base64);
  ok = (strncmp(base64, correct_base64, base64_len) == 0) ? 1 : 0;

  free(bytes);
  free(base64);

  return ok ? 0 : 1;
}
