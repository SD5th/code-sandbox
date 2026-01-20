#include "../CanEnterCell.h"

static bool compareSumOfAbsNumbersWithLimit(const Cell& cell, int const & limit);
CompareSumOfAbsNumbersWithLimit::CompareSumOfAbsNumbersWithLimit(int const & limit):
  CanEnterCell(
    [limit](const Cell& cell) -> bool {
      return compareSumOfAbsNumbersWithLimit(cell, limit);
    },
    "Checking if |x| + |y| <= " + std::to_string(limit)
  ),
  limit(limit) 
{ }


static bool compareSumOfAbsNumbersWithLimit(const Cell& cell, int const & limit){
  if (std::abs(cell.x) + std::abs(cell.y) <= limit)
    return true;
  return false;
}