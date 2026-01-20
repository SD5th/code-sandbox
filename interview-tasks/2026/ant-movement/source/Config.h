#ifndef CONFIG_H
#define CONFIG_H

#include "CanEnterCell.h"
#include <string>
#include <cstring>

struct Config {
  int xStart;
  int yStart;
  CanEnterCell* canEnterCell;

  ~Config();
};

Config LoadConfigFromArgs(int argc, char* argv[]);

#endif