#pragma once

#include "opengl.h"
#include <map>


namespace glw
{
  namespace engine
  {
    struct Texture
    {
      GLuint shaderId;
      std::string textureId;
      std::string filename;
    };

    class TextureManager
    {
    public:

      TextureManager();
      
      ~TextureManager();

      bool importTexture(std::string filename, std::string id);

      bool addTexture(std::string textureId, GLuint glId, std::string filename = "");

      bool releaseTexture(std::string textureId);

      GLuint getTexture(std::string textureId);

    private:
      
      std::map<std::string, Texture> m_textureMap;

    };
  }
}
