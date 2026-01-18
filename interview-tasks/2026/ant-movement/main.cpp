#include <iostream>

using namespace std;

int sumOfDigits(int x) {
  int sum = 0;
  while (x > 0) {
    sum += x % 10;
    x /= 10;
  }
  return sum;
}

int cellValue(int x, int y) {
  return sumOfDigits(x) + sumOfDigits(y);
}

int RecursiveCount(bool matrix[2001][2001], int x, int y) {
  if (cellValue(x, y) > 25 || matrix[x][y])
    return 0;
  
  matrix[x][y] = true;
  int result = 1;

  result += RecursiveCount(matrix, x, y + 1);
  result += RecursiveCount(matrix, x, y - 1);
  result += RecursiveCount(matrix, x + 1, y);
  result += RecursiveCount(matrix, x - 1, y);

  return result;
}

int main() {
  bool matrix[2001][2001];
  for (int x = 0; x <= 2000; x++)
    for (int y = 0; y <= 2000; y++)
      matrix[x][y] = false;

  int visitedCells = RecursiveCount(matrix, 1000, 1000);

  std::cout << "Ant can visit " << visitedCells << " different squares.\n";
}