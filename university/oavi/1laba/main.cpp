#include "source/bmp_image.h"

#include <string>
#include <iostream>
#include <algorithm>

void solve1(const std::string & sampleName) {
  std::cout << "=== Processing " << sampleName << " ===" << std::endl;
  std::cout << "Loading image: samples/" << sampleName << ".bmp" << std::endl;
  
  BMPImage sample("samples/" + sampleName + ".bmp");
  std::cout << "Image loaded: " << sample.getWidth() << "x" << sample.getHeight() << std::endl;
  
  // 1. Цветовые модели
    // 1) Выделить компоненты R, G, B и сохранить как отдельные изображения.
    std::cout << "\n[1.1] Extracting RGB channels..." << std::endl;
      BMPImage sample_red   = sample;
      BMPImage sample_green = sample;
      BMPImage sample_blue  = sample;
  
      for (int x = 0; x < sample.getWidth(); x++) {
        for (int y = 0; y < sample.getHeight(); y++) {
          sample_red.getPixel(x, y).blue  = 0;
          sample_red.getPixel(x, y).green = 0;
          
          sample_green.getPixel(x, y).red  = 0;
          sample_green.getPixel(x, y).blue = 0;
          
          sample_blue.getPixel(x, y).red   = 0;
          sample_blue.getPixel(x, y).green = 0;
        }
      }
    
      sample_red.save("output/" + sampleName + "_1.1_red.bmp");
      sample_green.save("output/" + sampleName + "_1.1_green.bmp");
      sample_blue.save("output/" + sampleName + "_1.1_blue.bmp");
      std::cout << "  Saved: red, green, blue channels" << std::endl;

    // 2) Привести изображение к цветовой модели HSI, сохранить яркостную компоненту как отдельное изображение.
    std::cout << "\n[1.2] Extracting HSI intensity component..." << std::endl;
      BMPImage sample_hsi(sample.getWidth(), sample.getHeight());
      
      for (int x = 0; x < sample.getWidth(); x++) {
        for (int y = 0; y < sample.getHeight(); y++) {
          BMP::Pixel pixel = sample.getPixel(x, y);
          double intensity = ((pixel.red + pixel.green + pixel.blue) / 3.0) / 255.0;

          sample_hsi.getPixel(x, y).red   = 255 * intensity;
          sample_hsi.getPixel(x, y).green = 255 * intensity;
          sample_hsi.getPixel(x, y).blue  = 255 * intensity;
        }
      }
      sample_hsi.save("output/" + sampleName + "_1.2_hsi.bmp");
      std::cout << "  Saved: intensity component" << std::endl;

    // 3) Инвертировать яркостную компоненту в исходном изображении, сохранить производное изображение.
    std::cout << "\n[1.3] Inverting intensity component..." << std::endl;
      BMPImage sample_hsi_inverted(sample.getWidth(), sample.getHeight());
      
      for (int x = 0; x < sample.getWidth(); x++) {
        for (int y = 0; y < sample.getHeight(); y++) {
          BMP::Pixel pixel = sample.getPixel(x, y);
          double intensity = ((pixel.red + pixel.green + pixel.blue) / 3.0) / 255.0;

          sample_hsi_inverted.getPixel(x, y).red   = 255 * (1 - intensity);
          sample_hsi_inverted.getPixel(x, y).green = 255 * (1 - intensity);
          sample_hsi_inverted.getPixel(x, y).blue  = 255 * (1 - intensity);
        }
      }
      sample_hsi_inverted.save("output/" + sampleName + "_1.3_hsi_inverted.bmp");
      std::cout << "  Saved: inverted intensity component" << std::endl;
  
  std::cout << "\n=== Done ===" << std::endl;
}



uint8_t bilinearInterpolate(uint8_t a, uint8_t b, uint8_t c, uint8_t d, double dx, double dy) {
  double top    = a * (1 - dx) + b * dx;
  double bottom = c * (1 - dx) + d * dx;
  double result = top * (1 - dy) + bottom * dy;
  return static_cast<uint8_t>(std::clamp(result, 0.0, 255.0));
}

BMPImage stretch(const BMPImage& src, int M) {
  int srcW = src.getWidth();
  int srcH = src.getHeight();
  int dstW = srcW * M;
  int dstH = srcH * M;
    
  BMPImage dst(dstW, dstH);
    
  for (int y = 0; y < dstH; ++y) {
    for (int x = 0; x < dstW; ++x) {
      double srcX = static_cast<double>(x) / M;
      double srcY = static_cast<double>(y) / M;
            
      int x1 = static_cast<int>(srcX);
      int x2 = std::min(x1 + 1, srcW - 1);
      int y1 = static_cast<int>(srcY);
      int y2 = std::min(y1 + 1, srcH - 1);
            
      double dx = srcX - x1;
      double dy = srcY - y1;
            
      BMP::Pixel p11 = src.getPixel(x1, y1);
      BMP::Pixel p12 = src.getPixel(x2, y1);
      BMP::Pixel p21 = src.getPixel(x1, y2);
      BMP::Pixel p22 = src.getPixel(x2, y2);
            
      BMP::Pixel result;
      result.red   = bilinearInterpolate(p11.red, p12.red, p21.red, p22.red, dx, dy);
      result.green = bilinearInterpolate(p11.green, p12.green, p21.green, p22.green, dx, dy);
      result.blue  = bilinearInterpolate(p11.blue, p12.blue, p21.blue, p22.blue, dx, dy);
      
      dst.setPixel(x, y, result);
    }
  }
    
  return dst;
}

