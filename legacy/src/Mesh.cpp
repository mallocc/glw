#include "Mesh.h"
#include "CLog.h"
#include "StringFormat.h"

using gfx::engine::Mesh;

namespace
{
	const char * CLASSNAME = "Mesh";
}

// Buffers Vertex data into the VBO
void Mesh::init(std::vector<gfx::Vertex_T> * d)
{
	m_data_size = d->size();
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, m_data_size * sizeof(struct gfx::Vertex_T), d->data(), GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(struct gfx::Vertex_T),
		(const GLvoid*)offsetof(struct gfx::Vertex_T, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(struct gfx::Vertex_T),
		(const GLvoid*)offsetof(struct gfx::Vertex_T, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(struct gfx::Vertex_T),
		(const GLvoid*)offsetof(struct gfx::Vertex_T, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer((GLuint)3, 2, GL_FLOAT, GL_FALSE, sizeof(struct gfx::Vertex_T),
		(const GLvoid*)offsetof(struct gfx::Vertex_T, uv));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer((GLuint)4, 3, GL_FLOAT, GL_FALSE, sizeof(struct gfx::Vertex_T),
		(const GLvoid*)offsetof(struct gfx::Vertex_T, tangent));
	glEnableVertexAttribArray(4);
	glBindVertexArray(0);
	glFlush();

	CINFO(alib::StringFormat("    buffered into VAO %0").arg(m_vao).str());
}

// Loads image file into a texture
void Mesh::load_textures(const char *texfilename)
{
	if (texfilename != "")
	{
		m_tex = alib::ImageLoader::loadTextureFromImage(texfilename);
		CINFO(alib::StringFormat("    %0 -> Texture ID %1").arg(texfilename).arg(m_tex).str());
	}
	else
	{
		CINFO("    no texture file loaded");
	}
}

// Draws the mesh including linking the model matrix
void Mesh::draw(int wire_frame, gfx::engine::MeshHandle_T handles)
{
	handles.modelMatHandle->load(get_model_mat());
	draw_array(wire_frame, handles.textureHandle);
}

// Draws just the VBO and activating the texture
void Mesh::draw_array(int wire_frame, gfx::engine::VarHandle *texture_handle)
{
	// load the textures
	if (m_tex != GL_TEXTURE0)
	{
		load_texture_handle(texture_handle);
		glActiveTexture(GL_TEXTURE0 + m_tex);
		glBindTexture(GL_TEXTURE_2D, m_tex);
	}

	// draw the data
	glBindVertexArray(m_vao);
	glDrawArrays(wire_frame ? GL_LINE_LOOP : GL_TRIANGLES, 0, m_data_size);
	glBindVertexArray(0);

	// unload the texture
	if (m_tex != GL_TEXTURE0)
	{
		glActiveTexture(GL_TEXTURE0 + m_tex);
		glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);
	}
	glActiveTexture(GL_TEXTURE0);
	glFinish();
}

// Override the texture handle seperately
void Mesh::load_texture_handle(gfx::engine::VarHandle * handle)
{
	handle->load(m_tex);
}

// Sets the texture
void Mesh::set_tex(GLuint tex)
{
	this->m_tex = tex;
}

// Get the model matrix
glm::mat4 Mesh::get_model_mat()
{
	return glm::translate(glm::mat4(1.), m_pos) *
		glm::rotate(glm::mat4(1.), m_theta, m_rotation) *
		glm::rotate(glm::mat4(1.), m_pre_theta, m_pre_rotation) *
		glm::scale(glm::mat4(1.), m_scale);
}


Mesh::Mesh() {}

// Texture filename, Vertex data pack, world position, dynamic axis of rotation, and amount, static axis of rotation, and amount, scale vector. 
Mesh::Mesh(
	const char *texfilename,
	std::vector<gfx::Vertex_T>	data,
	glm::vec3 _pos,
	glm::vec3 _rotation,
	GLfloat _theta,
	glm::vec3 _pre_rotation,
	GLfloat _pre_theta,
	glm::vec3 _scale
)
{
	CINFO("Loading new Mesh...");
	CINFO(alib::StringFormat("    Vertex count = %0").arg(data.size()).str());

	m_pos = _pos;
	m_rotation = _rotation;
	m_theta = _theta;
	m_scale = _scale;
	m_pre_rotation = _pre_rotation;
	m_pre_theta = _pre_theta;

	load_textures(texfilename);
	init(&data);
}