#include "rules.h"

double apply_rule(const double* cells, int x, int y, int size) {
  if (x == 0)
    return 1;
  return cells[y * size + x - 1];
}