#ifndef FREQ_H
#define FREQ_H

#include <stdint.h>
#include <stdlib.h>

enum freq_err {
  FREQ_OK,
  FREQ_FAIL,
};

enum freq_table_kind {
  FREQ_TABLE_LINGUISTIC, // when processing, only consider alphabet
  FREQ_TABLE_BYTE,       // when processing, consider all bytes
};

typedef struct freq_table_t {
  enum freq_table_kind kind; // picks strategy for counting bytes: either all or a subset
  size_t total_processed;    // this and the next field are private fields for computing freq_table
  size_t occurences[256];
  double freq_table[256];    // the table itself
} freq_table_t;

/// @brief Set initial values in a freshly allocated frequency table.
enum freq_err FREQ_InitializeTable(freq_table_t *table, enum freq_table_kind kind);

/// @brief Count the occurences of different values in bytes and add those values to the table.
enum freq_err FREQ_UpdateTableWith(freq_table_t *table, const uint8_t *bytes, size_t bytes_len);

/// @brief Calculate the deviation of distributions relative to each other.
/// @param table_b if NULL, take the frequency table of the English language.
double FREQ_CalcAverageDev(const freq_table_t *table_a, const freq_table_t *table_b);

/// @brief Print the frequency table into stdout.
/// @param cols The number of columns to print the table in.
/// @param ignore_zero If non-zero, do not include the characters with 0 occurences.
void FREQ_PrintFreqTable(const freq_table_t *table, size_t cols, int ignore_zero);

#endif // FREQ_H
