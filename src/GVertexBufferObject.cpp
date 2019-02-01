#include "GVertexBufferObject.h"
#include "CLog.h"
#include "StringFormat.h"

#include "GVertex_T.h"
#include "StringFormat.h"
#include "GReturnCode.h"
#include "GShaderHandle_T.h"

#include <string>

#include "ImageLoader.h"
using util::ImageLoader;


using glw::engine::buffers::GVertexBufferObject;
using glw::engine::buffers::GVertex_T;
using glw::GReturnCode::GLW_FAIL;
using glw::GReturnCode::GLW_SUCCESS;
using glw::engine::glsl::GShaderHandle_T;
using util::StringFormat;


namespace
{
  const char * TRG = "GVBO";
  const char * __CLASSNAME__ = "GVertexBufferObject";
}


GVertexBufferObject::GVertexBufferObject()
{
}

// Texture filename, Vertex data pack, world position, 
// dynamic axis of rotation, and amount, static axis of rotation, 
// and amount, scale vector. 
GVertexBufferObject::GVertexBufferObject(
    std::vector<GVertex_T>& data,
    glm::vec3 pos,
    glm::vec3 rotation,
    GLfloat theta,
    glm::vec3 preRotation,
    GLfloat preTheta,
    glm::vec3 scale,
    const char * texfilename)
  :
    m_pos(pos),
    m_rotation(rotation),
    m_theta(theta),
    m_scale(scale),
    m_preRotation(preRotation),
    m_preTheta(preTheta)
{
  CINFO(TRG, "Loading new GVertexBufferObject...");

  loadTextures(texfilename);

  if (GLW_SUCCESS != init(&data))
	{
    CERROR(TRG, "Failed it initialise GVertexBufferObject.",
	      __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
}

GVertexBufferObject::~GVertexBufferObject()
{
}

// Buffers Vertex data into the VBO
GReturnCode GVertexBufferObject::init(std::vector<GVertex_T> * d)
{
  m_dataSize = d->size();
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, m_dataSize * sizeof(struct GVertex_T), d->data(), GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(struct GVertex_T),
		(const GLvoid*)offsetof(struct GVertex_T, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(struct GVertex_T),
		(const GLvoid*)offsetof(struct GVertex_T, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(struct GVertex_T),
		(const GLvoid*)offsetof(struct GVertex_T, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer((GLuint)3, 2, GL_FLOAT, GL_FALSE, sizeof(struct GVertex_T),
		(const GLvoid*)offsetof(struct GVertex_T, uv));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer((GLuint)4, 3, GL_FLOAT, GL_FALSE, sizeof(struct GVertex_T),
		(const GLvoid*)offsetof(struct GVertex_T, tangent));
	glEnableVertexAttribArray(4);
	glBindVertexArray(0);

	CINFO(TRG, "Buffered into VAO");
	
	return GLW_SUCCESS;
}

// Loads image file into a texture
void GVertexBufferObject::loadTextures(const char *texfilename)
{
  const std::string filename(texfilename);
  if (filename.compare("NULL") != 0)
  {
    m_tex = ImageLoader::loadTextureFromImage(texfilename);
    CINFO(TRG, StringFormat("%0 -> Texture Id %1").arg(texfilename).arg(m_tex).str());
  }
  else
  {
//    m_tex = ImageLoader::loadBlankTexture();
    CINFO(TRG, "No texture file loaded");
  }
}

// Draws the mesh including linking the model matrix
void GVertexBufferObject::draw(int wireFrame, GShaderHandle_T handles)
{
  if(NULL != 	handles.modelMatHandle)
  {
    handles.modelMatHandle->load(getModelMat());
  }
  else
  {
    CINFO(TRG, "NULL model");
  }
  if(NULL != handles.textureHandle)
  {
     handles.textureHandle->load(m_tex);
  }
  else
  {
    CINFO(TRG, "NULL texture");
  }
  drawArray(wireFrame);
}

// Draws just the VBO and activating the texture
void GVertexBufferObject::drawArray(int wireFrame)
{
  // load the texture
  glActiveTexture(GL_TEXTURE0 + m_tex);
  glBindTexture(GL_TEXTURE_2D, m_tex);

	// draw the data
	glBindVertexArray(m_vao);
	glDrawArrays(wireFrame ? GL_LINE_LOOP : GL_TRIANGLES, 0, m_dataSize);
	glBindVertexArray(0);

  // unload the texture
  glActiveTexture(GL_TEXTURE0 + m_tex);
  glBindTexture(GL_TEXTURE_2D, NULL);
}

// Get the model matrix
glm::mat4 GVertexBufferObject::getModelMat()
{
	return glm::translate(glm::mat4(1.), m_pos) *
		glm::rotate(glm::mat4(1.), m_theta, m_rotation) *
		glm::rotate(glm::mat4(1.), m_preTheta, m_preRotation) *
		glm::scale(glm::mat4(1.), m_scale);
}
