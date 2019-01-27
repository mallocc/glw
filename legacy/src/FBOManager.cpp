#include "FBOManager.h"
#include "Mesh.h"

using gfx::engine::FBOManager;

// constructor
FBOManager::FBOManager() {}

// Add an FBO to the list and returns its FBOID
gfx::engine::FBOID FBOManager::addFBO(glm::vec2 size, gfx::engine::Mesh * render_mesh)
{
	gfx::engine::FBO fbo = gfx::engine::FBO(size);
	fbo.set_render_mesh(render_mesh);
	m_fbos.insert({ fbo.get_fboid(), fbo });
	return fbo.get_fboid();
}

// Get the pointer to an FBO in the list
gfx::engine::FBO * FBOManager::get_fbo(gfx::engine::FBOID id)
{
	return &m_fbos[id];
}