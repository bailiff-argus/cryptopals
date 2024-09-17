#ifndef ENCODING_H
#define ENCODING_H

#include <stdint.h>
#include <stddef.h>

enum enc_error {
  ENC_OK,
  ENC_FAIL
};


/// @brief decode a hex string of length hex_len into bytes and write the result into dst.
/// @return ENC_OK -- successful decode
enum enc_error ENC_HexToBytes(uint8_t *dst, const unsigned char *hex, size_t hex_len);

/// @brief encode bytes_len number of bytes into hex and write the result into dst.
/// @return ENC_OK -- successful encode
enum enc_error ENC_BytesToHex(char *dst, const uint8_t *bytes, size_t bytes_len);

/// @brief Case-insensitively check if len bytes of hex_a match hex_b
/// @return ENC_OK -- hexes match, ENC_FAIL -- hexes are different
enum enc_error ENC_HexesMatch(const char *hex_a, const char *hex_b, size_t len);

/// @brief decode a base64 string of length hex_len into bytes and write the result into dst.
/// @return ENC_OK -- successful encode
enum enc_error ENC_Base64ToBytes(uint8_t *dst, const unsigned char *base64, size_t base64_len);

/// @brief encode bytes_len number of bytes into base64 and write the result into dst.
/// @return ENC_OK -- successful encode
enum enc_error ENC_BytesToBase64(char *dst, const uint8_t *bytes, size_t bytes_len);

/// @brief Calculates the buffer size required to decode a hex string to bytes.
static inline size_t ENC_CalcHexDecodeBufSize(size_t hex_len) {
  return hex_len / 2;
}

/// @brief Calculates the buffer size required to encode bytes into a hex string, excluding the '\0' terminator.
static inline size_t ENC_CalcHexEncodeBufSize(size_t bytes_len) {
  return bytes_len * 2;
}

/// @brief Calculates the buffer size required to encode bytes into a Base64-encoded string, excluding the '\0' terminator.
size_t ENC_CalcBase64EncodeBufSize(size_t bytes_len);

/// @brief Calculates the buffer size required to decode a Base64-encoded string into bytes.
size_t ENC_CalcBase64DecodeBufSize(const char *base64_string);


#endif // !ENCODING_H
