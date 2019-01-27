#include "GShaderVariableHandle.h"
#include "CLog.h"
#include "StringFormat.h"

using glw::engine::glsl::GShaderVariableHandle;
using glw::engine::glsl::GShaderVariableHandleId;
using util::StringFormat;

namespace
{ 
  const char * TRG = "GSVR";
  const char * __CLASSNAME__ = "GShaderVariableHandle";
  
  enum
	{
		NO_HANDLE,
		MAT4_HANDLE,
		VEC3_HANDLE,
		VEC4_HANDLE,
		FLOAT_HANDLE,
		GLUINT_HANDLE,
		INT_HANDLE
	};
	
	const GLuint NULL_UNIFORM_LOCATION = -1;
}


GShaderVariableHandle::GShaderVariableHandle()
{ 
}

GShaderVariableHandle::GShaderVariableHandle(const char * variableIdentifier)
    :
    m_type(NO_HANDLE),
    m_variableIdentifier(variableIdentifier)
{
}

GShaderVariableHandle::GShaderVariableHandle(const char * variableIdentifier, glm::mat4 * data)
    :
    m_type(MAT4_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_m4(data)
{
}

GShaderVariableHandle::GShaderVariableHandle(const char * variableIdentifier, glm::vec3 * data)
    :
    m_type(VEC3_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_v3(data)
{
}
GShaderVariableHandle::GShaderVariableHandle(const char * variableIdentifier, glm::vec4 * data)
    :
    m_type(VEC4_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_v4(data)
{
}

GShaderVariableHandle::GShaderVariableHandle(const char * variableIdentifier, GLfloat   * data)
    :
    m_type(FLOAT_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_glf(data)
{
}

GShaderVariableHandle::GShaderVariableHandle(const char * variableIdentifier, GLuint    * data)
    :
    m_type(GLUINT_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_gli(data)
{
}

GShaderVariableHandle::GShaderVariableHandle(const char * variableIdentifier, int       * data)
    :
    m_type(INT_HANDLE),
    m_variableIdentifier(variableIdentifier),
    m_data_i(data)
{
}

GShaderVariableHandle::~GShaderVariableHandle()
{
}

GReturnCode GShaderVariableHandle::init(const GLuint program)
{
  GReturnCode success = GLW_SUCCESS;

	m_id = glGetUniformLocation(program, m_variableIdentifier);
	if (NULL_UNIFORM_LOCATION == m_id)
	{
	  success = GLW_FAIL;
	  CERROR(TRG, StringFormat("GShaderProgram Id %0, failed linking %1")
	    .arg(program).arg(m_variableIdentifier).str(),
	    __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
	else
	{
	  CINFO(TRG, StringFormat("GShaderProgram Id %0, linking %1 to GShaderVariableHandleId %2")
	      .arg(program).arg(m_variableIdentifier).arg(m_id).str());
  }
  
  return success;
}

void GShaderVariableHandle::load()
{
  switch (m_type)
  {
  case MAT4_HANDLE:
			glUniformMatrix4fv(m_id, 1, GL_FALSE, glm::value_ptr(*m_data_m4));
			break;
	case VEC3_HANDLE:
			glUniform3f(m_id, m_data_v3->x, m_data_v3->y, m_data_v3->z);
			break;
	case VEC4_HANDLE:
			glUniform4f(m_id, m_data_v4->x, m_data_v4->y, m_data_v4->z, m_data_v4->w);
	  break;
	case FLOAT_HANDLE:
			glUniform1f(m_id, *m_data_glf);
	  break;
	case GLUINT_HANDLE:
			glUniform1i(m_id, *m_data_gli);
	  break;
	case INT_HANDLE:
			glUniform1i(m_id, *m_data_i);
	  break;
	case NO_HANDLE:
  default:
//    CERROR(TRG, StringFormat("GShaderVariableHandle %0 has not been constructed properly.")
//        .arg(m_variableIdentifier).str(),
//        __FILE__, __LINE__, __CLASSNAME__, __func__);
    break;
  }
}

void GShaderVariableHandle::load(glm::mat4 data)
{
	glUniformMatrix4fv(m_id, 1, GL_FALSE, glm::value_ptr(data));
}

void GShaderVariableHandle::load(glm::vec3 data)
{
	glUniform3f(m_id, data.x, data.y, data.z);
}

void GShaderVariableHandle::load(glm::vec4 data)
{
	glUniform4f(m_id, data.x, data.y, data.z, data.w);
}

void GShaderVariableHandle::load(GLfloat data)
{
	glUniform1f(m_id, data);
}

void GShaderVariableHandle::load(GLuint data)
{
	glUniform1i(m_id, data);
}

void GShaderVariableHandle::load(int data)
{
	glUniform1i(m_id, data);
}

glm::mat4 * GShaderVariableHandle::getMat4()
{
  return m_data_m4;
}

glm::vec3 * GShaderVariableHandle::getVec3()
{
  return m_data_v3;
}

glm::vec4 * GShaderVariableHandle::getVec4()
{
  return m_data_v4;
}

GLfloat   * GShaderVariableHandle::getGLfloat()
{
  return m_data_glf;
}

GLuint    * GShaderVariableHandle::getGLuint()
{
  return m_data_gli;
}

int       * GShaderVariableHandle::getInt()
{
  return m_data_i;
}

const char * GShaderVariableHandle::getVariableIdentifier()
{
  return m_variableIdentifier;
}	    

GShaderVariableHandleId GShaderVariableHandle::getHandleId()
{
  return m_id;
}
