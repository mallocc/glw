#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

using util::ImageLoader;
using util::ImageData_T;


namespace
{
  const char * TRG = "IMGL";
  const char * __CLASSNAME__ = "ImageLoader";
}


ImageLoader::ImageLoader()
{
}


ImageLoader::~ImageLoader()
{
}

GLuint ImageLoader::loadTextureFromImage(const char *filename)
{
  int w, h, n;
  unsigned char *data = stbi_load(filename, &w, &h, &n, STBI_rgb_alpha);
  if (data == NULL)
  {
    fprintf(stderr, "Image not loaded: %s\n", filename);
    const char *error = stbi_failure_reason();
    fprintf(stderr, "Failure reason %s\n", error);
    return GL_TEXTURE0;
  }

  GLuint tex = getTexId(data, w, h);
  stbi_image_free(data);

  return tex;
}

GLuint ImageLoader::loadBlankTexture()
{
  int w = 1, h = 1;
  unsigned char *data = new unsigned char[3 * w * h];
  for (int i = 0; i < 3 * w*h; ++i)
    data[i] = 0;

  GLuint tex = getTexId(data, w, h);
  stbi_image_free(data);

  return tex;
}

GLuint ImageLoader::loadUniformColourTexture(int r, int g, int b)
{
  int w = 1, h = 1;
  unsigned char *data = new unsigned char[3 * w * h];
  for (int i = 0; i < 3 * w*h; i += 3)
  {
    data[i] = r;
    data[i+1] = g;
    data[i+2] = b;
  }

  GLuint tex = getTexId(data, w, h);
  stbi_image_free(data);

  return tex;
}

void ImageLoader::getData(ImageData_T& imageData, const char * filename)
{
  int w, h, n;
  unsigned char * data = stbi_load(filename, &w, &h, &n, 0);
  loadImageData(imageData, data, w, h, n);
}

void ImageLoader::getDataPNG(ImageData_T& imageData, const char * filename)
{
  int w, h, n;
  unsigned char *data = stbi_load(filename, &w, &h, &n, STBI_rgb_alpha);
  loadImageData(imageData, data, w, h, n);
}

void ImageLoader::loadImageData(ImageData_T& imageData, unsigned char * data, int w, int h, int n)
{
  imageData.w = w;
  imageData.h = h;
  imageData.n = n;
  imageData.data = data;
}


GLuint ImageLoader::getTexId(unsigned char * data, int w, int h)
{
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  // set the texture wrapping parameters
  // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  return tex;
}
