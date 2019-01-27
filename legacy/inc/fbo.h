#pragma once

#include "glm.h"
#include "opengl.h"
#include "mesh.h"

namespace gfx
{
	namespace engine
	{
		// An FBO ID
		typedef GLuint FBOID;

		// This is an FBO struct.
		// Holds the properties: FBO, texture, dimensions, meshes to render, render mesh for texture
		class FBO
		{
		public:
			// Set the render mesh for the texture
			void set_render_mesh(Mesh * mesh);

			// Binds FBO for render
			void bind();

			// Unbinds all FBOs
			static void unbind();

			// Gets texture for this FBO
			GLuint * get_tex();

			// Gets depth buffer for this FBO
			GLuint * get_depth();

			// Gets FBO ID
			FBOID get_fboid();

			// Add an mesh to render on this FBO
			FBO * add_mesh(Mesh * m);

			// Just draws the meshes for this FBO
			void draw_meshes(MeshHandle_T handles);

			// Renders meshes to this FBO
			void binding_draw_meshes(MeshHandle_T handles);

			// Load this FBOs texture to the shader
			void activate_texture(VarHandle * handle);

			// Unload this FBOs texture from the shader
			void deactivate_texture();

			// Draw the texture on the render mesh (make sure ortho is used)
			void draw_render_mesh(MeshHandle_T handles);

			// Creates a FBO and its render texture and depth buffer
			void get_frame_buffer(GLuint * FramebufferName, GLuint * colorTexture, GLuint *depthTexture);

			// constructors

			FBO();

			// Frame size
			FBO(int w, int h);

			// Frame size
			FBO(glm::vec2 window_size);

		protected:

			// member variables

			FBOID m_id;

			GLuint m_tex, m_depth;

			int m_width = 0, m_height = 0;

			std::vector<Mesh*> m_meshes;

			Mesh * m_render_mesh;
		};
	}
}



//void get3DTexture(GLuint * tex, int w, int h, int d, GLuint * data)
//{
//	//create and bind texture
//	glGenTextures(1, tex);
//	glBindTexture(GL_TEXTURE_2D_ARRAY, *tex);
//
//	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, w, h, d, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//
//	// set texture wrapping to GL_REPEAT (default wrapping method)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	for (int i = 0; i < d; ++i)
//		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, w, h, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
//
//}