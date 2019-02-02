#include "GShaderProgram.h"
#include "Logger.h"
#include "StringFormat.h"
#include "GReturnCode.h"
#include <fstream>

using glw::engine::glsl::GShaderProgram;
using glw::engine::glsl::GShaderProgramId;
using glw::engine::glsl::GShaderVariableHandle;
using glw::engine::glsl::GShaderVariableHandleId;
using glw::engine::glsl::GShaderHandle_T;
using util::StringFormat;
using glw::GReturnCode::GLW_SUCCESS;
using glw::GReturnCode::GLW_FAIL;


namespace
{
  const char * TRG = "GSPR";
  const char * __CLASSNAME__ = "GShaderProgram";
  
  const char * VAR_NAME_MODEL_MAT = "u_m";
	const char * VAR_NAME_VIEW_MAT  = "u_v";
	const char * VAR_NAME_PROJ_MAT  = "u_p";
	const char * VAR_NAME_COLOR_VEC = "u_c";
	const char * VAR_NAME_TEX0      = "u_tex";
	const char * VAR_NAME_TEX1		  = "u_tex1";
	const char * VAR_NAME_FLAG      = "u_flag";
}


GShaderProgram::GShaderProgram()
{
}

GShaderProgram::GShaderProgram(const char * vertexFilePath,	const char * fragmentFilePath) 
    :
    m_valid(true)
{

  GReturnCode returnCode = GLW_SUCCESS;

	LINFO(TRG, "Creating new program...");

	this->m_vertexFilePath = vertexFilePath;
	this->m_fragmentFilePath = fragmentFilePath;

	// Create the shaders
	m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
	  returnCode = GLW_FAIL;
		LERROR(TRG, StringFormat("Failed to open %0.")
			.arg(vertexFilePath).str(), __FILE__, __LINE__, __CLASSNAME__, __func__);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragmentFilePath, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
	  returnCode = GLW_FAIL;
		LERROR(TRG, StringFormat("Failed to open %0.")
			.arg(fragmentFilePath).str(), __FILE__, __LINE__, __CLASSNAME__, __func__);
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(m_vertexShaderId, 1, &VertexSourcePointer, NULL);
	glCompileShader(m_vertexShaderId);

	// Check Vertex Shader
	glGetShaderiv(m_vertexShaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(m_vertexShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
	  returnCode = GLW_FAIL;
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(m_vertexShaderId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		LERROR(TRG, StringFormat("Vertex shader compilation failed: \n%0")
			.arg(&VertexShaderErrorMessage[0]).str(), __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
	else
		LINFO(TRG, StringFormat("%0 compiled succesfully.").arg(vertexFilePath).str());

	// Compile Fragment Shader
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(m_fragmentShaderId, 1, &FragmentSourcePointer, NULL);
	glCompileShader(m_fragmentShaderId);

	// Check Fragment Shader
	glGetShaderiv(m_fragmentShaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(m_fragmentShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
	  returnCode = GLW_FAIL;
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(m_fragmentShaderId, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		LERROR(TRG, StringFormat("Fragment shader compilation failed: \n%0")
			.arg(&FragmentShaderErrorMessage[0]).str(), __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
	else
		LINFO(TRG, StringFormat("%0 compiled succesfully.").arg(fragmentFilePath).str());

	// Link the program
	GLuint ProgramId = glCreateProgram();
	glAttachShader(ProgramId, m_vertexShaderId);
	glAttachShader(ProgramId, m_fragmentShaderId);
	glLinkProgram(ProgramId);

	// Check the program
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
	  returnCode = GLW_FAIL;
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		LERROR(TRG, StringFormat("Linking failed:\n%0\n")
			.arg(&ProgramErrorMessage[0]).str(), __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
	else
		LINFO(TRG, "Program linked succesfully.");


	glDetachShader(ProgramId, m_vertexShaderId);
	glDetachShader(ProgramId, m_fragmentShaderId);

	glDeleteShader(m_vertexShaderId);
	glDeleteShader(m_fragmentShaderId);

	this->m_id = ProgramId;

  if (GLW_FAIL == returnCode)
  {
    m_valid = false;
    LERROR(TRG, StringFormat("Loaded GShaderProgram %0 unsuccessfully.").arg(ProgramId).str(),
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }
  else
  {
    LINFO(TRG, StringFormat("Loaded GShaderProgram %0 succesfully.").arg(ProgramId).str());
  }
}

GShaderProgram::~GShaderProgram()
{
}

GReturnCode GShaderProgram::addHandle(GShaderVariableHandle handle)
{
	GReturnCode success = handle.init(m_id);
	if(GLW_SUCCESS == success)
	{
	  m_handles.insert({ handle.getHandleId(), handle });
	}
	else
	{
	  LERROR(TRG, StringFormat("Failed to add handle.").str(),
	       __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
	return success;
}

void GShaderProgram::load()
{
	glUseProgram(m_id);
	for (auto& sm_pair : m_handles)
	{
		sm_pair.second.load();
  }
}


GShaderVariableHandle * GShaderProgram::getHandle(GShaderVariableHandleId id)
{
  GShaderVariableHandle * handle = NULL;
  if (m_handles.find(id) != m_handles.end())
  {
    handle = &m_handles[id];
  }
	return handle;
}

GShaderProgramId GShaderProgram::getId()
{
	return m_id;
}

GReturnCode GShaderProgram::setModelMat4Handle(glm::mat4 * mat)
{
	GShaderVariableHandle handle = GShaderVariableHandle(VAR_NAME_MODEL_MAT, mat);
	GReturnCode success = handle.init(m_id);
	if(GLW_SUCCESS == success)
	{
	  m_modelMat = handle.getHandleId();
	  m_handles.insert({ handle.getHandleId(), handle });
	}
	else
	{
	  LERROR(TRG, StringFormat("Failed to set ModelMat4Handle.").str(),
	       __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
	return success;
}

GReturnCode GShaderProgram::setViewMat4Handle(glm::mat4 * mat)
{
	GShaderVariableHandle handle = GShaderVariableHandle(VAR_NAME_VIEW_MAT, mat);
	GReturnCode success = handle.init(m_id);
	if(GLW_SUCCESS == success)
	{
	  m_viewMat = handle.getHandleId();
	  m_handles.insert({ handle.getHandleId(), handle });
	}
	else
	{
	  LERROR(TRG, StringFormat("Failed to set ViewMat4Handle.").str(),
	       __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
	return success;
}

GReturnCode GShaderProgram::setProjMat4Handle(glm::mat4 * mat)
{
	GShaderVariableHandle handle = GShaderVariableHandle(VAR_NAME_PROJ_MAT, mat);
	GReturnCode success = handle.init(m_id);
	if(GLW_SUCCESS == success)
	{
	  m_projMat = handle.getHandleId();
	  m_handles.insert({ handle.getHandleId(), handle });
	}
	else
	{
	  LERROR(TRG, StringFormat("Failed to set ProjMat4Handle.").str(),
	       __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
	return success;
}

GReturnCode GShaderProgram::setTexHandle()
{
	GShaderVariableHandle handle = GShaderVariableHandle(VAR_NAME_TEX0);
	GReturnCode success = handle.init(m_id);
	if(GLW_SUCCESS == success)
	{
	  m_tex = handle.getHandleId();
	  m_handles.insert({ handle.getHandleId(), handle });
	}
	else
	{
	  LERROR(TRG, StringFormat("Failed to set TexHandle.").str(),
	       __FILE__, __LINE__, __CLASSNAME__, __func__);
	}
	return success;
}

GShaderVariableHandle * GShaderProgram::getModelMat4Handle()
{
	return getHandle(m_modelMat);
}

GShaderVariableHandle * GShaderProgram::getViewMat4Handle()
{
	return getHandle(m_viewMat);
}

GShaderVariableHandle * GShaderProgram::getProjMat4Handle()
{
	return getHandle(m_projMat);
}

GShaderVariableHandle * GShaderProgram::getTexHandle()
{
	return getHandle(m_tex);
}

GShaderHandle_T GShaderProgram::getShaderHandle()
{
  return {
    getTexHandle(),
        getModelMat4Handle(),
        getViewMat4Handle(),
        getProjMat4Handle()};
}

void GShaderProgram::getShaderHandle(GShaderHandle_T& shaderHandle)
{
  shaderHandle.textureHandle = getTexHandle();
  shaderHandle.modelMatHandle = getModelMat4Handle();
  shaderHandle.viewMatHandle = getViewMat4Handle();
  shaderHandle.projMatHandle = getProjMat4Handle();
}
			  
bool GShaderProgram::isValid() const
{
  return m_valid;
}
