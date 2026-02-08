import cffi
import numpy as np
import os


class SolverWrapper:
  def __init__(self, lib_path='./lib/libsolver.so'):
    self.ffi = cffi.FFI()
    self.ffi.cdef(
    """
      void calculate_step(double* cells, int size);
    """
    )

    if not os.path.exists(lib_path):
      print(f"DEBUG: Library not found at: {lib_path}")
      print(f"DEBUG: Current directory: {os.getcwd()}")
      print(f"DEBUG: Directory contents:")
      try:
        print(f"1:")
        for item in os.listdir('.'):
          print(f"2:")
          full_path = os.path.join('.', item)
          if os.path.isdir(full_path):
            print(f"  DIR:  {item}/")
          else:
            size = os.path.getsize(full_path)
            print(f"  FILE: {item} ({size} bytes)")
      except Exception as e:
        print(f"  ERROR listing directory: {e}")
      raise FileNotFoundError(f"Library not found: {lib_path}")

    self.lib = self.ffi.dlopen(lib_path)

    
  def step(self, cells: np.ndarray) -> None:
    size = cells.shape[0]

    if not cells.flags['C_CONTIGUOUS']:
      raise ValueError("Array must be C-contiguous")

    if cells.dtype != np.float64:
      raise ValueError("Array must be float64")

    cells_ptr = self.ffi.cast("double*", cells.ctypes.data)

    try:
      self.lib.calculate_step(cells_ptr, size)
    except Exception as e:
      raise RuntimeError(f"C++ function calculate_step failed: {e}")