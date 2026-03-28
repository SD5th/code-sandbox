#ifndef BMP_FORMAT_H

#include <cstdint>
#include <cstddef>

namespace BMP {

  #pragma pack(push, 1) 

  struct FileHeader {
    uint16_t signature;      // The header field used to identify the BMP and DIB file is 0x42 0x4D in hexadecimal, same as BM in ASCII
    uint32_t fileSize;       // The size of the BMP file in bytes
    uint16_t reserved1;      // Reserved; actual value depends on the application that creates the image, if created manually can be 0
    uint16_t reserved2;      // Reserved; actual value depends on the application that creates the image, if created manually can be 0.
    uint32_t dataOffset;     // The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found
  };

  struct InfoHeader {
    uint32_t headerSize;     // The size of this header, in bytes (40) 
    int32_t  width;          // The bitmap width in pixels (signed integer) 
    int32_t  height;         // The bitmap height in pixels (signed integer) 
    uint16_t planes;         // The number of color planes (must be 1) 
    uint16_t bitsPerPixel;   // the number of bits per pixel, which is the color depth of the image 
    uint32_t compression;    // The compression method being used 
    uint32_t pixelArraySize; // The size of the raw bitmap data
    int32_t  xPixelsPerMeter;// The horizontal resolution of the image. (pixel per metre, signed integer) 
    int32_t  yPixelsPerMeter;// The vertical resolution of the image. (pixel per metre, signed integer) 
    uint32_t colorsUsed;     // The number of colors in the color palette, or 0 to default to 2^n
    uint32_t colorsImportant;// The number of important colors used, or 0 when every color is important; generally ignored 
  };

  struct Pixel {
    uint8_t blue;   
    uint8_t green;  
    uint8_t red;    
  };

  #pragma pack(pop)

  // BMP constants
  static constexpr uint16_t SIGNATURE = 0x4D42;   // "BM"
  static constexpr uint16_t PLANES = 1;           // always 1
  static constexpr uint16_t BITS_PER_PIXEL = 24;  // RGB (3*8)
  static constexpr uint32_t COMPRESSION_NONE = 0; // no compression
  static constexpr int32_t  DPI_72 = 2835;        // 72 DPI in pixels per meter
  static constexpr uint32_t COLORS_ALL = 0;       // every RGB color can be used
  
  // Header sizes
  static constexpr size_t FILE_HEADER_SIZE = sizeof(FileHeader);
  static constexpr size_t INFO_HEADER_SIZE = sizeof(InfoHeader);
  static constexpr size_t HEADERS_TOTAL_SIZE = FILE_HEADER_SIZE + INFO_HEADER_SIZE;

  // Other
  static constexpr int IGNORED = 0;
  static constexpr int PADDING = 4; //  The size of each row is rounded up to a multiple of 4 bytes (a 32-bit DWORD) by padding.
  static constexpr int BYTES_PER_PIXEL = BITS_PER_PIXEL / 8; 

  inline int32_t calculateRowSize(int32_t width) { return ((width * BYTES_PER_PIXEL) + (PADDING - 1)) & ~(PADDING - 1); }  

}

#endif