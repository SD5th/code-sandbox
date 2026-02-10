#include "interface.h"

#include "automaton.h"
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif

static inline automaton* to_automaton(automaton_handle handle) {
  return static_cast<automaton*>(handle);
}

automaton_handle create_automaton(int width, int height) { return new automaton(width, height); }
void destroy_automaton(automaton_handle handle) { delete to_automaton(handle); }

int automaton_get_width(automaton_handle handle) { return to_automaton(handle)->get_width(); }
int automaton_get_height(automaton_handle handle) { return to_automaton(handle)->get_height(); }
const bool* automaton_get_cells(automaton_handle handle) { return to_automaton(handle)->get_cells(); }

bool automaton_get_cell(automaton_handle handle, int x, int y) { return to_automaton(handle)->get_cell(x, y); }
void automaton_set_cell(automaton_handle handle, int x, int y, bool value) { to_automaton(handle)->set_cell(x, y, value); }

void automaton_randomize(automaton_handle handle) { to_automaton(handle)->randomize_cells(); }
void automaton_clear(automaton_handle handle) { to_automaton(handle)->clear_cells(); }

void automaton_step(automaton_handle handle) { to_automaton(handle)->step(); }
void automaton_steps(automaton_handle handle, int n_steps) { to_automaton(handle)->steps(n_steps); }

#ifdef __cplusplus
}
#endif