#include "GMesh.h"
#include "CLog.h"

#include "GVertex_T.h"
#include "StringFormat.h"
#include "GReturnCode.h"
#include "GMeshShaderHandle_T.h"


using glw::engine::buffers::GMesh;
using glw::engine::buffers::GVertex_T;
using glw::GReturnCode::GLW_FAIL;
using glw::GReturnCode::GLW_SUCCESS;
using glw::engine::glsl::GMeshShaderHandle_T;


namespace
{
  const char * TRG = "GMSH";
  const char * __CLASSNAME__ = "GMesh";
}


GMesh::GMesh()
{
}

// Texture filename, Vertex data pack, world position, 
// dynamic axis of rotation, and amount, static axis of rotation, 
// and amount, scale vector. 
GMesh::GMesh(
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
	CINFO(TRG, "Loading new Mesh...");

#ifdef USE_TEXTURES
  load_textures(texfilename);
#endif

	if (GLW_SUCCESS != init(&data))
	{
	  CERROR(TRG, "Failed it initialise GMesh.", 
	      __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
}

GMesh::~GMesh()
{
}

// Buffers Vertex data into the VBO
GReturnCode GMesh::init(std::vector<GVertex_T> * d)
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
	glFlush();

	CINFO(TRG, "Buffered into VAO");
	
	return GLW_SUCCESS;
}

// Draws the mesh including linking the model matrix
void GMesh::draw(int wireFrame, GMeshShaderHandle_T handles)
{
	handles.modelMatHandle->load(getModelMat());
	drawArray(wireFrame, handles.textureHandle);
}

// Draws just the VBO and activating the texture
void GMesh::drawArray(int wireFrame, GShaderVariableHandle * textureHandle)
{
#ifdef USE_TEXTURES
	// load the textures
  if (m_tex != GL_TEXTURE0)
  {
    load_texture_handle(texture_handle);
    glActiveTexture(GL_TEXTURE0 + m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
  }
#endif

	// draw the data
	glBindVertexArray(m_vao);
	glDrawArrays(wireFrame ? GL_LINE_LOOP : GL_TRIANGLES, 0, m_dataSize);
	glBindVertexArray(0);

#ifdef USE_TEXTURES
  // unload the texture
  if (m_tex != GL_TEXTURE0)
  {
    glActiveTexture(GL_TEXTURE0 + m_tex);
    glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);
  }
  glActiveTexture(GL_TEXTURE0);
#endif

	glFinish();
}

// Get the model matrix
glm::mat4 GMesh::getModelMat()
{
	return glm::translate(glm::mat4(1.), m_pos) *
		glm::rotate(glm::mat4(1.), m_theta, m_rotation) *
		glm::rotate(glm::mat4(1.), m_preTheta, m_preRotation) *
		glm::scale(glm::mat4(1.), m_scale);
}
