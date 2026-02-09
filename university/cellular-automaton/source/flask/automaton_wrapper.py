import cffi
import numpy as np
import os

class AutomatonWrapper:
  def __init__(self, lib_path='./lib/libautomaton.so'):
    self._ffi = cffi.FFI()
    self._ffi.cdef("""
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
    """)

    if not os.path.exists(lib_path):
      raise FileNotFoundError(f"Library not found: {lib_path}")

    self._lib = self._ffi.dlopen(lib_path)
    self._handle = None
    self._view_cells = None
    self._cached_width = 0
    self._cached_height = 0

  def create(self, width, height):
    if self._handle:
      self.destroy()
    
    self._handle = self._lib.create_automaton(width, height)
    self._update_properties()

    return self
  
  def _update_properties(self):
    if not self._handle:
      return

    width = self._lib.automaton_get_width(self._handle)
    height = self._lib.automaton_get_height(self._handle)
    cells_ptr = self._lib.automaton_get_cells(self._handle)

    self._cached_width = width
    self._cached_height = height
    self._view_cells = np.frombuffer(
      self._ffi.buffer(cells_ptr, width * height),
      dtype=np.uint8
    ).reshape(height, width)

  def destroy(self):
    if self._handle:
      self._lib.destroy_automaton(self._handle)
      self._handle = None
      self._view_cells = None
      self._cached_width = 0
      self._cached_height = 0

  @property
  def width(self):
    if not self._handle:
      raise RuntimeError("automaton is not created")

    return self._cached_width

  @property
  def height(self):
    if not self._handle:
      raise RuntimeError("automaton is not created")

    return self._cached_height

  @property
  def cells(self):
    if not self._handle:
      raise RuntimeError("automaton is not created")
    
    if self._view_cells is None:
      self._update_properties()
        
    return self._view_cells

  def get_cell(self, x, y):
    if not self._handle:
      raise RuntimeError("automaton is not created")
    
    if not ((0 <= x and x < self.width) and (0 <= y and y < self.height)):
      raise ValueError(f"Cell coordinates ({x}, {y}) out of bounds")

    return bool(self.cells[y, x])

  def set_cell(self, x, y, value):
    if not self._handle:
      raise RuntimeError("automaton is not created")
    
    if not ((0 <= x and x < self.width) and (0 <= y and y < self.height)):
      raise ValueError(f"Cell coordinates ({x}, {y}) out of bounds")
    
    self._lib.automaton_set_cell(self._handle, x, y, bool(value))
  
  def toggle_cell(self, x, y):
    if not self._handle:
      raise RuntimeError("automaton is not created")
    
    cell = self.get_cell(x, y)
    cell_new = not cell
    self.set_cell(x, y, cell_new)
    return cell_new
 
  def randomize(self):
    if self._handle:
      self._lib.automaton_randomize(self._handle)
      self._update_properties()
  
  def clear(self):
    if self._handle:
      self._lib.automaton_clear(self._handle)
      self._update_properties()


  def step(self):
    if not self._handle:
      raise RuntimeError("automaton is not created")
    self._lib.automaton_step(self._handle)
    self._update_properties()

  def steps(self, n_steps):
    if not self._handle:
      raise RuntimeError("automaton is not created")
    self._lib.automaton_steps(self._handle, int(n_steps))
    self._update_properties()

  def __enter__(self):
    return self

  def __exit__(self, *args):
    self.destroy()