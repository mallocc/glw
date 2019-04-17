#include "TextureManager.h"
#include "Logger.h"
#include "StringFormat.h"
#include "ImageLoader.h"

using util::StringFormat;
using glw::engine::TextureManager;
using glw::engine::Texture;
using util::ImageLoader;


namespace
{
  const char * TRG = "";
  const char * __CLASSNAME__ = "TextureManager";
}


TextureManager::TextureManager()
  : m_textureMap()
{
}


TextureManager::~TextureManager()
{
}

bool TextureManager::importTexture(std::string filename, std::string id)
{
  bool success = true;

  if (filename != "")
  {
    Texture tex;
    tex.filename = filename;
    tex.shaderId = ImageLoader::loadTextureFromImage(filename.c_str());
    tex.textureId = id;
    LINFO(StringFormat("Imported texture: filename=%0 shaderId=%1 textureId=%2")
          .arg(tex.filename).arg(tex.shaderId).arg(tex.textureId).str());
  }
  else
  {
    LINFO(StringFormat("Couldn't find texture file '%0' to import").arg(filename).str());
  }

  return success;
}

bool TextureManager::addTexture(std::string textureId, GLuint glId, std::string filename)
{
  bool success = true;

  return success;
}

bool TextureManager::releaseTexture(std::string textureId)
{
  bool success = true;

  return success;
}

GLuint TextureManager::getTexture(std::string textureId)
{

}
