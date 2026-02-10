#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <memory>  // std::unique_ptr
#include <cstddef> // std::size_t
#include <cstdlib> // std::free

class automaton {
private:
  int width_, height_; 
  int cells_amount_;

  struct aligned_deleter {
    void operator()(void* p) const { std::free(p); }
  };

  static constexpr std::size_t ALIGNMENT = 64;
  
  std::unique_ptr<bool[], aligned_deleter> cells_;
  
  #ifdef USE_NEIGHBOURS
    static constexpr int NEIGHBOURS_COUNT = 8;
    std::unique_ptr<int[], aligned_deleter> neighbours_;
    void fill_neighbours() noexcept;
    
    bool apply_rule(int cell_idx) const noexcept;
  #else
    template<bool IsInnerCell>
    bool apply_rule(int x, int y) const noexcept;
  #endif
  
public:
  automaton(int width, int height);
  ~automaton() = default;

  automaton(const automaton&) = delete;
  automaton& operator=(const automaton&) = delete;

  automaton(automaton&&) = default;
  automaton& operator=(automaton&&) = default;

  inline int get_width() const { return width_; }
  inline int get_height() const { return height_; }
  inline const bool* get_cells() const { return cells_.get(); }
  
  bool get_cell(int x, int y) const;
  void set_cell(int x, int y, bool value);
  
  void randomize_cells() noexcept;
  void clear_cells() noexcept;

  void step();
  void steps(int n_steps);
};

#endif