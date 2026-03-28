#ifndef BMP_IMAGE_H

#include "bmp_format.h"

#include <memory>
#include <cmath>

class BMPImage {
private:

  BMP::FileHeader fileHeader;
  BMP::InfoHeader infoHeader;
  std::unique_ptr<std::byte[]> pixelArray;

  void initializeHeaders();
  void updateHeaders(int32_t width, int32_t height);
  void validateHeaders() const;
  void normalizeHeaders();

  inline int32_t getRowSize() const { return BMP::calculateRowSize(infoHeader.width); }
  inline int32_t getRawHeight() const { return infoHeader.height; }
  
  inline uint32_t getPixelArraySize() const { return infoHeader.pixelArraySize; }

public:
  BMPImage() = delete;
  BMPImage(int w, int h);
  BMPImage(const std::string& filename);
  BMPImage(const BMPImage& other);
  BMPImage& operator=(const BMPImage& other);

  inline int32_t getWidth() const { return infoHeader.width; }
  inline int32_t getHeight() const { return std::abs(infoHeader.height); }

  void save(const std::string& filename) const;
    
  BMP::Pixel& getPixel(int x, int y);
  const BMP::Pixel& getPixel(int x, int y) const;
  void setPixel(int x, int y, const BMP::Pixel& pixel);
};

#endif