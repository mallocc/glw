#include "GTextureManager.h"
#include "Logger.h"
#include "StringFormat.h"
#include "ImageLoader.h"

using util::StringFormat;
using glw::engine::GTextureManager;
using util::ImageLoader;


namespace
{
  const char * TRG = "";
  const char * __CLASSNAME__ = "TextureManager";
}


GTextureManager::GTextureManager()
  : m_textureMap()
{
}


GTextureManager::~GTextureManager()
{
}

GLuint GTextureManager::importTexture(std::string filename)
{
  GLuint id = NULL_TEX;

  if (filename != NULL_FILE)
  {
     id = ImageLoader::loadTextureFromImage(filename.c_str());

     if (id != NULL_TEX)
     {
       this->addTexture(filename, id);
     }
  }

  return id;
}

void GTextureManager::addTexture(std::string filename, GLuint id)
{
  m_textureMap[filename] = id;
}

void GTextureManager::releaseTexture(std::string filename)
{
  m_textureMap.erase(filename);
}

void GTextureManager::releaseTexture(GLuint id)
{
  std::map<std::string, GLuint>::iterator itor = m_textureMap.begin();

  bool found = false;

  while (itor != m_textureMap.end() && !found)
  {
    if (itor->second == id)
    {
      m_textureMap.erase(itor);
      found = true;
    }
    else
    {
      ++itor;
    }
  }
}

GLuint GTextureManager::getTextureId(std::string filename)
{
  GLuint id = NULL_TEX;

  std::map<std::string, GLuint>::iterator itor = m_textureMap.find(filename);

  if (itor != m_textureMap.end())
  {
    id = itor->second;
  }

  if (id == NULL_TEX)
  {
    id = importTexture(filename);
  }

  return id;
}

std::string  GTextureManager::getTextureFilename(GLuint id)
{
  std::string filename = NULL_FILE;

  std::map<std::string, GLuint>::iterator itor = m_textureMap.begin();

  bool found = false;

  while (itor != m_textureMap.end() && !found)
  {
    if (itor->second == id)
    {
      filename = itor->first;
      found = true;
    }
    else
    {
      ++itor;
    }
  }

  return filename;
}
