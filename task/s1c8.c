#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"
#include "encoding.h"
#include "lib/encoding.h"

#define INPUT_FILE "files/s1c8"

struct block_counter {
  size_t  num_blocks;
  uint8_t blocks[20][16];
};

// If the text has duplications, we will find blocks that are the same
static int countDuplications(struct block_counter *blocks) {
  size_t result = 0;

  for (size_t i = 1; i < blocks->num_blocks; i++)
    for (size_t j = i - 1; j > 0; j--)
      if (memcmp(blocks->blocks[i], blocks->blocks[j], 16) == 0)
        result++;

  return result;
}

int main(void) {
  FILE *fp;
  char hexstring[0x200 + 1] = {0};
  size_t line = 0, max_matches = 0, max_line = 0;

  EXIT_ON_NULL(fp = fopen(INPUT_FILE, "rb"), "Opening" INPUT_FILE);

  while (fgets(hexstring, 0x200, fp) != NULL) {
    size_t hexlen = strlen(hexstring), matches = 0;
    struct block_counter bc = {0};

    printf("String: %s\n", hexstring);

    ENC_HexToBytes((uint8_t *)bc.blocks, (uint8_t *)hexstring, hexlen);
    bc.num_blocks = hexlen >> (1 + 4);
    matches = countDuplications(&bc);

    if (matches > max_matches)
      max_line = line;

    line++;
  }

  printf("Line with ECB: %lu\n", max_line);
  return (max_line == 132) ? 0 : 1;
}
