#include "GLSLProgramManager.h"

using gfx::engine::GLSLProgramManager;

GLSLProgram * GLSLProgramManager::loadProgram(gfx::engine::GLSLProgramID id)
{
	m_shaderPrograms[id].load();
	m_currentProgram = id;
	return getProgram(id);
}

GLSLProgram * GLSLProgramManager::addProgram(
	const char * vertex_file_path,
	const char * fragment_file_path
)
{
	GLSLProgram program = GLSLProgram(vertex_file_path, fragment_file_path);
	m_shaderPrograms.insert({ program.getId(), program });
	return getProgram(program.getId());
}

GLSLProgram * GLSLProgramManager::addProgram(GLSLProgram program)
{

	m_shaderPrograms.insert({ program.getId(), program });
	return getProgram(program.getId());
}

GLSLProgram * GLSLProgramManager::getProgram(gfx::engine::GLSLProgramID id)
{
	return &m_shaderPrograms[id];
}

GLSLProgram * GLSLProgramManager::getCurrentProgram()
{
	return &m_shaderPrograms[m_currentProgram];
}

gfx::engine::GLSLProgramID GLSLProgramManager::addProgram(
	const char * vertex_file_path,
	const char * fragment_file_path,
	glm::mat4 * model_data,
	glm::mat4 * view_data,
	glm::mat4 * proj_data
)
{
	GLSLProgram program = GLSLProgram(vertex_file_path, fragment_file_path);
	program.setModelMat4Handle(model_data);
	program.setViewMat4Handle(view_data);
	program.setProjMat4Handle(proj_data);
	m_shaderPrograms.insert({ program.getId(), program });
	return program.getId();
}

GLSLProgramManager::GLSLProgramManager() {}