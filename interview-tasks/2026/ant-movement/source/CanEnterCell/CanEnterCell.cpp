#include "../CanEnterCell.h"

CanEnterCell::CanEnterCell(std::function<bool (const Cell &)> const & func, std::string const & desc):
  function(func),
  description(desc)
{}
