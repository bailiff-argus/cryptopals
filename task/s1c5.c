#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "helper.h"
#include "encoding.h"
#include "lib/encoding.h"
#include "lib/xor.h"

int main(void) {
  unsigned char text[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
  unsigned char expected_result[] = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
  unsigned char key[] = "ICE";

  size_t text_len = strlen((const char *)text), hex_len = ENC_CalcHexEncodeBufSize(text_len);
  unsigned char *enc_hex;
  uint8_t *enc_bytes;
  int ret;

  EXIT_ON_NULL(enc_bytes = calloc(text_len + 1, sizeof(char)), "bytes malloc");
  EXIT_ON_NULL(enc_hex = calloc(hex_len + 1, sizeof(char)), "hex malloc");

  XOR_XORArrayWithKey(enc_bytes, text, text_len, key, strlen((char *)key));
  ENC_BytesToHex((char *)enc_hex, enc_bytes, text_len);

  printf("%s\n%s\n", enc_hex, expected_result);

  ret = ENC_HexesMatch((char *)enc_hex, (char *)expected_result, hex_len) == ENC_OK ? 0 : 1;

  free(enc_bytes);
  free(enc_hex);
  return ret;
}
