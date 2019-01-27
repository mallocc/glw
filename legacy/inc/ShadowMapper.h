#pragma once

#include "FBO.h"
#include "Types.h"
namespace gfx
{
	namespace engine
	{
		class ShadowMapper
		{
		public:
			FBO m_fbo;
			type::Light_T m_light_source;
			glm::mat4 m_v, m_p;

			ShadowMapper() {}
			ShadowMapper(FBO fbo, type::Light_T  light_source, glm::vec3 dir_pos, glm::vec3 up)
			{
				this->m_fbo = fbo;
				this->m_light_source = light_source;
				m_v = glm::lookAt(light_source.pos, dir_pos, up);
				m_p = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 1000.0f);
			}

			// Add an mesh to render on this FBO
			FBO * add_mesh(Mesh * m)
			{
				return m_fbo.add_mesh(m);
			}

			void render_shadowmap(VarHandle * model_handle, VarHandle * texture_handle)
			{
				m_fbo.binding_draw_meshes(model_handle, texture_handle);
			}

			void render_shadowmap(VarHandle * model_handle, VarHandle * texture_handle, VarHandle * view_handle, VarHandle *proj_handle)
			{
				view_handle->load(m_v);
				proj_handle->load(m_p);
				m_fbo.binding_draw_meshes(model_handle, texture_handle);
			}

			void load_mats(VarHandle * view_handle, VarHandle *proj_handle)
			{
				view_handle->load(m_v);
				proj_handle->load(m_p);
			}

			ShadowMapper * set_fbo(FBO fbo)
			{
				this->m_fbo = fbo;
				return this;
			}

			ShadowMapper * set_light_source(type::Light_T  light_source)
			{
				this->m_light_source = light_source;
				return this;
			}

			ShadowMapper * set_mvp(glm::vec3 pos, glm::vec3 dir_pos, glm::vec3 up)
			{
				m_v = glm::lookAt(pos, dir_pos, up);
				m_p = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 1000.0f);
				return this;
			}

			ShadowMapper * set_mvp(glm::vec3 dir_pos, glm::vec3 up)
			{
				m_v = glm::lookAt(m_light_source.pos, dir_pos, up);
				m_p = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 1000.0f);
				return this;
			}

			glm::mat4 * get_view_mat4()
			{
				return &m_v;
			}

			glm::mat4 * get_proj_mat4()
			{
				return &m_p;
			}

			GLuint * get_depth()
			{
				return m_fbo.get_depth();
			}
			GLuint * get_color()
			{
				return m_fbo.get_tex();
			}
		};
	}
}
