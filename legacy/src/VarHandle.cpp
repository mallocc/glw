#include "VarHandle.h"
#include "CLog.h"
#include "StringFormat.h"

using gfx::engine::VarHandle;

namespace
{
	const char * CLASSNAME = "VarHandle";

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
}

void VarHandle::init(GLuint program)
{
	m_handle = glGetUniformLocation(program, m_var_name);
	CINFO(alib::StringFormat("Program ID %0: linking %1 -> VarHandle ID %2").arg(program).arg(m_var_name).arg(m_handle).str());
}

void VarHandle::load()
{
	if (m_handle_type != NO_HANDLE)
	{
		if (m_handle_type == MAT4_HANDLE)
		{
			glm::mat4 mat = *m_data_m;
			glUniformMatrix4fv(m_handle, 1, GL_FALSE, &mat[0][0]);
		}
		else if (m_handle_type == VEC3_HANDLE)
			glUniform3f(m_handle, m_data_v3->x, m_data_v3->y, m_data_v3->z);
		else if (m_handle_type == VEC4_HANDLE)
			glUniform4f(m_handle, m_data_v4->x, m_data_v4->y, m_data_v4->z, m_data_v4->w);
		else if (m_handle_type == FLOAT_HANDLE)
			glUniform1f(m_handle, *m_data_f);
		else if (m_handle_type == GLUINT_HANDLE)
			glUniform1i(m_handle, *m_data_i);
		else if (m_handle_type == INT_HANDLE)
			glUniform1i(m_handle, *m_data_ii);
	}
}
void VarHandle::load(glm::mat4 data)
{
	glUniformMatrix4fv(m_handle, 1, GL_FALSE, &data[0][0]);
}
void VarHandle::load(glm::vec3 data)
{
	glUniform3f(m_handle, data.x, data.y, data.z);
}
void VarHandle::load(glm::vec4 data)
{
	glUniform4f(m_handle, data.x, data.y, data.z, data.w);
}
void VarHandle::load(GLfloat data)
{
	glUniform1f(m_handle, data);
}
void VarHandle::load(GLuint data)
{
	glUniform1i(m_handle, data);
}
void VarHandle::load(int data)
{
	glUniform1i(m_handle, data);
}

glm::mat4 * VarHandle::get_handle_data_mat4()
{
	return m_data_m;
}

glm::vec3 * VarHandle::get_handle_data_vec3()
{
	return m_data_v3;
}

gfx::engine::VarHandleID VarHandle::get_handle_id()
{
	return m_handle;
}

const char * VarHandle::get_handle_name()
{
	return m_var_name;
}

VarHandle::VarHandle()
{
	m_handle_type = NO_HANDLE;
}
VarHandle::VarHandle(const char * var_name_)
{
	m_var_name = var_name_;
	m_handle_type = NO_HANDLE;
}
VarHandle::VarHandle(const char * var_name_, glm::mat4 * data)
{
	m_var_name = var_name_;
	m_data_m = data;
	m_handle_type = MAT4_HANDLE;
}
VarHandle::VarHandle(const char * var_name_, glm::vec3 * data)
{
	m_var_name = var_name_;
	m_data_v3 = data;
	m_handle_type = VEC3_HANDLE;
}
VarHandle::VarHandle(const char * var_name_, glm::vec4 * data)
{
	m_var_name = var_name_;
	m_data_v4 = data;
	m_handle_type = VEC4_HANDLE;
}
VarHandle::VarHandle(const char * var_name_, GLfloat * data)
{
	m_var_name = var_name_;
	m_data_f = data;
	m_handle_type = FLOAT_HANDLE;
}
VarHandle::VarHandle(const char * var_name_, GLuint * data)
{
	m_var_name = var_name_;
	m_data_i = data;
	m_handle_type = GLUINT_HANDLE;
}
VarHandle::VarHandle(const char * var_name_, int * data)
{
	m_var_name = var_name_;
	m_data_ii = data;
	m_handle_type = INT_HANDLE;
}