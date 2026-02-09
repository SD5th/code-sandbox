#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <memory>

class automaton {
private:
  int width_, height_, cells_amount_;
  std::unique_ptr<bool[]> cells_;
  
  int get_neighbour_cell_idx(int cell_idx, int dx, int dy) const;
  bool apply_rule(int cell_idx) const;
  
public:
  automaton(int width, int height);
  
  inline int get_width() const { return width_; }
  inline int get_height() const { return height_; }
  inline const bool* get_cells() const { return cells_.get(); }
  
  bool get_cell(int x, int y) const;
  
  void set_cell(int x, int y, bool value);
  void randomize_cells();
  void clear_cells();

  void step();
  void steps(int n_steps);
};

#endif