#include "../1laba/source/bmp_image.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

BMPImage rankFilter(const BMPImage& src) {
  int width = src.getWidth();
  int height = src.getHeight();
  BMPImage dst(width, height);
    
  int maskX[] = {-1, -1, 0, 1, 1};
  int maskY[] = {-1, 1, 0, -1, 1};
  int maskSize = 5;
  int rank = 4;
    
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      std::vector<uint8_t> values;
            
      for (int i = 0; i < maskSize; ++i) {
        int nx = x + maskX[i];
        int ny = y + maskY[i];
        
        if (0 <= nx && nx < width && 0 <= ny && ny < height) {
          BMP::Pixel p = src.getPixel(nx, ny);
          values.push_back(p.red);
        } else {
          BMP::Pixel p = src.getPixel(x, y);
          values.push_back(p.red);
        }
      }
            
      std::sort(values.begin(), values.end());
      uint8_t result = values[rank - 1];
            
      dst.getPixel(x, y).red   = result;
      dst.getPixel(x, y).green = result;
      dst.getPixel(x, y).blue  = result;
    }
  }
    
  return dst;
}

BMPImage differenceMonochrome(const BMPImage& original, const BMPImage& filtered) {
  int width = original.getWidth();
  int height = original.getHeight();
  BMPImage dst(width, height);
  
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      uint8_t orig = original.getPixel(x, y).red;
      uint8_t filt = filtered.getPixel(x, y).red;
      uint8_t diff = (orig != filt) ? 255 : 0;
      
      dst.getPixel(x, y).red   = diff;
      dst.getPixel(x, y).green = diff;
      dst.getPixel(x, y).blue  = diff;
    }
  }
  
  return dst;
}

BMPImage differenceSemitone(const BMPImage& original, const BMPImage& filtered, int contrast = 1) {
  int width = original.getWidth();
  int height = original.getHeight();
  BMPImage dst(width, height);
  
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      uint8_t orig = original.getPixel(x, y).red;
      uint8_t filt = filtered.getPixel(x, y).red;
      int diff = std::abs(orig - filt);
          
      diff = diff * contrast;
      if (diff > 255) diff = 255;
          
      uint8_t result = static_cast<uint8_t>(diff);
      
      dst.getPixel(x, y).red   = result;
      dst.getPixel(x, y).green = result;
      dst.getPixel(x, y).blue  = result;
    }
  }
  
  return dst;
}

void solve(const std::string& sampleName, bool isMonochrome, int contrast = 1) {
  std::cout << "=== Processing " << sampleName << " ===" << std::endl;
    
  BMPImage image("samples/" + sampleName + ".bmp");
  std::cout << "Image loaded: " << image.getWidth() << "x" << image.getHeight() << std::endl;
        
  std::cout << "\n[1] Applying rank filter (cross, rank 4/5)..." << std::endl;
    
  BMPImage filtered = rankFilter(image);
  filtered.save("output/" + sampleName + "_filtered.bmp");
  std::cout << "  Saved: filtered image" << std::endl;
    
  std::cout << "\n[2] Creating difference image..." << std::endl;
    
  BMPImage diff(image.getWidth(), image.getHeight());
  if (isMonochrome) {
    diff = differenceMonochrome(image, filtered);
  } else {
    diff = differenceSemitone(image, filtered, contrast);
  }
  diff.save("output/" + sampleName + "_difference.bmp");
  std::cout << "  Saved: difference image" << std::endl;
  
  std::cout << "\n=== Done ===" << std::endl;
}

int main() {
  solve("sample1", true);
  solve("sample2", true);
  solve("sample3", false, 5);
  solve("sample4", false, 2);
    
}