#pragma once

#include "opengl.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <map>

#include "varhandle.h"
#include "GLSLProgram.h"

using gfx::engine::GLSLProgram;

namespace gfx
{
	namespace engine
	{
		struct GLSLProgramManager
		{
		public:
			GLSLProgram * loadProgram(gfx::engine::GLSLProgramID id);

			GLSLProgram * addProgram(
				const char * vertex_file_path,
				const char * fragment_file_path
			);

			GLSLProgram * addProgram(GLSLProgram program);

			GLSLProgram * getProgram(gfx::engine::GLSLProgramID id);

			GLSLProgram * getCurrentProgram();

			gfx::engine::GLSLProgramID addProgram(
				const char * vertex_file_path,
				const char * fragment_file_path,
				glm::mat4 * model_data,
				glm::mat4 * view_data,
				glm::mat4 * proj_data
			);

			GLSLProgramManager();

		private:
			std::map<gfx::engine::GLSLProgramID, GLSLProgram> m_shaderPrograms;
			gfx::engine::GLSLProgramID m_currentProgram;
		};
	}
}