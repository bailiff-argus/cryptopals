#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "encoding.h"

static const char hex_encode_map[] = "0123456789ABCDEF";
static const char hex_decode_map[] = {
  ['0'] = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  ['A'] = 10, 11, 12, 13, 14, 15,
  ['a'] = 10, 11, 12, 13, 14, 15, 
};

static const char base64_encode_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const int base64_decode_map[] = {
  ['+'] = 62,
  ['/'] = 63,
  ['0'] = 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
  ['A'] =  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
  ['a'] = 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
  ['='] = 0,
};

enum enc_error ENC_HexToBytes(uint8_t *dst, const unsigned char *hex, size_t hex_len) {
  size_t bytes_len = ENC_CalcHexDecodeBufSize(hex_len);
  for (size_t i = 0; i < bytes_len; i++)
    dst[i] = hex_decode_map[hex[i*2]] << 4 | hex_decode_map[hex[i*2 + 1]];

  return ENC_OK;
}

enum enc_error ENC_BytesToHex(char *dst, const uint8_t *bytes, size_t bytes_len) {
  for (size_t i = 0; i < bytes_len; i++) {
    dst[i*2]     = hex_encode_map[ bytes[i] >> 4   ];
    dst[i*2 + 1] = hex_encode_map[ bytes[i] & 0x0f ];
  }

  return ENC_OK;
}

enum enc_error ENC_HexesMatch(const char *hex_a, const char *hex_b, size_t len) {
  for (size_t i = 0; i < len; i++)
    if (tolower(hex_a[i]) != tolower(hex_b[i]))
      return ENC_FAIL;

  return ENC_OK;
}

size_t ENC_CalcBase64EncodeBufSize(size_t bytes_len) {
  return ((bytes_len + 2) / 3) * 4;
}

size_t ENC_CalcBase64DecodeBufSize(const char *base64_string) {
  size_t bytes_len;

  bytes_len = strlen(base64_string) * 3 / 4;
  for (size_t end = bytes_len - 1; base64_string[end] == '='; end--)
      bytes_len--;

  return bytes_len;
}

// Base64 char is 6 bits, so we can encode in blocks: 3 bytes are 4 Base64 digits
enum enc_error ENC_BytesToBase64(char *dst, const uint8_t *bytes, size_t bytes_len) {
  size_t i = 0, j = 0, buf;

  while (i < bytes_len) {
    buf  = bytes[i] << 16;
    buf |= (++i < bytes_len) ? (bytes[i] << 8) : 0;
    buf |= (++i < bytes_len) ?  bytes[i] : 0;
    i++;

    dst[j++] = base64_encode_map[(buf >> 18) & 0x3F];
    dst[j++] = base64_encode_map[(buf >> 12) & 0x3F];
    dst[j++] = (i > (bytes_len + 1)) ? '=' : base64_encode_map[(buf >> 6) & 0x3F];
    dst[j++] = (i >  bytes_len    )  ? '=' : base64_encode_map[ buf       & 0x3F];
  }

  return ENC_OK;
}

// base64_len is guaranteed to be %4, so we can decode it in blocks
enum enc_error ENC_Base64ToBytes(uint8_t *dst, const unsigned char *base64, size_t base64_len) {
  uint32_t i = 0, j = 0, buf;
  while (j < base64_len) {
    buf  = base64_decode_map[base64[j++]] << 18;
    buf |= base64_decode_map[base64[j++]] << 12;
    buf |= base64_decode_map[base64[j++]] << 6;
    buf |= base64_decode_map[base64[j++]];

    dst[i++] = (buf >> 16) & 0xFF;
    if (base64[j-2] != '=') dst[i++] = (buf >>  8) & 0xFF;
    if (base64[j-1] != '=') dst[i++] = buf & 0xFF;
  }

  return ENC_OK;
}
