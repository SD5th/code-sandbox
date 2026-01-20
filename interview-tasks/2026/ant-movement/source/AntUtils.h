#ifndef ANT_UTILS_H
#define ANT_UTILS_H

#include <string>
#include "AntMatrix.h"

std::string DrawAntMatrix(AntMatrix const & antMatrix);

void GenerateIndexHTML(AntMatrix const & antMatrix, 
  Cell startingCell,
  std::string isCellReachableFunction, 
  int cellsVisited,
  std::string imageFileName
);

#endif