BMPImage compress(const BMPImage& src, int N) {
  int srcW = src.getWidth();
  int srcH = src.getHeight();
  int dstW = srcW / N;
  int dstH = srcH / N;
  
  BMPImage dst(dstW, dstH);
  
  for (int y = 0; y < dstH; ++y) {
    for (int x = 0; x < dstW; ++x) {
      long long sumR = 0, sumG = 0, sumB = 0;
      int pixelCount = 0;
        
      for (int dy = 0; dy < N; ++dy) {
        for (int dx = 0; dx < N; ++dx) {
          int srcX = x * N + dx;
          int srcY = y * N + dy;
              
          if (srcX < srcW && srcY < srcH) {
            BMP::Pixel p = src.getPixel(srcX, srcY);
            sumR += p.red;
            sumG += p.green;
            sumB += p.blue;
            pixelCount++;
          }
        }
      }
        
      BMP::Pixel result;
      result.red   = static_cast<uint8_t>(sumR / pixelCount);
      result.green = static_cast<uint8_t>(sumG / pixelCount);
      result.blue  = static_cast<uint8_t>(sumB / pixelCount);
      
      dst.setPixel(x, y, result);
    }
  }
  
  return dst;
}

BMPImage resample(const BMPImage& src, double K) {
  int srcW = src.getWidth();
  int srcH = src.getHeight();
  int dstW = static_cast<int>(srcW * K);
  int dstH = static_cast<int>(srcH * K);
  
  BMPImage dst(dstW, dstH);
  
  for (int y = 0; y < dstH; ++y) {
    for (int x = 0; x < dstW; ++x) {
      double srcX = static_cast<double>(x) / K;
      double srcY = static_cast<double>(y) / K;
          
      srcX = std::clamp(srcX, 0.0, static_cast<double>(srcW - 1));
      srcY = std::clamp(srcY, 0.0, static_cast<double>(srcH - 1));
          
      int x1 = static_cast<int>(srcX);
      int x2 = std::min(x1 + 1, srcW - 1);
      int y1 = static_cast<int>(srcY);
      int y2 = std::min(y1 + 1, srcH - 1);
          
      double dx = srcX - x1;
      double dy = srcY - y1;
          
      BMP::Pixel p11 = src.getPixel(x1, y1);
      BMP::Pixel p12 = src.getPixel(x2, y1);
      BMP::Pixel p21 = src.getPixel(x1, y2);
      BMP::Pixel p22 = src.getPixel(x2, y2);
          
      BMP::Pixel result;
      result.red   = bilinearInterpolate(p11.red, p12.red, p21.red, p22.red, dx, dy);
      result.green = bilinearInterpolate(p11.green, p12.green, p21.green, p22.green, dx, dy);
      result.blue  = bilinearInterpolate(p11.blue, p12.blue, p21.blue, p22.blue, dx, dy);
          
      dst.setPixel(x, y, result);
    }
  }
  
  return dst;
}

void solve2(const std::string & sampleName, int M, int N) {
  std::cout << "\n=== Processing " << sampleName << " (M=" << M << ", N=" << N << ") ===" << std::endl;
  
  BMPImage sample("samples/" + sampleName + ".bmp");
  std::cout << "Original image: " << sample.getWidth() << "x" << sample.getHeight() << std::endl;
  
  // 2. Передискретизация
    // 1) Растяжение (интерполяция) изображения в M раз;
    std::cout << "\n[2.1] Stretching by factor " << M << "..." << std::endl;
    BMPImage stretched = stretch(sample, M);
    stretched.save("output/" + sampleName + "_2.1_stretched_" + std::to_string(M) + ".bmp");
    std::cout << "  Result: " << stretched.getWidth() << "x" << stretched.getHeight() << std::endl;
    
    // 2) Сжатие (децимация) изображения в N раз;
    std::cout << "\n[2.2] Compressing by factor " << N << "..." << std::endl;
    BMPImage compressed = compress(sample, N);
    compressed.save("output/" + sampleName + "_2.2_compressed_" + std::to_string(N) + ".bmp");
    std::cout << "  Result: " << compressed.getWidth() << "x" << compressed.getHeight() << std::endl;
    
    // 3) Передискретизация изображения в K=M/N раз путём растяжения и последующего сжатия (в два прохода);
    double K = static_cast<double>(M) / N;
    std::cout << "\n[2.3] Two-pass resampling by factor " << K << " (" << M << "/" << N << ")..." << std::endl;
    
    BMPImage temp = stretch(sample, M);
    BMPImage twoPass = compress(temp, N);
    twoPass.save("output/" + sampleName + "_2.3_twopass_" + std::to_string(M) + "_" + std::to_string(N) + ".bmp");
    std::cout << "  Result: " << twoPass.getWidth() << "x" << twoPass.getHeight() << std::endl;
    
    // 4) Передискретизация изображения в K раз за один проход.
    std::cout << "\n[2.4] One-pass resampling by factor " << K << "..." << std::endl;
    BMPImage onePass = resample(sample, K);
    onePass.save("output/" + sampleName + "_2.4_onepass_" + std::to_string(M) + "_" + std::to_string(N) + ".bmp");
    std::cout << "  Result: " << onePass.getWidth() << "x" << onePass.getHeight() << std::endl;
  
  std::cout << "\n=== Done ===" << std::endl;
}

int main() {
  solve1("sample1");
  solve2("sample1", 5, 3);
  solve1("sample2");
  solve2("sample2", 2, 10);

}