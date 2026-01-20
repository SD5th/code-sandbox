#include "AntMatrix.h"

Cell::Cell():
  x(0), y(0) { };
Cell::Cell(int const & x, int const & y):
x(x), y(y) { };

bool Cell::operator==(const Cell &c) const {
  return x == c.x && y == c.y;
}

AntMatrix::AntMatrix():
  cells(std::unordered_set<Cell>()),
  width(0),
  height(0),
  upCell(Cell(0,0)),
  downCell(Cell(0,0)),
  leftCell(Cell(0,0)),
  rightCell(Cell(0,0))
{ }


bool AntMatrix::GetOrFalse(Cell const & cell) const {
  if (cells.count(cell) == 0) 
    return false;
  return true;
}

void AntMatrix::setTrue(Cell const & cell) {
  cells.emplace(cell);
}

int AntMatrix::Solve(std::function<bool(Cell const &)> const & canEnterCell, Cell const & cell0) {
  int cellsVisited = recursiveSolver(canEnterCell, cell0);
  
  upCell = cell0;
  downCell = cell0;
  leftCell = cell0;
  rightCell = cell0;

  std::unordered_set<Cell>::iterator it;
  for (it = cells.begin(); it != cells.end(); it++) {
    Cell const & cell = *it;

    if (cell.x < leftCell.x)
      leftCell = cell;
    if (rightCell.x < cell.x)
      rightCell = cell;
    if (cell.y < downCell.y)
      downCell = cell;
    if (upCell.y <  cell.y)
      upCell = cell;
  }

  height = upCell.y - downCell.y + 1;
  width = rightCell.x - leftCell.x + 1;

  return cellsVisited;
}

int AntMatrix::recursiveSolver(std::function<bool(Cell const &)> const & canEnterCell, Cell cell) {
  if (!canEnterCell(cell) || GetOrFalse(cell)) {
    return 0;
  }

  setTrue(cell);
  int cellsVisited = 1;
  cellsVisited += recursiveSolver(canEnterCell, Cell(cell.x+1, cell.y));
  cellsVisited += recursiveSolver(canEnterCell, Cell(cell.x,   cell.y+1));
  cellsVisited += recursiveSolver(canEnterCell, Cell(cell.x-1, cell.y));
  cellsVisited += recursiveSolver(canEnterCell, Cell(cell.x,   cell.y-1));
  return cellsVisited;
}