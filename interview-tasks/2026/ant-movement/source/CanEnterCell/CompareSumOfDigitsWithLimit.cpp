#include "../CanEnterCell.h"

static bool compareSumOfDigitsWithLimit(const Cell& cell, int const & limit);
CompareSumOfDigitsWithLimit::CompareSumOfDigitsWithLimit(int const & limit):
  CanEnterCell(
    [limit](const Cell& cell) -> bool {
      return compareSumOfDigitsWithLimit(cell, limit);
    },
    "Checking if sum of digits of x and y <= " + std::to_string(limit)
  ),
  limit(limit)
{ }

static int sumOfDigits(int x) {
  int sum = 0;
  x = std::abs(x);
  
  while (x > 0) {
    sum += x % 10;
    x /= 10;
  }
  return sum;
}

bool compareSumOfDigitsWithLimit(const Cell& cell, int const & limit){
  if (sumOfDigits(cell.x) + sumOfDigits(cell.y) <= limit)
    return true;
  return false;
}

