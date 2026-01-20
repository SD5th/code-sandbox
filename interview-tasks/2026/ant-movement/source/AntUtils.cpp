#include "AntUtils.h"
#include "CanEnterCell.h"

#include <fstream>
#include <sstream>
#include <ctime>


static void convertPPMIntoJPG(std::string imageName);

std::string DrawAntMatrix(AntMatrix const & antMatrix) {
  std::string imageName = "ant-movement-" + std::to_string(std::time(nullptr));
  std::ofstream img(imageName + ".ppm");
  img << "P5\n" << antMatrix.width << " " << antMatrix.height << "\n255\n";
  for (int y = antMatrix.upCell.y; antMatrix.downCell.y <= y; y--) {
    for (int x = antMatrix.leftCell.x; x <= antMatrix.rightCell.x; x++) {
      char pixel = antMatrix.GetOrFalse(Cell(x, y)) ? 0 : 255; 
      img.write(&pixel, 1);
    }
  }
  img.close();
  convertPPMIntoJPG(imageName);
  std::string imageFileName = imageName + ".png";
  return imageFileName;
}

static void convertPPMIntoJPG(std::string imageName) {
  std::string covertCmd = "magick " + imageName + ".ppm " + imageName + ".png";
  system(covertCmd.c_str());
  
  std::string rmCmd = "rm " + imageName + ".ppm";
  system(rmCmd.c_str());
}

// void GenerateIndexHTML(
//   AntMatrix const & antMatrix, 
//   Cell startingCell,
//   std::string isCellReachableFunction, 
//   int cellsVisited,
//   std::string imageFileName
// ){
//   std::stringstream ss;
//   ss << 
//   R"(
//     <!DOCTYPE html>
//     <html>
// <head>
//     <title>Ant Movement</title>
//     <style>
//       body { font-family: sans-serif; margin: 20px; }
//       img { max-width: 400px; margin: 10px; border: 2px solid #ddd; }
//       .gallery { display: flex; flex-wrap: wrap; }
//     </style>
// </head>
// <body>
//     <h1>Starting conditions</h1>
//     <h2>Starting cell: ()" << startingCell.x << ", " << startingCell.y << R"()</h2>
//     <h2>IsCellReachable-function: )" << isCellReachableFunction <<R"(</h2>
//     </p>
//     <h1>Result</h1>
//     <h2>Cells visited:   )" << cellsVisited <<R"(</h2>
//     <h2>Matrix size:     )" << antMatrix.width << "x" << antMatrix.height << R"(</h2>
//     <h3>Highest   cell: ()" << antMatrix.upCell.x << ", " << antMatrix.upCell.y << R"()</h3>
//     <h3>Lowest    cell: ()" << antMatrix.downCell.x << ", " << antMatrix.downCell.y << R"()</h3>
//     <h3>Leftmost  cell: ()" << antMatrix.leftCell.x << ", " << antMatrix.leftCell.y << R"()</h3>
//     <h3>Rightmost cell: ()" << antMatrix.rightCell.x << ", " << antMatrix.rightCell.y << R"()</h3>
//     </p>
//     <div class="gallery">
//       <div><a href=")" << imageFileName << R"("><img src=")" << imageFileName <<R"(" alt="Ant Movement"></a></div>
//     </div>
// </body>
// </html>)";

//   std::ofstream file("index.html");
//   if (file) {
//     file << ss.str();
//   }
// }



void GenerateIndexHTML(
  AntMatrix const & antMatrix, 
  Cell startingCell,
  std::string funcDescription,
  int cellsVisited,
  std::string imageFileName
){
  std::stringstream ss;
  ss << 
  R"(

<!DOCTYPE html>
<html>
<head>
  <title>Ant Matrix Solver</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 20px;
      background: #f8f9fa;
    }
        
    .container {
      display: flex;
      max-width: 1200px;
      margin: 0 auto;
      gap: 20px;
    }
        
    .left-panel {
      width: 350px;
    }
        
    .right-panel {
      flex: 1;
    }
        
    .panel {
      background: white;
      border: 1px solid #ddd;
      margin-bottom: 20px;
    }
        
    .panel-header {
      background: #2c3e50;
      color: white;
      padding: 12px 15px;
      font-weight: bold;
    }
        
    .panel-content {
      padding: 20px;
    }
        
    .form-row {
      margin-bottom: 15px;
    }
        
    label {
      display: block;
      margin-bottom: 5px;
      color: #444;
      font-weight: 500;
    }
    
    input[type="number"], select {
      width: 100%;
      padding: 8px 10px;
      border: 1px solid #ccc;
      font-size: 14px;
      box-sizing: border-box;

    }
    
    .cell-inputs {
      display: flex;
      gap: 10px;
    }
    
    .cell-inputs > div {
      flex: 1;
      box-sizing: border-box;
      padding: 0 2px;
    }
    
    .solve-button {
      width: 100%;
      padding: 12px;
      background: #3498db;
      color: white;
      border: none;
      font-size: 16px;
      cursor: pointer;
      margin-top: 10px;
    }
    
    .solve-button:hover {
      background: #2980b9;
    }
    
    .result-item {
      margin-bottom: 15px;
      padding-bottom: 15px;
      border-bottom: 1px solid #eee;
    }
    
    .result-item:last-child {
      border-bottom: none;
      margin-bottom: 0;
    }
    
    .result-label {
      color: #666;
      font-size: 13px;
      margin-bottom: 3px;
    }
    
    .result-value {
      font-weight: bold;
      color: #2c3e50;
    }
    
    .cell {
      font-family: 'Courier New', monospace;
      background: #e8f4f8;
      padding: 2px 6px;
    }
    
    .matrix-image {
      width: 100%;
      max-width: 700px;
      border: 1px solid #ccc;
      display: block;
      margin: 0 auto;
      image-rendering: pixelated;
    }
    
    .image-info {
      text-align: center;
      margin-top: 10px;
      color: #666;
      font-size: 14px;
    }
    
    .function-hint {
      font-size: 12px;
      color: #777;
      margin-top: 3px;
    }
        
    @media (max-width: 900px) {
      .container { flex-direction: column; }
      .left-panel, .right-panel { width: 100%; }
    }

  </style>
