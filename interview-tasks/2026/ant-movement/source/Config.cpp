#include "Config.h"

Config::~Config() {
  if (canEnterCell != nullptr) 
    delete canEnterCell;
}

Config LoadConfigFromArgs(int argc, char* argv[]) {
  Config config;

  bool xStartProvided = false;
  bool yStartProvided = false;

  std::string functionName;
  bool functionProvided = false;
  
  int limitBuffer;
  bool limitProvided = false;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--x-start") == 0 && i + 1 < argc) {
      config.xStart = std::atoi(argv[++i]);
      xStartProvided = true;
    } else if (strcmp(argv[i], "--y-start") == 0 && i + 1 < argc) {
      config.yStart = std::atoi(argv[++i]);
      yStartProvided = true;
    } else if (strcmp(argv[i], "--limit") == 0 && i + 1 < argc) {
      limitBuffer = std::atoi(argv[++i]);
      limitProvided = true;
    } else if (strcmp(argv[i], "--function") == 0 && i + 1 < argc) {
      functionName = argv[++i];
      functionProvided = true;
    }
  }
  
  if (!xStartProvided)
    config.xStart = 1000;
  
  if (!yStartProvided)
    config.yStart = 1000;

  if (!functionProvided) {
    config.canEnterCell = new CompareSumOfDigitsWithLimit(25);
  } else {
    if (functionName == "SUM_OF_DIGITS") {
      int limit = limitProvided ? limitBuffer : 25;
      config.canEnterCell = new CompareSumOfDigitsWithLimit(limit);
    } else if (functionName == "SUM_OF_ABS_NUMBERS") {
      int limit = limitProvided ? limitBuffer : 500;
      config.canEnterCell = new CompareSumOfAbsNumbersWithLimit(limit);
    } else if (functionName == "SUM_OF_SQUARED_NUMBERS") {
      int limit = limitProvided ? limitBuffer : 600;
      config.canEnterCell = new CompareSumOfSquaredNumbersWithLimit(limit);
    } else 
      config.canEnterCell = new CompareSumOfDigitsWithLimit(25);
  }

  return config;
}