#include "FBO.h"
#include "mesh.h"
#include "VarHandle.h"
#include "CLog.h"
#include "StringFormat.h"

using gfx::engine::FBO;

namespace
{
	const char * CLASSNAME = "FBO";
}

void FBO::get_frame_buffer(GLuint * FramebufferName, GLuint * colorTexture, GLuint *depthTexture)
{
	CINFO("creating fbo...");

	glGenFramebuffersEXT(1, FramebufferName);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *FramebufferName);

	//RGBA8 2D texture, 24 bit depth texture
	glGenTextures(1, colorTexture);
	glBindTexture(GL_TEXTURE_2D, *colorTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//NULL means reserve texture memory, but texels are undefined
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//Attach 2D texture to this FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *colorTexture, 0);

	//RGBA8 2D texture, 24 bit depth texture
	glGenTextures(1, depthTexture);
	glBindTexture(GL_TEXTURE_2D, *depthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//NULL means reserve texture memory, but texels are undefined
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, *depthTexture, 0);

	GLuint depthBuffer;
	// The depth buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	//Attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	//Does the GPU support current FBO configuration?
	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		break;
	default:
		CERROR(alib::StringFormat("error creating FBO").str(), __FILE__, __LINE__, CLASSNAME, "get_frame_buffer");
	}

	glFlush();

	CINFO(alib::StringFormat("    FBO created: Tex %0 -> FBO %1").arg(m_tex).arg(m_id).str());
}

FBO::FBO() {};

// Frame size
FBO::FBO(int w, int h)
{
	this->m_width = w; this->m_height = h;
	get_frame_buffer(&m_id, &m_tex, &m_depth);
}
// Frame size
FBO::FBO(glm::vec2 window_size)
{
	this->m_width = window_size.x; this->m_height = window_size.y;
	get_frame_buffer(&m_id, &m_tex, &m_depth);
}

// Set the render mesh for the texture
void FBO::set_render_mesh(gfx::engine::Mesh * mesh)
{
	m_render_mesh = mesh;
}

// Binds FBO for render
void FBO::bind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_id);
	//glClearDepth(1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);
	glViewport(0, 0, m_width, m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, m_width, 0.0, m_height, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
}

// Unbinds all FBOs
void FBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Gets texture for this FBO
GLuint * FBO::get_tex()
{
	return &m_tex;
}

// Gets depth buffer for this FBO
GLuint * FBO::get_depth()
{
	return &m_depth;
}

// Gets FBO ID
gfx::engine::FBOID FBO::get_fboid()
{
	return m_id;
}

// Add an mesh to render on this FBO
FBO * FBO::add_mesh(gfx::engine::Mesh * m)
{
	m_meshes.push_back(m);
	return this;
}

// Just draws the meshes for this FBO
void FBO::draw_meshes(gfx::engine::MeshHandle_T handles)
{
	for (Mesh * m : m_meshes)
		m->draw(0, handles);
}

// Renders meshes to this FBO
void FBO::binding_draw_meshes(gfx::engine::MeshHandle_T handles)
{
	bind();
	for (Mesh * m : m_meshes)
		m->draw(0, handles);
	unbind();
}

// Load this FBOs texture to the shader
void FBO::activate_texture(gfx::engine::VarHandle * handle)
{
	handle->load(m_tex);
	glActiveTexture(GL_TEXTURE0 + m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);
}

// Unload this FBOs texture from the shader
void FBO::deactivate_texture()
{
	glActiveTexture(GL_TEXTURE0 + m_tex);
	glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);
}

// Draw the texture on the render mesh (make sure ortho is used)
void FBO::draw_render_mesh(gfx::engine::MeshHandle_T handles)
{
	activate_texture(handles.textureHandle);
	m_render_mesh->draw(0, handles);
	deactivate_texture();
}