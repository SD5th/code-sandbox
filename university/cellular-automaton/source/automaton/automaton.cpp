#include "automaton.h"

#include <memory>    // std::unique_ptr
#include <cstdlib>   // std::aligned_alloc
#include <cstring>   // std::memset
#include <random>    // std::mt19937, std::bernoulli_distribution
#include <new>       // std::bad_alloc
#include <stdexcept> // std::invalid_argument, std::out_of_range

#ifdef _OPENMP
#include <omp.h>     // omp_get_thread_num
#endif

namespace {
  inline bool check_boundaries(int x, int y, int width, int height) { 
    return ((0 <= x) && (x < width) && (0 <= y) && (y < height )); 
  }
  
  inline int get_idx(int x, int y, int width) { 
    return y * width + x; 
  }

  inline bool conways_game_of_life(bool cell, int live_count){
    return 
    ( cell && ((live_count == 2) || (live_count == 3))) || 
    (!cell &&   live_count == 3);
  
    // alive cell stays alive if there are 2 or 3 alive cells near
    // dead cell turns alive if there are 3 alive cells near
    // in other ways cell turns/stays dead
  } 
}

automaton::automaton(int width, int height): 
  width_(width), 
  height_(height),
  cells_amount_(width * height) 
{
  if (width <= 3 || height <= 3) {
    throw std::invalid_argument("Width and height must be greater than 3");
  }

  void* cells_ptr = std::aligned_alloc(ALIGNMENT, cells_amount_ * sizeof(bool));
  if (!cells_ptr) throw std::bad_alloc();
  cells_.reset(static_cast<bool*>(cells_ptr));
  
  #ifdef USE_NEIGHBOURS
    void* neighbours_ptr = std::aligned_alloc(ALIGNMENT, cells_amount_ * NEIGHBOURS_COUNT * sizeof(int));
    if (!neighbours_ptr) throw std::bad_alloc();
    neighbours_.reset(static_cast<int*>(neighbours_ptr));
    
    fill_neighbours();
  #endif
    
  clear_cells(); 
}

#ifdef USE_NEIGHBOURS
void automaton::fill_neighbours() noexcept {
  #ifdef _OPENMP
  #pragma omp parallel for collapse(2)
  #endif
  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      int idx = y * width_ + x;
      int* nb = &neighbours_[idx * NEIGHBOURS_COUNT];
      
      nb[0] = (y == 0 ? height_ - 1 : y - 1) * width_ + (x == 0 ? width_ - 1 : x - 1);
      nb[1] = (y == 0 ? height_ - 1 : y - 1) * width_ + x;
      nb[2] = (y == 0 ? height_ - 1 : y - 1) * width_ + (x == width_ - 1 ? 0 : x + 1);
      nb[3] = y                              * width_ + (x == 0 ? width_ - 1 : x - 1);
      nb[4] = y                              * width_ + (x == width_ - 1 ? 0 : x + 1);
      nb[5] = (y == height_ - 1 ? 0 : y + 1) * width_ + (x == 0 ? width_ - 1 : x - 1);
      nb[6] = (y == height_ - 1 ? 0 : y + 1) * width_ + x;
      nb[7] = (y == height_ - 1 ? 0 : y + 1) * width_ + (x == width_ - 1 ? 0 : x + 1);
    }
  }
}
#endif

bool automaton::get_cell(int x, int y) const {
  if (check_boundaries(x, y, width_, height_)) [[likely]] {
    return cells_[get_idx(x, y, width_)];
  }
  [[unlikely]] throw std::out_of_range("x, y out of boundaries");
}

void automaton::set_cell(int x, int y, bool value) {
  if (check_boundaries(x, y, width_, height_)) [[likely]] {
    cells_[get_idx(x, y, width_)] = value;
    return;
  }
  [[unlikely]] throw std::out_of_range("x, y out of boundaries");
}

void automaton::randomize_cells() noexcept {
  #ifdef _OPENMP
    #pragma omp parallel 
    {
      static thread_local std::mt19937 rng(std::random_device{}());
      static thread_local bool initialized = false;
      
      if (!initialized) {
        std::random_device rd;
        rng.seed(rd() ^ static_cast<unsigned>(omp_get_thread_num()));
        initialized = true;
      }
      
      std::bernoulli_distribution dist(0.5);
      
      #pragma omp for 
      for (int i = 0; i < cells_amount_; i++) {
        cells_[i] = dist(rng);
      }
    }
  #else
    static std::mt19937 rng(std::random_device{}());
    static bool initialized = false;
    
    if (!initialized) {
      std::random_device rd;
      rng.seed(rd());
      initialized = true;
    }
    
    std::bernoulli_distribution dist(0.5);
    
    for (int i = 0; i < cells_amount_; i++) {
      cells_[i] = dist(rng);
    }
  #endif
}

