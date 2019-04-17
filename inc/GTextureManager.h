#pragma once

#include "opengl.h"
#include <map>


#define NULL_FILE ""
#define NULL_TEX 0U


namespace glw
{
  namespace engine
  {
    class GTextureManager
    {
    public:

      static GTextureManager& getInstance()
      {
          static GTextureManager instance;
          return instance;
      }

      GTextureManager();
      
      ~GTextureManager();

      GLuint importTexture(std::string filename);

      void addTexture(std::string filename, GLuint id);

      void releaseTexture(std::string filename);
      void releaseTexture(GLuint id);

      GLuint getTextureId(std::string filename);
      std::string  getTextureFilename(GLuint id);

    private:
      
      std::map<std::string, GLuint> m_textureMap;

    };
  }
}
