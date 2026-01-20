#include <iostream>
#include "source/AntMatrix.h"
#include "source/AntUtils.h"
#include "source/Config.h"

int main(int argc, char* argv[]) {
  Config config = LoadConfigFromArgs(argc, argv);

  Cell startingCell = Cell(config.xStart, config.yStart);
  CanEnterCell* canEnterCell = config.canEnterCell;
  
  AntMatrix antMatrix;
  int cellsVisited = antMatrix.Solve(canEnterCell->function, startingCell);

  std::string imageFileName = DrawAntMatrix(antMatrix);

  GenerateIndexHTML(antMatrix, startingCell, canEnterCell->description, cellsVisited, imageFileName);
}