</head>

<body>
  <div class="container">
    <div class="left-panel">
        <div class="panel">
          <div class="panel-header">Parameters</div>
              <div class="panel-content">
                <form action="/solve.cgi" method="get">
                  <div class="form-row">
                    <label>Start Cell</label>
                      <div class="cell-inputs">
                        <div>
                          <input type="number" 
                                  name="startcell_x" 
                                  required
                                  placeholder="X">
                        </div>
                        <div>
                          <input type="number" 
                                  name="startcell_y" 
                                  required
                                  placeholder="Y">
                        </div>
                      </div>
                  </div>
        
                  <div class="form-row">
                    <label>Function</label>
                    <select name="function">
                      <option value="SUM_OF_DIGITS">Sum of digits <= limit</option>
                      <option value="SUM_OF_ABS_NUMBERS">|x| + |y| <= limit</option>
                      <option value="SUM_OF_SQUARED_NUMBERS">x^2 + y^2 <= limit</option>
                    </select>
                  </div>
                        
                  <div class="form-row">
                    <label>Limit</label>
                    <input type="number" 
                            name="limit" 
                            required
                            min="1">
                  </div>
                        
                  <button type="submit" class="solve-button">
                    Solve Ant-problem
                  </button>
                </form>
              </div>
            </div>
            
            <div class="panel">
              <div class="panel-header">Results</div>
              <div class="panel-content">
                <div class="result-item">
                  <div class="result-label">Input</div>
                  <div class="result-value">
                    Start: <span class="cell">()" << startingCell.x << ", " << startingCell.y << R"()</span><br>
                    Function: <span class="cell">)" << funcDescription <<R"(</span><br>
                  </div>
                </div>    
                    <div class="result-item">
                    <div class="result-label">Cells visited</div>
                    <div class="result-value">)" << cellsVisited << R"(</div>
                </div>
                    
                <div class="result-item">
                  <div class="result-label">Matrix size</div>
                  <div class="result-value">)" << antMatrix.width << " x " << antMatrix.height << R"(</div>
                </div>
                    
                <div class="result-item">
                  <div class="result-label">Boundaries</div>
                  <div class="result-value">
                    Top: <span class="cell">()" << antMatrix.upCell.x << ", " << antMatrix.upCell.y << R"()</span><br>
                    Bottom: <span class="cell">()" << antMatrix.downCell.x << ", " << antMatrix.downCell.y << R"()</span><br>
                    Left: <span class="cell">()" << antMatrix.leftCell.x << ", " << antMatrix.leftCell.y << R"()</span><br>
                    Right: <span class="cell">()" << antMatrix.rightCell.x << ", " << antMatrix.rightCell.y << R"()</span><br>
                  </div>
                </div>
              </div>
            </div>
        </div>
        
        <div class="right-panel">
          <div class="panel">
            <div class="panel-header">Matrix Visualization</div>
            <div class="panel-content">
              <img src=")" << imageFileName << R"("
                alt="Matrix" 
                class="matrix-image">
              <div class="image-info">
                  Black cells are reachable, white cells are unreachable </p>
                  If you don't see image, the solver may have caught segmentation fault :P </p>
                  (try another arguments)
              </div>
            </div>
          </div>
        </div>
    </div>
    
    <script>
        const functionSelect = document.querySelector('select[name="function"]');
        const functionHint = document.getElementById('function-hint');
        const limitInput = document.querySelector('input[name="limit"]');
        
        const hints = {
          'SUM_OF_DIGITS': 'Checks: sum(digits(x)) + sum(digits(y)) <= limit',
          'SUM_OF_ABS_NUMBERS': 'Checks: |x| + |y| <= limit',
          'SUM_OF_SQUARED_NUMBERS': 'Checks: x^2 + y^2 ≤ limit'
        };
        
        functionSelect.addEventListener('change', function() {
            functionHint.textContent = hints[this.value];
        });
        
        functionSelect.dispatchEvent(new Event('change'));
                        
        const form = document.querySelector('form');
        form.addEventListener('submit', function() {
          const button = this.querySelector('.solve-button');
          button.disabled = true;
          button.textContent = 'Processing...';
          setTimeout(() => {
            button.disabled = false;
            button.textContent = 'Run Simulation';
          }, 3000);
        });
        
        const params = new URLSearchParams(window.location.search);
    ['startcell_x', 'startcell_y', 'function', 'limit'].forEach(param => {
      if (params.has(param)) {
        const element = document.querySelector(`[name="${param}"]`);
        if (element) element.value = params.get(param);
      }
    });
  </script>
</body>
</html>
  
)";

  std::ofstream file("index.html");
  if (file) {
    file << ss.str();
  }
}