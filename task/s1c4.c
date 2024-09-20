#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "helper.h"
#include "encoding.h"
#include "xor.h"

#define FILENAME "files/s1c4"

#define LINE_LEN 60

struct line {
  size_t num;
  double deviation;
  uint8_t buf[30 + 1];
  uint8_t key;
};

struct line_array {
  size_t count;
  size_t max; 
  struct line *lines;
};

int cmp_lines(const void *void_a, const void *void_b) {
  struct line *a = (struct line *)void_a, *b = (struct line *)void_b;
  return (a->deviation > b->deviation) - (a->deviation < b->deviation);
}

int main(void) {
  FILE *fp = NULL;
  char buf[60 + 1 + 1] = {0}; // + newline + \0
  uint8_t byte_buf[30 + 1] = {0}, decr_buf[30 + 1] = {0};
  size_t line_num = 0;
  key_candidate_t keys[256];

  struct line_array larr = {0, .max = 500};

  EXIT_ON_NULL(fp = fopen(FILENAME, "r"), "failed to open " FILENAME);
  EXIT_ON_NULL(larr.lines = calloc(larr.max, sizeof(struct line)), "failed to allocate larr.lines");

  while (fgets(buf , 62, fp) != NULL) {
    size_t len = 0, bytes_len;

    while ((buf[len] != '\n') && (buf[len] != '\0'))
      len++;

    bytes_len = ENC_CalcHexDecodeBufSize(len);
    ENC_HexToBytes(byte_buf, (unsigned char *)buf, len);
    XOR_ProduceKeyScores(keys, byte_buf, bytes_len);

    for (size_t i = 0; (i < 256); i++) {

      XOR_XORArrayWithKey(decr_buf, byte_buf, bytes_len, &keys[i].key, 1);

      larr.lines[larr.count].num = line_num;
      larr.lines[larr.count].deviation = keys[i].penalty;
      larr.lines[larr.count].key = keys[i].key;
      memcpy(larr.lines[larr.count].buf, decr_buf, bytes_len);

      if (++larr.count >= larr.max) {
        larr.max += 500;
        larr.lines = realloc(larr.lines, larr.max * sizeof(struct line));
      }
    }

    line_num++;
  }

  qsort(larr.lines, larr.count, sizeof(struct line), cmp_lines);

  for (size_t i = 0; i < 25; i++) {
    struct line *line = larr.lines + i;
    printf("%lu %.2x: dev=%f -> ", line->num, line->key, line->deviation);
    for (size_t j = 0; j < 30; j++)
      printf("%c", isalnum(line->buf[j]) ? line->buf[j] : ' ');
    printf("\n");
  }

  free(larr.lines);
  return 0;
}
