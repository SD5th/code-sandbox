#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* automaton_handle;

automaton_handle create_automaton(int width, int height);
void destroy_automaton(automaton_handle handle);

int automaton_get_width(automaton_handle handle);
int automaton_get_height(automaton_handle handle);
const bool* automaton_get_cells(automaton_handle handle);

bool automaton_get_cell(automaton_handle handle, int x, int y);
void automaton_set_cell(automaton_handle handle, int x, int y, bool value);

void automaton_randomize(automaton_handle handle);
void automaton_clear(automaton_handle handle);

void automaton_step(automaton_handle handle);
void automaton_steps(automaton_handle handle, int n_steps);

#ifdef __cplusplus
}
#endif
#endif