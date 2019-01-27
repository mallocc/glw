#pragma once

#include "glm.h"
#include "opengl.h"
#include <map>
#include "mesh.h"
#include "FBO.h"

namespace gfx
{
	namespace engine
	{
		// Manages a list of FBOs
		class FBOManager
		{
		public:
			// Add an FBO to the list and returns its FBOID
			gfx::engine::FBOID addFBO(glm::vec2 size, Mesh * render_mesh);

			// Get the pointer to an FBO in the list
			gfx::engine::FBO * get_fbo(gfx::engine::FBOID id);


			// constructor

			FBOManager();

		private:
			// member variables 

			std::map<gfx::engine::FBOID, gfx::engine::FBO> m_fbos;

		};
	}
}