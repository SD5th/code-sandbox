#ifndef CAN_ENTER_CELL_H
#define CAN_ENTER_CELL_H

#include "AntMatrix.h"
#include <string>

struct CanEnterCell {
  std::function<bool(Cell const &)> function;
  std::string description;
  
  CanEnterCell(std::function<bool(Cell const &)> const & canEnterCell, const std::string& desc);
};

struct CompareSumOfDigitsWithLimit : CanEnterCell {
  int limit;
  CompareSumOfDigitsWithLimit(int const & limit);
};

struct CompareSumOfAbsNumbersWithLimit : CanEnterCell {
  int limit;
  CompareSumOfAbsNumbersWithLimit(int const & limit);
};

struct CompareSumOfSquaredNumbersWithLimit : CanEnterCell {
  int limit;
  CompareSumOfSquaredNumbersWithLimit(int const & limit);
};

#endif