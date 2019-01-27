#include "GLSLProgram.h"
#include "CLog.h"
#include "StringFormat.h"

using gfx::engine::GLSLProgram;

namespace
{
	const char * CLASSNAME          = "GLSLProgram";
	const char * VAR_NAME_MODEL_MAT = "u_m";
	const char * VAR_NAME_VIEW_MAT  = "u_v";
	const char * VAR_NAME_PROJ_MAT  = "u_p";
	const char * VAR_NAME_COLOR_VEC = "u_c";
	const char * VAR_NAME_TEX0      = "u_tex";
	const char * VAR_NAME_TEX1		  = "u_tex1";
	const char * VAR_NAME_FLAG      = "u_flag";
}

GLSLProgram * GLSLProgram::addHandle(gfx::engine::VarHandle handle)
{
	handle.init(m_Id);
	m_handles.insert({ handle.get_handle_id(), handle });
	return this;
}

void GLSLProgram::load()
{
	glUseProgram(m_Id);
	for (auto& sm_pair : m_handles)
		sm_pair.second.load();
}

gfx::engine::VarHandle * GLSLProgram::getHandle(gfx::engine::VarHandleID id)
{
	return &m_handles[id];
}

gfx::engine::GLSLProgramID GLSLProgram::getId()
{
	return m_Id;
}

GLSLProgram * GLSLProgram::setModelMat4Handle(glm::mat4 * mat)
{
	VarHandle handle = VarHandle(VAR_NAME_MODEL_MAT, mat);
	handle.init(m_Id);
	m_modelMat = handle.get_handle_id();
	addHandle(handle);
	return this;
}

GLSLProgram * GLSLProgram::setViewMat4Handle(glm::mat4 * mat)
{
	VarHandle handle = VarHandle(VAR_NAME_VIEW_MAT, mat);
	handle.init(m_Id);
	m_viewMat = handle.get_handle_id();
	addHandle(handle);
	return this;
}

GLSLProgram * GLSLProgram::setProjMat4Handle(glm::mat4 * mat)
{
	VarHandle handle = VarHandle(VAR_NAME_PROJ_MAT, mat);
	handle.init(m_Id);
	m_projMat = handle.get_handle_id();
	addHandle(handle);
	return this;
}

GLSLProgram * GLSLProgram::setColorHandle()
{
	VarHandle handle = VarHandle(VAR_NAME_COLOR_VEC);
	handle.init(m_Id);
	m_color = handle.get_handle_id();
	addHandle(handle);
	return this;
}

GLSLProgram * GLSLProgram::setFlagHandle()
{
	VarHandle handle = VarHandle(VAR_NAME_FLAG);
	handle.init(m_Id);
	m_flag = handle.get_handle_id();
	addHandle(handle);
	return this;
}

GLSLProgram * GLSLProgram::setTexHandle()
{
	VarHandle handle = VarHandle(VAR_NAME_TEX0);
	handle.init(m_Id);
	m_tex = handle.get_handle_id();
	addHandle(handle);
	return this;
}
GLSLProgram * GLSLProgram::setTex1Handle()
{
	VarHandle handle = VarHandle(VAR_NAME_TEX1);
	handle.init(m_Id);
	m_tex1 = handle.get_handle_id();
	addHandle(handle);
	return this;
}

gfx::engine::VarHandle * GLSLProgram::getModelMat4Handle()
{
	return getHandle(m_modelMat);
}

gfx::engine::VarHandle * GLSLProgram::getViewMat4Handle()
{
	return getHandle(m_viewMat);
}

gfx::engine::VarHandle * GLSLProgram::getProjMat4Handle()
{
	return getHandle(m_projMat);
}

gfx::engine::VarHandle * GLSLProgram::getColorHandle()
{
	return getHandle(m_color);
}

gfx::engine::VarHandle * GLSLProgram::getFlagHandle()
{
	return getHandle(m_flag);
}

gfx::engine::VarHandle * GLSLProgram::getTexHandle()
{
	return getHandle(m_tex);
}

gfx::engine::VarHandle * GLSLProgram::getTex1Handle()
{
	return getHandle(m_tex1);
}

//Loads shaders from their files into a shader program (from opengl-tutorials.org)
GLSLProgram::GLSLProgram() {}

GLSLProgram::GLSLProgram(
	const char * vertex_file_path,
	const char * fragment_file_path
) {

	CINFO("Creating new program...");

	this->m_vertexFilePath = vertex_file_path;
	this->m_fragmentFilePath = fragment_file_path;

	// Create the shaders
	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		CERROR(alib::StringFormat("failed to open %0")
			.arg(vertex_file_path).str(), __FILE__, __LINE__, CLASSNAME, "GLSLProgram");
		getchar();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		CERROR(alib::StringFormat("failed to open %0")
			.arg(fragment_file_path).str(), __FILE__, __LINE__, CLASSNAME, "GLSLProgram");
		getchar();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(m_vertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(m_vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(m_vertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(m_vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(m_vertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		CERROR(alib::StringFormat("compilation failed:\n%0")
			.arg(&VertexShaderErrorMessage[0]).str(), __FILE__, __LINE__, CLASSNAME, "GLSLProgram");
	}
	else
		CINFO(alib::StringFormat("    %0 compiled").arg(vertex_file_path).str());

	// Compile Fragment Shader
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(m_fragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(m_fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(m_fragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(m_fragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(m_fragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		CERROR(alib::StringFormat("compilation failed:\n%0")
			.arg(&FragmentShaderErrorMessage[0]).str(), __FILE__, __LINE__, CLASSNAME, "GLSLProgram");
	}
	else
		CINFO(alib::StringFormat("    %0 compiled").arg(fragment_file_path).str());

	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, m_vertexShaderID);
	glAttachShader(ProgramID, m_fragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		CERROR(alib::StringFormat("linking failed:\n%0")
			.arg(&ProgramErrorMessage[0]).str(), __FILE__, __LINE__, CLASSNAME, "GLSLProgram");
	}
	else
		CINFO("    program linked");


	glDetachShader(ProgramID, m_vertexShaderID);
	glDetachShader(ProgramID, m_fragmentShaderID);

	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);

	this->m_Id = ProgramID;

	CINFO(alib::StringFormat("    Loaded GLSLProgram -> Program ID %0").arg(ProgramID).str());
}
