#include <cstring>
#include "rules.h"

#ifdef __cplusplus
extern "C" {
#endif

void calculate_step(double* cells, int size) {
  double* new_cells = new double[size * size];
    
  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      new_cells[y * size + x] = apply_rule(cells, x, y, size);
    }
  }
  
  std::memcpy(cells, new_cells, size * size * sizeof(double));
  
  delete[] new_cells;
}

#ifdef __cplusplus
}
#endif