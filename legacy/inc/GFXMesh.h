#pragma once

#include "glm.h"
#include "opengl.h"
#include "PrimativeGenerator.h"
#include "CLog.h"
#include "StringFormat.h"
#include "colors.h"

using namespace alib::CLog;

namespace gfx
{
	namespace gui
	{
		class GFXMesh
		{
		public:

			bool isInside(glm::vec2 point)
			{
				return point.x >= 0 && point.y >= 0 && point.x <= m_size.x && point.y <= m_size.y;
			}

			glm::vec2 getPos()
			{
				return m_pos;
			}
			void setPos(glm::vec2 pos)
			{
				m_pos = pos;
			}

			glm::vec2 getSize()
			{
				return m_size;
			}
			void setSize(glm::vec2 size)
			{
				m_size = size;
			}

			glm::vec4 getColor()
			{
				return m_color;
			}
			void setColor(glm::vec4 color)
			{
				m_color = color;
			}
			void setColor(glm::vec3 color)
			{
				m_color = glm::vec4(color, 1.0f);
			}

			void setAlpha(float alpha)
			{
				m_color.w = alpha;
			}

			GLfloat getRotationAngle()
			{
				return m_theta;
			}
			void setRotationAngle(GLfloat rotationAngle)
			{
				m_theta = rotationAngle;
			}

			// Buffers Vertex data into the VBO
			void init(gfx::VertexData d)
			{
				m_dataSize = d.size();
				glGenVertexArrays(1, &m_vao);
				glBindVertexArray(m_vao);
				glGenBuffers(1, &m_buffer);
				glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
				glBufferData(GL_ARRAY_BUFFER, m_dataSize * sizeof(struct gfx::Vertex_T), d.data(), GL_STATIC_DRAW);
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

			// Get the model matrix
			glm::mat4 getModelMat()
			{
				return glm::translate(glm::mat4(1.), glm::vec3(m_pos, 0.0f)) *
					glm::rotate(glm::mat4(1.), m_theta, m_rotation) *
					glm::scale(glm::mat4(1.), glm::vec3(m_size, 0.0f));
			}
			glm::mat4 getRelativeModelMat()
			{
				return glm::translate(glm::mat4(1.), glm::vec3(m_pos, 0.0f)) *
					glm::rotate(glm::mat4(1.), m_theta, m_rotation);
			}

			// Draws the mesh including linking the model matrix
			void drawMesh(gfx::engine::MeshHandle_T handles)
			{
				handles.modelMatHandle->load(getModelMat());
				handles.colorHandle->load(m_color);
				handles.flagHandle->load(0);
				drawArray();
			}

			void drawMesh(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles)
			{
				handles.modelMatHandle->load(modelMat * getModelMat());
				handles.colorHandle->load(m_color);
				handles.flagHandle->load(0);
				drawArray();
			}

			// Draws just the VBO and activating the texture
			void drawArray()
			{
				// draw the data
				glBindVertexArray(m_vao);
				glDrawArrays(GL_TRIANGLES, 0, m_dataSize);
				glBindVertexArray(0);
				glFinish();
			}

			GFXMesh()
			{
				m_pos = glm::vec2();
				m_size = glm::vec3();
				m_dataSize = 0;
				m_rotation = glm::vec3(0, 0, 1);
				m_theta = 0.0f;
				m_color = glm::vec4(gfx::BLACK, 1.0f);
			}
			GFXMesh(gfx::VertexData d) : GFXMesh()
			{
				CINFO("Loading new MiniMesh...");
				init(d);
			}
			GFXMesh(glm::vec2 pos, gfx::VertexData d) : GFXMesh()
			{
				CINFO("Loading new MiniMesh...");
				m_pos = pos;
				init(d);
			}
			GFXMesh(glm::vec2 pos, glm::vec2 size, gfx::VertexData d) : GFXMesh()
			{
				CINFO("Loading new MiniMesh...");
				m_pos = pos;
				m_size = size;
				init(d);
			}
			GFXMesh(gfx::VertexData d, glm::vec4 color) : GFXMesh()
			{
				CINFO("Loading new MiniMesh...");
				m_color = color;
				init(d);
			}
			GFXMesh(glm::vec2 pos, gfx::VertexData d, glm::vec4 color) : GFXMesh()
			{
				CINFO("Loading new MiniMesh...");
				m_pos = pos;
				m_color = color;
				init(d);
			}
			GFXMesh(glm::vec2 pos, glm::vec2 size, gfx::VertexData d, glm::vec4 color) : GFXMesh()
			{
				CINFO("Loading new MiniMesh...");
				m_pos = pos;
				m_size = size;
				m_color = color;
				init(d);
			}
			GFXMesh(GFXMesh * miniMesh)
			{
				m_pos = miniMesh->m_pos;
				m_size = miniMesh->m_size;
				m_vao = miniMesh->m_vao;
				m_buffer = miniMesh->m_buffer;
				m_dataSize = miniMesh->m_dataSize;
				m_rotation = miniMesh->m_rotation;
				m_theta = miniMesh->m_theta;
				m_color = miniMesh->m_color;
			}


			//		protected:
			glm::vec2 m_pos;
			glm::vec2 m_size;
			GLuint m_vao;
			GLuint m_buffer;
			int m_dataSize;
			glm::vec3 m_rotation;
			GLfloat m_theta;
			glm::vec4 m_color;
		};
	}
}