void automaton::clear_cells() noexcept {
  bool* cells_ptr = cells_.get();
  if (cells_ptr && cells_amount_ > 0)
    std::memset(cells_ptr, 0, cells_amount_ * sizeof(bool));
}

void automaton::step() {
  void* cells_new_ptr = std::aligned_alloc(ALIGNMENT, cells_amount_ * sizeof(bool));
  if (!cells_new_ptr) throw std::bad_alloc();
  std::unique_ptr<bool[], aligned_deleter> cells_new(static_cast<bool*>(cells_new_ptr));

  #ifdef USE_NEIGHBOURS
    #ifdef _OPENMP
      #pragma omp parallel for
    #endif
    for (int i = 0; i < cells_amount_; i++)
      cells_new[i] = apply_rule(i);
  
  #else
    #ifdef _OPENMP
      #pragma omp parallel for collapse(2)
    #endif
    for (int y = 1; y < height_ - 1; y++) {
      for (int x = 1; x < width_ - 1; x++) {
        int cell_idx = get_idx(x, y, width_);
        cells_new[cell_idx] = apply_rule<true>(x, y);
      }
    }

    #ifdef _OPENMP
      #pragma omp parallel for
    #endif
    for (int x = 0; x < width_; x++) {
      int cell_idx1 = get_idx(x, 0, width_);
      cells_new[cell_idx1] = apply_rule<false>(x, 0);

      int cell_idx2 = get_idx(x, height_-1, width_);
      cells_new[cell_idx2] = apply_rule<false>(x, height_-1);
    }

    #ifdef _OPENMP
      #pragma omp parallel for
    #endif
    for (int y = 1; y < height_ - 1; y++) {
      int cell_idx1 = get_idx(0, y, width_);
      cells_new[cell_idx1] = apply_rule<false>(0, y);

      int cell_idx2 = get_idx(width_-1, y, width_);
      cells_new[cell_idx2] = apply_rule<false>(width_-1, y);
    }


  #endif

  cells_.swap(cells_new);
}

void automaton::steps(int n_steps) {
  for (int i = 0; i < n_steps; i++)
    step();
}

#ifdef USE_NEIGHBOURS
  bool automaton::apply_rule(int cell_idx) const noexcept {

    const int* nb = &neighbours_[cell_idx * NEIGHBOURS_COUNT];

    int live_count = 
      cells_[nb[0]] + cells_[nb[1]] + cells_[nb[2]] + cells_[nb[3]] + 
      cells_[nb[4]] + cells_[nb[5]] + cells_[nb[6]] + cells_[nb[7]];
    
    return conways_game_of_life(cells_[cell_idx], live_count);
  }
#else
template<bool IsInnerCell>
bool automaton::apply_rule(int x, int y) const noexcept {
  int live_count = 0;
  
  int y_above, y_below, x_left, x_right;
  if constexpr (IsInnerCell) {
    y_above = (y - 1) * width_;  
    y_below = (y + 1) * width_;  
    
    x_left  = x - 1;
    x_right = x + 1;
  } else {
    y_above = (y == 0 ? height_ - 1 : y - 1) * width_;
    y_below = (y == height_ - 1 ? 0 : y + 1) * width_;
    x_left  = x == 0 ? width_ - 1 : x - 1;
    x_right = x == width_ - 1 ? 0 : x + 1;
  }
  
  int curr_row = y * width_;
  
  live_count += cells_[y_above + x_left];   
  live_count += cells_[y_above + x];        
  live_count += cells_[y_above + x_right];  
  
  live_count += cells_[curr_row + x_left];  
  live_count += cells_[curr_row + x_right]; 
  
  live_count += cells_[y_below + x_left];   
  live_count += cells_[y_below + x];        
  live_count += cells_[y_below + x_right];  
  
  bool cell = cells_[curr_row + x];
  
  return conways_game_of_life(cell, live_count);
}
#endif