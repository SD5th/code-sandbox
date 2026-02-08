from flask import Flask, request, jsonify, render_template
import numpy as np
from solver_wrapper import SolverWrapper

app = Flask(__name__)
app.secret_key = 'key'

solver = SolverWrapper('./lib/libsolver.so')

current_grid = np.zeros((10, 10), dtype=np.float64)
step_count = 0

@app.route('/')
def home():
  """Главная страница - загружает HTML шаблон"""
  return render_template('index.html')

# API endpoints
@app.route('/api/state', methods=['GET'])
def get_state():
  """Показать текущую сетку"""
  return jsonify({
    "grid": current_grid.tolist(),
    "step": step_count,
    "size": current_grid.shape[0]
  })

@app.route('/api/init', methods=['POST'])
def init():
  """Создать новую сетку"""
  global current_grid, step_count
  
  data = request.json or {}
  
  if 'grid' in data:
    grid_data = data['grid']
    new_grid = np.array(grid_data, dtype=np.float64)
  else:
    size = data.get('size', 10)
    new_grid = np.random.rand(size, size).astype(np.float64)
  
  if not new_grid.flags['C_CONTIGUOUS']:
    new_grid = np.ascontiguousarray(new_grid)
  
  current_grid = new_grid
  step_count = 0
  
  return jsonify({
    "success": True, 
    "grid": current_grid.tolist(),
    "step": step_count
  })

@app.route('/api/step', methods=['POST'])
def step():
  """Сделать один шаг симуляции"""
  global current_grid, step_count
  
  solver.step(current_grid)
  step_count += 1
  
  return jsonify({
    "success": True,
    "grid": current_grid.tolist(),
    "step": step_count
  })

@app.route('/api/reset', methods=['POST'])
def reset():
  """Обнулить сетку"""
  global current_grid, step_count
  
  size = current_grid.shape[0]
  current_grid = np.zeros((size, size), dtype=np.float64)
  step_count = 0
    
  return jsonify({
    "success": True, 
    "grid": current_grid.tolist(),
    "step": step_count
  })

if __name__ == '__main__':
  print("Сервер запущен: http://localhost:5000")
  print("API: http://localhost:5000/api/state")
  print("Интерфейс: http://localhost:5000/")
  app.run(host='0.0.0.0', port=5000, debug=True)