#pragma once

#include "opengl.h"

#define NULL_IMAGE "NULL_IMAGE"

namespace util
{
  struct ImageData_T
  {
    unsigned char * data;
    int w, h, n;
  };

  class ImageLoader
  {
  public:
    
    ImageLoader();
    
    ~ImageLoader();

    static GLuint loadTextureFromImage(const char *filename);
    static GLuint loadBlankTexture();
    static GLuint loadUniformColourTexture(int r, int g, int b);
    static void getData(ImageData_T& imageData, const char * filename);
    static void getDataPNG(ImageData_T& imageData, const char * filename);
    static void loadImageData(ImageData_T& imageData, unsigned char * data, int w, int h, int n);
    
  private:

    static GLuint getTexId(unsigned char * data, int w, int h);
    
  };
}
