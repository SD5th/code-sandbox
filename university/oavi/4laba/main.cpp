#include "../1laba/source/bmp_image.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>

BMPImage rgbToSemitone(const BMPImage& src) {
  BMPImage dst(src.getWidth(), src.getHeight());
    
  for (int y = 0; y < src.getHeight(); ++y) {
    for (int x = 0; x < src.getWidth(); ++x) {
      BMP::Pixel p = src.getPixel(x, y);
      int colour = (p.red + p.green + p.blue) / 3;
            
      dst.getPixel(x, y).red   = colour;
      dst.getPixel(x, y).green = colour;
      dst.getPixel(x, y).blue  = colour;
    }
  }
  return dst;
}

uint8_t normalize(int val, int minV, int maxV) {
  if (maxV == minV) 
    return 0;
  return static_cast<uint8_t>((val - minV) * 255.0 / (maxV - minV));
};

void prewitt5x5(const BMPImage& src, BMPImage& gx, BMPImage& gy, BMPImage& g) {
  int width = src.getWidth();
  int height = src.getHeight();
    
  int maskX[5][5] = {
    {-1, -1, -1, -1, -1},
    { 0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0},
    { 1,  1,  1,  1,  1}
  };
    
  int maskY[5][5] = {
    {-1,  0,  0,  0,  1},
    {-1,  0,  0,  0,  1},
    {-1,  0,  0,  0,  1},
    {-1,  0,  0,  0,  1},
    {-1,  0,  0,  0,  1}
  };
    
  std::vector<std::vector<int>> gradX(width, std::vector<int>(height, 0));
  std::vector<std::vector<int>> gradY(width, std::vector<int>(height, 0));
  std::vector<std::vector<int>> gradM(width, std::vector<int>(height, 0));
    
  int minGx = 0, maxGx = 0;
  int minGy = 0, maxGy = 0;
  int minG = 0, maxG = 0;
    
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int sumX = 0, sumY = 0;
            
      for (int ky = -2; ky <= 2; ++ky) {
        for (int kx = -2; kx <= 2; ++kx) {
          int nx = x + kx;
          int ny = y + ky;
                    
          if (0 <= nx && nx < width && 0 <= ny && ny < height) {
            uint8_t val = src.getPixel(nx, ny).red;
            sumX += val * maskX[ky + 2][kx + 2];
            sumY += val * maskY[ky + 2][kx + 2];
          }
        }
      }
            
      gradX[x][y] = sumX;
      gradY[x][y] = sumY;
      gradM[x][y] = std::abs(sumX) + std::abs(sumY);
            
      minGx = std::min(minGx, sumX);
      maxGx = std::max(maxGx, sumX);
      minGy = std::min(minGy, sumY);
      maxGy = std::max(maxGy, sumY);
      minG = std::min(minG, gradM[x][y]);
      maxG = std::max(maxG, gradM[x][y]);
    }
  }
    
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      uint8_t nx = normalize(gradX[x][y], minGx, maxGx);
      uint8_t ny = normalize(gradY[x][y], minGy, maxGy);
      uint8_t nm = normalize(gradM[x][y], minG, maxG);
            
      gx.getPixel(x, y).red = gx.getPixel(x, y).green = gx.getPixel(x, y).blue = nx;
      gy.getPixel(x, y).red = gy.getPixel(x, y).green = gy.getPixel(x, y).blue = ny;
      g.getPixel(x, y).red  = g.getPixel(x, y).green  = g.getPixel(x, y).blue  = nm;
    }
  }
}

BMPImage thresholdG(const BMPImage& g, uint8_t threshold) {
  BMPImage dst(g.getWidth(), g.getHeight());
    
  for (int y = 0; y < g.getHeight(); ++y) {
    for (int x = 0; x < g.getWidth(); ++x) {
      uint8_t val = g.getPixel(x, y).red;
      uint8_t bin = (val > threshold) ? 255 : 0;
            
      dst.getPixel(x, y).red   = bin;
      dst.getPixel(x, y).green = bin;
      dst.getPixel(x, y).blue  = bin;
    }
  }
  return dst;
}

void solve(const std::string& sampleName, uint8_t threshold = 30) {
  std::cout << "=== Processing " << sampleName << " ===" << std::endl;
    
  BMPImage colour("samples/" + sampleName + ".bmp");
  std::cout << "colour image: " << colour.getWidth() << "x" << colour.getHeight() << std::endl;
  colour.save("output/" + sampleName + "_1_colour.bmp");
    
  BMPImage semitone = rgbToSemitone(colour);
  semitone.save("output/" + sampleName + "_2_semitone.bmp");
  std::cout << "Semitone saved" << std::endl;
  
  BMPImage gx(semitone.getWidth(), semitone.getHeight());
  BMPImage gy(semitone.getWidth(), semitone.getHeight());
  BMPImage g(semitone.getWidth(), semitone.getHeight());
  
  prewitt5x5(semitone, gx, gy, g);
  
  gx.save("output/" + sampleName + "_3_gx.bmp");
  gy.save("output/" + sampleName + "_3_gy.bmp");
  g.save("output/" + sampleName + "_3_g.bmp");
  std::cout << "Gradient matrices saved (Gx, Gy, G)" << std::endl;
  
  BMPImage binary = thresholdG(g, threshold);
  binary.save("output/" + sampleName + "_4_binary_th" + std::to_string(threshold) + ".bmp");
  std::cout << "Binary G saved with threshold = " << threshold << std::endl;
  
  BMPImage binary_inverted(binary);
  for (int y = 0; y < g.getHeight(); ++y) {
    for (int x = 0; x < g.getWidth(); ++x) {
      binary_inverted.getPixel(x, y).red = 255 - binary_inverted.getPixel(x, y).red;
      binary_inverted.getPixel(x, y).green = 255 - binary_inverted.getPixel(x, y).green;
      binary_inverted.getPixel(x, y).blue = 255 - binary_inverted.getPixel(x, y).blue;
    }
  } 
  binary_inverted.save("output/" + sampleName + "_4_binary_th" + std::to_string(threshold) + "_inverted.bmp");
  std::cout << "Binary G inverted saved with threshold = " << threshold << std::endl;

  std::cout << "\n=== Done ===" << std::endl;
}

int main() {
  solve("sample1", 30);
  solve("sample2", 25);
  solve("sample3", 20);
  solve("sample4", 10);
  
}