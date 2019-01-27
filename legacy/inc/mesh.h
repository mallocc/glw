#pragma once

#include "opengl.h"
#include "glm.h"
#include "ImageLoader.h"
#include "VarHandle.h"
#include "Types.h"
#include <vector>

namespace gfx
{
	namespace engine
	{
		// This is a struct for a Mesh.
		// Holds VBO, texture and entity properties.
		class Mesh
		{
		public:
			// Buffers Vertex data into the VBO
			void init(std::vector<gfx::Vertex_T> * d);

			// Loads image file into a texture
			void load_textures(const char *texfilename);

			// Draws the mesh including linking the model matrix
			void draw(int wire_frame, gfx::engine::MeshHandle_T handles);

			// Draws just the VBO and activating the texture
			void draw_array(int wire_frame, VarHandle *texture_handle);

			// Override the texture handle seperately
			void load_texture_handle(VarHandle * handle);

			// Sets the texture
			void set_tex(GLuint tex);

			// Get the model matrix
			glm::mat4 get_model_mat();
			

			Mesh();

			// Texture filename, Vertex data pack, world position, dynamic axis of rotation, and amount, static axis of rotation, and amount, scale vector. 
			Mesh(
				const char *texfilename,
				std::vector<gfx::Vertex_T>	data,
				glm::vec3 _pos,
				glm::vec3 _rotation,
				GLfloat _theta,
				glm::vec3 _pre_rotation,
				GLfloat _pre_theta,
				glm::vec3 _scale
			);

			GLuint
				m_vao,
				m_buffer,
				m_tex = GL_TEXTURE0;
			int
				m_data_size = 0;

			glm::vec3
				m_rotation = glm::vec3(0, 1, 0),
				m_pre_rotation = glm::vec3(0, 1, 0),
				m_scale = glm::vec3(1, 1, 1),
				m_pos, m_vel;
			GLfloat
				m_theta,
				m_pre_theta;
		};
	}
}

