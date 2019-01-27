#pragma once

#include "opengl.h"
#include "glm.h"

#include <stdio.h>

#include <vector>

namespace gfx
{
	namespace engine
	{
		typedef GLuint VarHandleID;

		class VarHandle
		{
		public:
			void init(GLuint program);

			void load();
			void load(glm::mat4 data);
			void load(glm::vec3 data);
			void load(glm::vec4 data);
			void load(GLfloat data);
			void load(GLuint data);
			void load(int data);

			glm::mat4 * get_handle_data_mat4();

			glm::vec3 * get_handle_data_vec3();

			VarHandleID get_handle_id();

			const char * get_handle_name();

			VarHandle();
			VarHandle(const char * var_name_);
			VarHandle(const char * var_name_, glm::mat4 * data);
			VarHandle(const char * var_name_, glm::vec3 * data);
			VarHandle(const char * var_name_, glm::vec4 * data);
			VarHandle(const char * var_name_, GLfloat * data);
			VarHandle(const char * var_name_, GLuint * data);
			VarHandle(const char * var_name_, int * data);

		private:
			const char * m_var_name;
			VarHandleID m_handle;
			glm::mat4 * m_data_m;
			glm::vec3 * m_data_v3;
			glm::vec4 * m_data_v4;
			GLfloat * m_data_f;
			GLuint * m_data_i;
			int * m_data_ii;
			int m_handle_type;
		};

		struct MeshHandle_T
		{
			VarHandle * textureHandle;
			VarHandle * colorHandle;
			VarHandle * flagHandle;
			VarHandle * modelMatHandle;
			VarHandle * viewMatHandle;
			VarHandle * projMatHandle;
		};
	}
}





