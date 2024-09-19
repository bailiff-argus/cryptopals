#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "freq.h"

static const freq_table_t english_freq = {
  .kind = FREQ_TABLE_LINGUISTIC,
  .total_processed = 0,
  .occurences = {0},
  .freq_table = {
    ['A'] = 0.0651738, ['B'] = 0.0124248, ['C'] = 0.0217339, ['D'] = 0.0349835, ['E'] = 0.1041442, ['F'] = 0.0197881, ['G'] = 0.0158610,
    ['H'] = 0.0492888, ['I'] = 0.0558094, ['J'] = 0.0009033, ['K'] = 0.0050529, ['L'] = 0.0331490, ['M'] = 0.0202124, ['N'] = 0.0564513,
    ['O'] = 0.0596302, ['P'] = 0.0137645, ['Q'] = 0.0008606, ['R'] = 0.0497563, ['S'] = 0.0515760, ['T'] = 0.0729357, ['U'] = 0.0225134,
    ['V'] = 0.0082903, ['W'] = 0.0171272, ['X'] = 0.0013692, ['Y'] = 0.0145984, ['Z'] = 0.0007836, [' '] = 0.1918182
  },
};

static int islang(char c) {
  return isalpha(c) || (c == ' ');
}

enum freq_err validateTable(const freq_table_t *table) {
  switch (table->kind) {
    case FREQ_TABLE_LINGUISTIC: case FREQ_TABLE_BYTE: break;
    default: return FREQ_FAIL;
  }

  return FREQ_OK;
}

enum freq_err FREQ_InitializeTable(freq_table_t *table, enum freq_table_kind kind) {
  memset(table, 0, sizeof(freq_table_t));
  table->kind = kind;
  return FREQ_OK;
}

enum freq_err FREQ_UpdateTableWith(freq_table_t *table, const uint8_t *bytes, size_t bytes_len) {
  uint8_t byte;
  int ret;

  if ((ret = validateTable(table)) != FREQ_OK)
    return ret;

  for (size_t i = 0; i < bytes_len; i++) {
    byte = toupper(bytes[i]); // to make sure we don't have separate frequencies for the same letter
    if ((table->kind == FREQ_TABLE_LINGUISTIC) && (!islang(byte)))
      continue;

    table->total_processed++;
    table->occurences[byte]++;
  }

  if (table->total_processed == 0) // in case of a lingustic table, it means we failed to find any letters
    return FREQ_FAIL;

  for (size_t i = 0; i < 256; i++)
    table->freq_table[i] = (double)table->occurences[i] / (double)table->total_processed;

  return FREQ_OK;
}

double FREQ_CalcAverageDev(const freq_table_t *table_a, const freq_table_t *table_b) {
  double deviation = 0, diff;

  if (table_b == NULL)
    table_b = &english_freq;

  if ((validateTable(table_a) != FREQ_OK) || (validateTable(table_b) != FREQ_OK))
    return -1.0;

  for (size_t i = 0; i < 256; i++) {
    diff = table_a->freq_table[i] - table_b->freq_table[i];
    deviation += diff * diff;
  }

  return sqrt(deviation / 256.0);
}

void FREQ_PrintFreqTable(const freq_table_t *table, size_t cols, int ignore_zero) {
  size_t printed = 0;
  for (int i = 0; i < 256; i++) {
    if (ignore_zero && (table->freq_table[i] == 0))
      continue;

    printf("%.2x: %f    ", i, table->freq_table[i]);
    if (++printed % cols == 0)
      printf("\n");
  }

  printf("\n");
}
