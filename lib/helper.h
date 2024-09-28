#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>

#define EXIT_ON_NULL(expr, perror_msg) { \
  if ((expr) == NULL) {                  \
    perror(perror_msg);                  \
    return 1;                            \
  }                                      \
}

#define POS_INF (1.0 / 0.0)
#define NEG_INF (- 1.0 / 0.0)

#endif
