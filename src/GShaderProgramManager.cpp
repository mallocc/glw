#include "GShaderProgramManager.h"
#include "Logger.h"


using glw::engine::glsl::GShaderProgramManager;
using glw::engine::glsl::GShaderProgram;
using glw::GReturnCode::GLW_FAIL;
using glw::GReturnCode::GLW_SUCCESS;


namespace
{
  const char * TRG = "GSPM";
  const char * __CLASSNAME__ = "GShaderProgramManager";
}


GShaderProgramManager::GShaderProgramManager()
{
}


GShaderProgramManager::~GShaderProgramManager()
{
}

GShaderProgram * GShaderProgramManager::loadProgram(GShaderProgramId id)
{
	m_shaderPrograms[id].load();
	m_currentProgram = id;
	return getProgram(id);
}

GReturnCode GShaderProgramManager::addNewProgram(
  const char * vertexFilePath,
  const char * fragmentFilePath
)
{
	GShaderProgram program = GShaderProgram(vertexFilePath, fragmentFilePath);
	GReturnCode success = program.isValid() ? GLW_SUCCESS : GLW_FAIL;
	if (GLW_SUCCESS != success)
	{
    LERROR("GShaderProgram not added to GShaderProgramManager.");
	}
	else
	{
	  m_shaderPrograms.insert({ program.getId(), program });	  
    LINFO("GShaderProgram added to GShaderProgramManager.");
	}
	return success;
}

GReturnCode GShaderProgramManager::addNewProgram(
  const char * vertexFilePath,
  const char * fragmentFilePath,
  glm::mat4 * model_data,
  glm::mat4 * view_data,
  glm::mat4 * proj_data,
  GShaderProgramId& shaderProgramId
)
{
	GShaderProgram program = GShaderProgram(vertexFilePath, fragmentFilePath);
  GReturnCode success = program.isValid() ? GLW_SUCCESS : GLW_FAIL;
	if (GLW_SUCCESS != success)
	{
	  shaderProgramId = NULL_SHADER_PROGRAM_ID;
    LERROR("GShaderProgram not added to GShaderProgramManager.");
	}
	else
	{
    if (GLW_SUCCESS != program.setModelMat4Handle(model_data))
    {
      success = GLW_FAIL;
    }
    if (GLW_SUCCESS != program.setViewMat4Handle(view_data))
    {
      success = GLW_FAIL;
    }
    if (GLW_SUCCESS != program.setProjMat4Handle(proj_data))
    {
      success = GLW_FAIL;
    }
    if(GLW_SUCCESS == success)
    {
	    m_shaderPrograms.insert({ program.getId(), program });	  
      shaderProgramId = program.getId();
      LINFO("GShaderProgram added to GShaderProgramManager.");
   	}
	}
  return success;
}

GReturnCode GShaderProgramManager::addNewProgram(GShaderProgram program)
{
  GReturnCode success = program.isValid() ? GLW_SUCCESS : GLW_FAIL;
  if (GLW_SUCCESS != success)
  {
    LERROR("GShaderProgram not added to GShaderProgramManager.");
  }
  else
  {
    m_shaderPrograms.insert({ program.getId(), program });	  
    LINFO("GShaderProgram added to GShaderProgramManager.");
  }
  return success;
}

GShaderProgram * GShaderProgramManager::getProgram(GShaderProgramId id)
{
	return &m_shaderPrograms[id];
}

GShaderProgram * GShaderProgramManager::getCurrentProgram()
{
	return &m_shaderPrograms[m_currentProgram];
}

void GShaderProgramManager::drawVBO(GVertexBufferObject& vbo)
{
  GShaderProgram * shaderProgram = getCurrentProgram();
  if(NULL != shaderProgram)
  {
    vbo.draw(shaderProgram->getShaderHandle());
  }
  else
  {
    LINFO("NULL shaderProgram");
  }
}

void GShaderProgramManager::drawVBOs(std::vector<GVertexBufferObject*>& vbos)
{
  GShaderProgram * shaderProgram = getCurrentProgram();
  if(NULL != shaderProgram)
  {
    for (GVertexBufferObject * vbo : vbos)
    {
      vbo->draw(shaderProgram->getShaderHandle());
    }
  }
  else
  {
    LINFO("NULL shaderProgram");
  }
}

void GShaderProgramManager::drawVBOs(std::vector<GVertexBufferObject>& vbos)
{
  GShaderProgram * shaderProgram = getCurrentProgram();
  if(NULL != shaderProgram)
  {
    for (GVertexBufferObject& vbo : vbos)
    {
      vbo.draw(shaderProgram->getShaderHandle());
    }
  }
  else
  {
    LINFO("NULL shaderProgram");
  }
}

void GShaderProgramManager::drawFBO(GFrameBufferObject& fbo)
{
  GShaderProgram * shaderProgram = getCurrentProgram();
  if(NULL != shaderProgram)
  {
    fbo.draw(shaderProgram->getShaderHandle());
  }
}
