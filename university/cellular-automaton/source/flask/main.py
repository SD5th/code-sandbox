from flask import Flask, request, jsonify, render_template
import threading
import time
import queue
from automaton_wrapper import AutomatonWrapper

app = Flask(__name__)

automaton = None
runtime_thread = None
runtime_queue = queue.Queue()
stop_event = threading.Event()
automaton_lock = threading.Lock()

def init_automaton(width=30, height=30):
  global automaton
  automaton = AutomatonWrapper('./lib/libautomaton.so')
  automaton.create(width, height)
  return automaton

init_automaton(30, 30)

def runtime_worker():
  while not stop_event.is_set():
    with automaton_lock:
      automaton.step()
      grid = automaton.cells.copy()
    
    try:
      runtime_queue.put_nowait(grid.tolist())
    except queue.Full:
      pass
    
    time.sleep(0.1)

@app.route('/')
def home():
  return render_template('index.html')

@app.route('/api/state', methods=['GET'])
def get_state():
  with automaton_lock:
    grid = automaton.cells.tolist() if automaton else []
    width = automaton.width if automaton else 0
    height = automaton.height if automaton else 0
  
  return jsonify({
    "grid": grid,
    "width": width,
    "height": height,
    "success": True
  })

@app.route('/api/init', methods=['POST'])
def init_grid():
  global automaton, runtime_thread, stop_event
  
  data = request.json or {}
  width = data.get('width', 30)
  height = data.get('height', 30)
  
  if runtime_thread and runtime_thread.is_alive():
    stop_event.set()
    runtime_thread.join()
    runtime_thread = None
    stop_event.clear()
  
  with automaton_lock:
    init_automaton(width, height)
  
  return jsonify({
    "success": True,
    "width": width,
    "height": height
  })

@app.route('/api/step', methods=['POST'])
def step():
  with automaton_lock:
    automaton.step()
    grid = automaton.cells.tolist()
  
  return jsonify({
    "success": True,
    "grid": grid
  })

@app.route('/api/steps', methods=['POST'])
def steps():
  data = request.json or {}
  n_steps = data.get('steps', 1)
  
  with automaton_lock:
    automaton.steps(n_steps)
    grid = automaton.cells.tolist()
  
  return jsonify({
    "success": True,
    "grid": grid
  })

@app.route('/api/randomize', methods=['POST'])
def randomize():
  with automaton_lock:
    automaton.randomize()
    grid = automaton.cells.tolist()
  
  return jsonify({
    "success": True,
    "grid": grid
  })

@app.route('/api/clear', methods=['POST'])
def clear():
  with automaton_lock:
    automaton.clear()
    grid = automaton.cells.tolist()
  
  return jsonify({
    "success": True,
    "grid": grid
  })

@app.route('/api/set_cell', methods=['POST'])
def set_cell():
  data = request.json or {}
  x = data.get('x')
  y = data.get('y')
  value = data.get('value', True)
  
  if x is None or y is None:
    return jsonify({"success": False, "error": "Missing coordinates"}), 400
  
  with automaton_lock:
    automaton.set_cell(x, y, value)
  
  return jsonify({"success": True})

@app.route('/api/toggle_cell', methods=['POST'])
def toggle_cell():
  data = request.json or {}
  x = data.get('x')
  y = data.get('y')
  
  if x is None or y is None:
    return jsonify({"success": False, "error": "Missing coordinates"}), 400
  
  with automaton_lock:
    current = automaton.get_cell(x, y)
    automaton.set_cell(x, y, not current)
  
  return jsonify({"success": True, "value": not current})

@app.route('/api/start', methods=['POST'])
def start_runtime():
  global runtime_thread, stop_event
  
  if runtime_thread and runtime_thread.is_alive():
    stop_event.set()
    runtime_thread.join()
  
  while not runtime_queue.empty():
    runtime_queue.get_nowait()
  
  stop_event.clear()
  runtime_thread = threading.Thread(target=runtime_worker, daemon=True)
  runtime_thread.start()
  
  return jsonify({"success": True})

@app.route('/api/stop', methods=['POST'])
def stop_runtime():
  global runtime_thread, stop_event
  
  if runtime_thread and runtime_thread.is_alive():
    stop_event.set()
    runtime_thread.join()
    runtime_thread = None
  
  return jsonify({"success": True})

@app.route('/api/runtime_state', methods=['GET'])
def get_runtime_state():
  grid = None
  
  while not runtime_queue.empty():
    grid = runtime_queue.get_nowait()
  
  if grid is None:
    with automaton_lock:
      grid = automaton.cells.tolist() if automaton else []
  
  return jsonify({
    "success": True,
    "grid": grid
  })

if __name__ == '__main__':
  print("=" * 50)
  print("Cellular Automaton Server Started!")
  print("Interface: http://localhost:5000")
  print("API: http://localhost:5000/api/state")
  print("=" * 50)
  app.run(host='0.0.0.0', port=5000, debug=True)