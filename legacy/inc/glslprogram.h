#pragma once

#include "opengl.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <map>

#include "VarHandle.h"

namespace gfx
{
	namespace engine
	{
		typedef GLuint GLSLProgramID;

		struct GLSLProgram
		{
		public:
			GLSLProgram * addHandle(VarHandle handle);

			void load();

			VarHandle * getHandle(VarHandleID id);

			GLSLProgramID getId();

			GLSLProgram * setModelMat4Handle(glm::mat4 * mat);

			GLSLProgram * setViewMat4Handle(glm::mat4 * mat);

			GLSLProgram * setProjMat4Handle(glm::mat4 * mat);

			GLSLProgram * setColorHandle();

			GLSLProgram * setFlagHandle();

			GLSLProgram * setTexHandle();
			GLSLProgram * setTex1Handle();

			VarHandle * getModelMat4Handle();

			VarHandle * getViewMat4Handle();

			VarHandle * getProjMat4Handle();

			VarHandle * getColorHandle();

			VarHandle * getFlagHandle();

			VarHandle * getTexHandle();

			VarHandle * getTex1Handle();

			MeshHandle_T getMeshHandle()
			{
				return {getTexHandle(), getColorHandle(), getFlagHandle(), getModelMat4Handle(), getViewMat4Handle(), getProjMat4Handle()};
			}

			//Loads shaders from their files into a shader program (from opengl-tutorials.org)
			GLSLProgram();

			GLSLProgram(
				const char * vertexFilePath,
				const char * fragmentFilePath
			);

		private:
				GLSLProgramID m_Id;

				GLuint m_vertexShaderID;
				GLuint m_fragmentShaderID;

				const char * m_vertexFilePath;
				const char * m_fragmentFilePath;

				std::map<VarHandleID, VarHandle> m_handles;

				VarHandleID
					m_modelMat, m_viewMat, m_projMat,
					m_color,
					m_flag,
					m_tex, m_tex1;
		};
	}
}


