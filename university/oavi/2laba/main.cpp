#include "../1laba/source/bmp_image.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

BMPImage wolfThreshold(const BMPImage& src, int windowSize, double k) {
  int width = src.getWidth();
  int height = src.getHeight();
  BMPImage dst(width, height);
  
  uint8_t globalMin = 255;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      BMP::Pixel p = src.getPixel(x, y);
      globalMin = std::min(globalMin, p.red);
    }
  }
  
  int halfWindow = windowSize / 2;
  double R = 128.0;  
  
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      double sumLocal = 0;
      double sumSqLocal = 0;
      int pixelCount = 0;
          
      for (int dy = -halfWindow; dy <= halfWindow; ++dy) {
        for (int dx = -halfWindow; dx <= halfWindow; ++dx) {
          int nx = x + dx;
          int ny = y + dy;
          if (0 <= nx && nx < width && 0 <= ny && ny < height) {
            BMP::Pixel p = src.getPixel(nx, ny);
            sumLocal += p.red;
            sumSqLocal += p.red * p.red;
            pixelCount++;
          }
        }
      }
          
      double localMean = sumLocal / pixelCount;
      double localStd = std::sqrt(sumSqLocal / pixelCount - localMean * localMean);
      
      double threshold = (1 - k) * localMean + 
                          k * globalMin + 
                          k * (localStd / R) * (localMean - globalMin);
      
      uint8_t value = (src.getPixel(x, y).red > threshold) ? 255 : 0;
      
      dst.getPixel(x, y).red   = value;
      dst.getPixel(x, y).green = value;
      dst.getPixel(x, y).blue  = value;
    }
  }
  
  return dst;
}

void solve(const std::string & sampleName, int windowSize = 15, double k = 0.5) {
  std::cout << "=== Processing " << sampleName << " ===" << std::endl;
  std::cout << "Loading image: samples/" << sampleName << ".bmp" << std::endl;
  
  BMPImage sample("samples/" + sampleName + ".bmp");
  
  /*
  1. Приведение полноцветного изображения к полутоновому. Новое изображение 
  создаётся в режиме полутона (1 яркостный канал, формат bmp), где яркость
  каждого пикселя вычисляется (взвешенным) усреднением каналов исходного
  полноцветного изображения. 
  */ 
  std::cout << "\n[1] Creating semitone image..." << std::endl;
    BMPImage semitone(sample.getWidth(), sample.getHeight());
    
    for (int x = 0; x < sample.getWidth(); x++) {
      for (int y = 0; y < sample.getHeight(); y++) {
        BMP::Pixel pixel = sample.getPixel(x, y);
        uint8_t colour = (pixel.red + pixel.green + pixel.blue) / 3;

        semitone.getPixel(x, y).red   = colour;
        semitone.getPixel(x, y).green = colour;
        semitone.getPixel(x, y).blue  = colour;
      }
    }
    semitone.save("output/" + sampleName + "_1_semitone.bmp");
    std::cout << "  Saved: semitone image" << std::endl;

    /*
    2. Приведение полутонового изображения к монохромному методом 
    пороговой обработки (Адаптивная бинаризация Вульфа). 
    */ 
    std::cout << "\n[2] Creating monochrome image..." << std::endl;
    BMPImage monochrome = wolfThreshold(semitone, windowSize, k);
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << k;
    std::string k_str = oss.str();

    monochrome.save("output/" + sampleName + "_2_monchrome_" + std::to_string(windowSize) + "_" + k_str + "_.bmp");
    std::cout << "  Saved: monchrome image" << std::endl;
  
  std::cout << "\n=== Done ===" << std::endl;
}

int main() {
  solve("sample1", 15, 0.20);
  solve("sample2", 18, 0.10);
  solve("sample3", 15, 0.05);
}