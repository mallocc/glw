#include "GFrameBufferObject.h"
#include "GPrimativeFactory.h"

#include "Logger.h"
#include "StringFormat.h"

#include "GReturnCode.h"


using glw::engine::buffers::GFrameBufferObject;
using util::StringFormat;
using glw::engine::buffers::GPrimativeFactory;
using glw::engine::buffers::GArrayVec2;
using glw::engine::buffers::GArrayVec3;
using glw::engine::buffers::GArrayVertex;
using glw::GReturnCode;
using glw::GLW_SUCCESS;
using glw::GLW_FAIL;


namespace
{
  const char * TRG = "GFBO";
  const char * __CLASSNAME__ = "GFrameBufferObject";
}


GFrameBufferObject::GFrameBufferObject()
{
}

// Frame size
GFrameBufferObject::GFrameBufferObject(int w, int h)
{
  this->m_width = w; this->m_height = h;
  createFBO();
  createVBO();
}
// Frame size
GFrameBufferObject::GFrameBufferObject(glm::vec2 windowSize)
{
  this->m_width = windowSize.x; this->m_height = windowSize.y;
  createFBO();
  createVBO();
}

GFrameBufferObject::~GFrameBufferObject()
{
}

void GFrameBufferObject::draw(GShaderHandle_T handles)
{
  m_vbo.m_tex = m_tex;
  m_vbo.draw(handles);
}

// Binds FBO for render
void GFrameBufferObject::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);
  //glClearDepth(1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
  glBlendEquation(GL_FUNC_ADD);
  glViewport(0, 0, m_width, m_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, m_width, 0.0, m_height, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Unbinds all FBOs
void GFrameBufferObject::unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GFrameBufferObject::setTopLeft(glm::vec2 topLeft)
{
  m_vbo.m_pos.x = topLeft.x;
  m_vbo.m_pos.y = topLeft.y;
}

void GFrameBufferObject::setSize(glm::vec2 size)
{
  m_vbo.m_scale.x = size.x;
  m_vbo.m_scale.y = size.y;
}

void GFrameBufferObject::setZ(float z)
{
  m_vbo.m_pos.z = z;
}

void GFrameBufferObject::rotation(float rotation)
{
  m_vbo.m_theta = rotation;
}

void GFrameBufferObject::setTex(GLuint tex)
{
  m_tex = tex;
}

GReturnCode GFrameBufferObject::createFBO()
{
  GReturnCode retCode = GLW_SUCCESS;

  LINFO(TRG, "Creating new FBO...", __CLASSNAME__, __func__);

  glGenFramebuffers(1, &m_id);
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);

  //RGBA8 2D texture, 24 bit depth texture
  glGenTextures(1, &m_tex);
  glBindTexture(GL_TEXTURE_2D, m_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //NULL means reserve texture memory, but texels are undefined
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  //Attach 2D texture to this FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);

  //RGBA8 2D texture, 24 bit depth texture
  glGenTextures(1, &m_depth);
  glBindTexture(GL_TEXTURE_2D, m_depth);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //NULL means reserve texture memory, but texels are undefined
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, m_depth, 0);

  GLuint depthBuffer;
  // The depth buffer
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
  //Attach depth buffer to FBO
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

  //Does the GPU support current FBO configuration?

  GLenum status;
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch (status)
  {
  case GL_FRAMEBUFFER_COMPLETE:
    break;
  default:
    retCode = GLW_FAIL;
    LERROR(TRG, StringFormat("Error creating FBO").str(), __FILE__, __LINE__, __CLASSNAME__, __func__);
  }

  LINFO(TRG, StringFormat("Successfully created FBO = %0").arg(m_id).str(), __CLASSNAME__, __func__);
  return retCode;
}

void GFrameBufferObject::createVBO()
{
  GArrayVec3 v, c, n, t;
  GPrimativeFactory::centeredSquareMesh(v, 1, 1);

  GArrayVec2 uv;
  GPrimativeFactory::squareMeshUVs(uv, 1, 1);

  GArrayVertex o;
  GPrimativeFactory::packObject(o, v, c, n, t, uv);

  m_vbo = GVertexBufferObject(o,
                glm::vec3(0, 0, 0),
                glm::vec3(0, 0, 1),	glm::radians(0.0f),
                glm::vec3(0, 1, 0),	glm::radians(0.0f),
                glm::vec3(m_width, m_height, 1));
}
