#include "bmp_image.h"

#include <fstream>
#include <stdexcept>
#include <cstring>
#include <string>

void BMPImage::initializeHeaders() {
  fileHeader.signature  = BMP::SIGNATURE;
  fileHeader.fileSize   = BMP::HEADERS_TOTAL_SIZE; // empty BMP
  fileHeader.reserved1  = BMP::IGNORED;
  fileHeader.reserved2  = BMP::IGNORED;
  fileHeader.dataOffset = BMP::HEADERS_TOTAL_SIZE;

  infoHeader.headerSize = BMP::INFO_HEADER_SIZE;
  infoHeader.width      = 0;  // empty BMP
  infoHeader.height     = 0;  // empty BMP
  infoHeader.planes     = BMP::PLANES;
  infoHeader.bitsPerPixel = BMP::BITS_PER_PIXEL;
  infoHeader.compression  = BMP::COMPRESSION_NONE;
  infoHeader.pixelArraySize  = 0; // empty BMP
  infoHeader.xPixelsPerMeter = BMP::DPI_72;
  infoHeader.yPixelsPerMeter = BMP::DPI_72;
  infoHeader.colorsUsed      = BMP::COLORS_ALL;        
  infoHeader.colorsImportant = BMP::IGNORED;
}

void BMPImage::updateHeaders(int32_t width, int32_t height) {    
  infoHeader.width = width;
  infoHeader.height = height;
  infoHeader.pixelArraySize = getRowSize() * std::abs(height);
  fileHeader.fileSize = fileHeader.dataOffset + infoHeader.pixelArraySize;
}

void BMPImage::normalizeHeaders() {
  fileHeader.fileSize   = BMP::HEADERS_TOTAL_SIZE + fileHeader.fileSize - fileHeader.dataOffset;
  fileHeader.dataOffset = BMP::HEADERS_TOTAL_SIZE;

  infoHeader.headerSize = BMP::INFO_HEADER_SIZE;
}

template<typename T>
static void assert(T actual, T expected, const std::string& fieldName) {
  if (actual != expected) {
    throw std::runtime_error("invalid " + fieldName + "value: " + 
                            "expected " + std::to_string(expected) + ", " +
                            "got " + std::to_string(actual));
    }
}

void BMPImage::validateHeaders() const {
  assert(fileHeader.signature, BMP::SIGNATURE, "BMP signature");
  
  if (infoHeader.headerSize < static_cast<uint32_t>(BMP::INFO_HEADER_SIZE)) {
    throw std::runtime_error("invalid INFO_HEADER_SIZE: " + 
                              std::to_string(infoHeader.headerSize) +
                             ". Versions below BITMAPINFOHEADER are not supported.");
  }

  assert(infoHeader.planes, BMP::PLANES, "planes");

  assert(infoHeader.bitsPerPixel, BMP::BITS_PER_PIXEL, "bits per pixel");

  assert(infoHeader.compression, BMP::COMPRESSION_NONE, "compression");

  if (infoHeader.width < 0) {
    throw std::runtime_error("invalid width: " + std::to_string(infoHeader.width) + 
                              " (cannot be negative)");
  }

  if (infoHeader.width > 0 && infoHeader.height != 0) {
    uint32_t expectedPixelArraySize = BMP::calculateRowSize(infoHeader.width) * std::abs(infoHeader.height);
    assert(infoHeader.pixelArraySize, expectedPixelArraySize, "pixel array size");
  } else if (infoHeader.pixelArraySize != 0) {
    throw std::runtime_error("pixel array size should be 0 for empty image");
  }

  assert(infoHeader.colorsUsed, BMP::COLORS_ALL, "color used");
}

BMPImage::BMPImage(int w, int h) {
  if (w < 0) {
    throw std::invalid_argument("width cannot be negative");
  }
  
  initializeHeaders();
  updateHeaders(w, h);

  pixelArray = std::make_unique<std::byte[]>(getPixelArraySize());
}

