#include "automaton.h"
#include <cstdlib>  // std::rand
#include <time.h>
#include <omp.h>
#include <stdexcept> // std::out_of_range
	

automaton::automaton(int width, int height): 
  width_(width), 
  height_(height),
  cells_amount_(width * height),
  cells_(new bool[cells_amount_]) 
{ 
  clear_cells(); 
  std::srand(static_cast<unsigned>(time(nullptr)));
}

static inline int get_idx(int x, int y, int width) { return y * width + x; }

static inline bool check_boundaries(int x, int y, int width, int height) { return ((0 <= x && x < width) && (0 <= y && y < height )); }

bool automaton::get_cell(int x, int y) const {
  if (check_boundaries(x, y, width_, height_))
    return cells_[get_idx(x, y, width_)];
  throw std::out_of_range("x, y out of boundaries");
}

void automaton::set_cell(int x, int y, bool value) {
  if (check_boundaries(x, y, width_, height_))
    cells_[get_idx(x, y, width_)] = value;
}

void automaton::randomize_cells() {
  for (int i = 0; i < cells_amount_; i++)
    cells_[i] = std::rand() % 2;
}

void automaton::clear_cells() {
  for (int i = 0; i < cells_amount_; i++)
    cells_[i] = false;
}

void automaton::step() {
  bool* cells_new = nullptr;
  try {
    cells_new = new bool[cells_amount_];
  } catch (const std::bad_alloc&) {
    throw;
  }
  
  #pragma omp parallel for  
  for (int i = 0; i < cells_amount_; i++)
    cells_new[i] = apply_rule(i);
  
  cells_ = std::unique_ptr<bool[]>(cells_new);
}

void automaton::steps(int n_steps) {
  for (int i = 0; i < n_steps; i++)
    step();
}

static inline int mod(int a, int b) {
  return (a % b + b) % b;
}

int automaton::get_neighbour_cell_idx(int cell_idx, int dx, int dy) const
{
  int x = cell_idx % width_; 
  int y = cell_idx / width_;
  
  int x_new = mod(x + dx, width_);
  int y_new = mod(y + dy, height_);
  
  return get_idx(x_new, y_new, width_);
}

bool automaton::apply_rule(int cell_idx) const {
  unsigned counter = 0;
  
  const int dx[8] = {-1, -1, -1,  0,  0,  1,  1,  1};
  const int dy[8] = {-1,  0,  1, -1,  1, -1,  0,  1};

  for (int i = 0; i < 8; i++)
    if (cells_[get_neighbour_cell_idx(cell_idx, dx[i], dy[i])]) 
      counter++;
  
  if (cells_[cell_idx]) {
    if (counter <= 1) // underpopulation
    return false;   
    if (4 <= counter) // overpopulation
    return false;   
    return true;      // happy survival
  }
  if (counter == 3)   // reproduction
  return true;      
  return false;       // no reproduction
}