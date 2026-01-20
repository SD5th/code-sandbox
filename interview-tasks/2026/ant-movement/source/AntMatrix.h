#ifndef ANT_MATRIX_H
#define ANT_MATRIX_H

#include <unordered_set>
#include <functional>

class Cell {
public:
  int x; int y;

  Cell();
  Cell(int const & x, int const & y);

  bool operator==(Cell const & cell) const;
};
namespace std {
  template<>
  struct hash<Cell> {
    size_t operator()(const Cell& c) const {
      return hash<int>()(c.x) ^ (hash<int>()(c.y) << 1);
    }
  };
}

class AntMatrix {
private:
  std::unordered_set<Cell> cells;
  int recursiveSolver(std::function<bool(Cell const &)> const & canEnterCell, Cell cell);
  void setTrue(Cell const & cell);
  
public:
  int height;
  int width;
  Cell upCell;
  Cell downCell;
  Cell leftCell;
  Cell rightCell;

  AntMatrix();
  bool GetOrFalse(Cell const & cell) const;

  int Solve(std::function<bool(Cell const &)> const & canEnterCell, Cell const & cell0);
};

#endif