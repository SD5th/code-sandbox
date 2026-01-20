#include "../CanEnterCell.h"

static bool compareSumOfSquaredNumbersWithLimit(const Cell& cell, int const & limit);
CompareSumOfSquaredNumbersWithLimit::CompareSumOfSquaredNumbersWithLimit(int const & limit):
  CanEnterCell(
    [limit](const Cell& cell) -> bool {
      return compareSumOfSquaredNumbersWithLimit(cell, limit);
    },
    "Checking if x^2 + y^2 <= " + std::to_string(limit)
  ),
  limit(limit) 
{ }


static bool compareSumOfSquaredNumbersWithLimit(const Cell& cell, int const & limit){
  if (cell.x * cell.x + cell.y * cell.y <= limit)
    return true;
  return false;
}