BMPImage::BMPImage(const std::string &filename) {
  try {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
      throw std::runtime_error("cannot open file");
    }
    
    if (!file.read(reinterpret_cast<char*>(&fileHeader), BMP::FILE_HEADER_SIZE)) {
      throw std::runtime_error("failed to read file header");
    }
    
    if (!file.read(reinterpret_cast<char*>(&infoHeader), BMP::INFO_HEADER_SIZE)) {
      throw std::runtime_error("failed to read info header");
    }
    
    validateHeaders();
    
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();

    if (fileSize < fileHeader.dataOffset + infoHeader.pixelArraySize) {
      throw std::runtime_error("file is truncated");
    }
    
    file.seekg(fileHeader.dataOffset, std::ios::beg);
    if (!file) {
      throw std::runtime_error("failed to seek to pixel data");
    }

    pixelArray = std::make_unique<std::byte[]>(getPixelArraySize());

    if (!file.read(reinterpret_cast<char*>(pixelArray.get()), getPixelArraySize())) {
      throw std::runtime_error("failed to read pixel data");
    }
    if (file.gcount() != static_cast<std::streamsize>(getPixelArraySize())) {
      throw std::runtime_error("incomplete pixel data read");
    }
    
    file.close();
    
    normalizeHeaders();

  } catch (const std::exception& e) {
    pixelArray.reset();
    fileHeader = {0};
    infoHeader = {0};

    throw std::runtime_error("failed to load BMP file '" + filename + "': " + e.what());
  }
}

BMPImage::BMPImage(const BMPImage &other) : 
  fileHeader(other.fileHeader),
  infoHeader(other.infoHeader)
{
  pixelArray = std::make_unique<std::byte[]>(getPixelArraySize());
  std::memcpy(pixelArray.get(), other.pixelArray.get(), getPixelArraySize());
}

BMPImage &BMPImage::operator=(const BMPImage &other) {
  if (this != &other) {
    fileHeader = other.fileHeader;
    infoHeader = other.infoHeader;
    if (other.pixelArray) {
      pixelArray = std::make_unique<std::byte[]>(getPixelArraySize());
      std::memcpy(pixelArray.get(), other.pixelArray.get(), getPixelArraySize());
    } else {
      pixelArray.reset();
    }  
  }
  return *this;
}

void BMPImage::save(const std::string &filename) const {
  try {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
      throw std::runtime_error("failed to create file");
    }

    file.write(reinterpret_cast<const char*>(&fileHeader), BMP::FILE_HEADER_SIZE);
    file.write(reinterpret_cast<const char*>(&infoHeader), BMP::INFO_HEADER_SIZE);
    file.write(reinterpret_cast<const char*>(pixelArray.get()), getPixelArraySize());
    
    file.close();
} catch (const std::exception& e) {
    throw std::runtime_error("failed to save BMP file '" + filename + "': " + e.what());
  }
}

BMP::Pixel& BMPImage::getPixel(int x, int y) {
  if (!pixelArray) {
    throw std::runtime_error("pixel data not allocated");
  }
  if (!(0 <= x && x < getWidth() && 0 <= y && y < getHeight())) {
    throw std::out_of_range("Pixel coordinates out of range");
  }
  int yIndex = (getRawHeight() > 0) ? (getHeight() - 1 - y) : y;
  return reinterpret_cast<BMP::Pixel&>(pixelArray[yIndex * getRowSize() + x * sizeof(BMP::Pixel)]);
}

const BMP::Pixel &BMPImage::getPixel(int x, int y) const {
  if (!pixelArray) {
    throw std::runtime_error("pixel data not allocated");
  }
  if (!(0 <= x && x < getWidth() && 0 <= y && y < getHeight())) {
    throw std::out_of_range("Pixel coordinates out of range");
  }
  int yIndex = (getRawHeight() > 0) ? (getHeight() - 1 - y) : y;
  return reinterpret_cast<BMP::Pixel&>(pixelArray[yIndex * getRowSize() + x * sizeof(BMP::Pixel)]);
}

void BMPImage::setPixel(int x, int y, const BMP::Pixel &pixel) {
  if (!pixelArray) {
    throw std::runtime_error("pixel data not allocated");
  }
  if (!(0 <= x && x < getWidth() && 0 <= y && y < getHeight())) {
    throw std::out_of_range("Pixel coordinates out of range");
  }
  int yIndex = (getRawHeight() > 0) ? (getHeight() - 1 - y) : y;
  reinterpret_cast<BMP::Pixel&>(pixelArray[yIndex * getRowSize() + x * sizeof(BMP::Pixel)]) = pixel;
}
