#pragma once

#include "glm.h"
#include "vector"
#include "mesh.h"
#include "CLog.h"
#include "StringFormat.h"
#include "FBO.h"
#include "PrimativeGenerator.h"
#include <map>

#define GFX_NULLPTR nullptr
#define GFX_NULL_ID -1

namespace gfx
{
	namespace gui
	{
		class GFXManager;

		class GFXMesh
		{
		public:

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
				m_color = glm::vec4(color,1.0f);
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
					glm::scale(glm::mat4(1.), glm::vec3(m_size,0.0f));
			}

			// Draws the mesh including linking the model matrix
			void drawMesh(gfx::engine::VarHandle *model, gfx::engine::VarHandle *color)
			{
				model->load(getModelMat());
				color->load(m_color);
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
				m_color = glm::vec4(gfx::GREEN,1.0f);
			}
			GFXMesh(gfx::VertexData d) : GFXMesh()
			{
				CINFO("Loading new MiniMesh...");
				init(d);
			}
			GFXMesh(gfx::VertexData d, glm::vec4 color) : GFXMesh()
			{
				CINFO("Loading new MiniMesh...");
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

		typedef int GFXID;
		class GFXUnit
		{
		public:
			GFXUnit * getParent()
			{
				return m_parent;
			}
			void setParent(GFXUnit * parent)
			{
				m_parent = parent;
			}
			
			GFXManager * getManager()
			{
				return m_manager;
			}
			void setManager(GFXManager * manager)
			{
				m_manager = manager;
			}

			GFXID getId()
			{
				return m_id;
			}
			void setId(int id)
			{
				m_id = id;
			}

			GFXUnit()
			{
				m_id = GFX_NULL_ID;
				m_parent = GFX_NULLPTR;				
				m_manager = GFX_NULLPTR;
			}
			GFXUnit(GFXID id, GFXUnit * parent, GFXManager * manager)
			{
				m_id = id;
				m_parent = parent;
				m_manager = manager;
			}
			GFXUnit(GFXUnit * component)
			{
				m_id = component->m_id;
				m_parent = component->m_parent;
			}
			
		protected:
			GFXManager * m_manager;
			GFXUnit * m_parent;
			GFXID m_id;		
		};

		typedef int GFXTrigger;
		typedef void(*GFXAction)();
		typedef std::pair<GFXTrigger, GFXAction> GFXLink;
		class GFXLinker
		{
		public:
			void link(GFXTrigger trigger, GFXAction action)
			{
				m_links.push_back(std::make_pair(trigger, action));
			}
		protected:
			void checkLinks(GFXTrigger trigger)
			{
				for (GFXLink& link : m_links)
					if (trigger == link.first)
						link.second();
			}

			std::vector<GFXLink> m_links;
		};

		struct GFXColorStyle_T
		{
			glm::vec4 color1;
			glm::vec4 color2;
			glm::vec4 color3;
		};

		class GFXComponent : public GFXUnit
		{
			virtual void draw(gfx::engine::VarHandle *model, gfx::engine::VarHandle *color) = 0;
		};

		class GFXGroup
		{
		public:
			GFXGroup * add(GFXUnit * component)
			{
				m_group.push_back(component);
				return this;
			}
			GFXGroup * add(GFXUnit * component, int id, GFXUnit * parent, GFXManager * manager)
			{
				component->setId(id);
				component->setParent(parent);
				component->setManager(manager);
				m_group.push_back(component);
				return this;
			}

			void drawGroup(gfx::engine::VarHandle *model, gfx::engine::VarHandle *color)
			{
				for (GFXUnit unit : m_group)
					if (typeid(unit) == typeid(GFXComponent))
						((GFXComponent*) &unit)->drawMesh(model, color);
			}

			void bringForward(GFXUnit * component)
			{
				for (int i = 0; i < m_group.size(); ++i)
					if (m_group[i] == component)
					{
						m_group.erase(m_group.begin() + i);
						m_group.insert(m_group.begin() + i + 1, component);
						break;
					}
			}
			void bringBackward(GFXUnit * component)
			{
				for (int i = 0; i < m_group.size(); ++i)
					if (m_group[i] == component)
					{
						m_group.erase(m_group.begin() + i);
						m_group.insert(m_group.begin() + i - 1, component);
						break;
					}
			}
		private:
			std::vector<GFXUnit*> m_group;
		};

		class GFXManager : public GFXGroup
		{
		public:
			gfx::engine::FBOID getFBOId()
			{
				return m_fboId;
			}
			void setFBOId(gfx::engine::FBOID id)
			{
				m_fboId = id;
			}

			gfx::engine::GLSLProgramID getProgramId()
			{
				return m_programId;
			}
			void setProgramId(gfx::engine::GLSLProgramID id)
			{
				m_programId = id;
			}

			gfx::engine::VarHandle getColorHandle()
			{
				return m_colorHandle;
			}
			void setColorHandle(gfx::engine::VarHandle colorHandle)
			{
				m_colorHandle = colorHandle;
			}

			int getNextId()
			{
				return currentId++;
			}

			GFXManager * addUnit(GFXUnit * unit)
			{
				add(unit, getNextId(), GFX_NULLPTR, this);
			}

			void drawAll(gfx::engine::VarHandle *model, gfx::engine::VarHandle *color)
			{
				drawGroup(model, color);
			}

			GFXManager() {};
			GFXManager(gfx::engine::GLSLProgramID programId, gfx::engine::FBOID fboId)
			{
				m_fboId = fboId;
				m_programId = programId;
			}
		protected:
			gfx::engine::FBOID m_fboId;
			gfx::engine::GLSLProgramID m_programId;
			gfx::engine::VarHandle m_colorHandle;
			int currentId = -1;
		};

		class GFXContainer : public GFXComponent, public GFXGroup
		{
		public:
			GFXContainer * addUnit(GFXUnit * unit)
			{
				add(unit, m_manager->getNextId(), this, m_manager);
			}

			void drawAll(gfx::engine::VarHandle *model, gfx::engine::VarHandle *color)
			{
				draw(model, color);
				drawGroup(model, color);
			}
		private:
		};

		class GFXColorStyle
		{
		public:
			GFXColorStyle_T getColorStyle()
			{
				return m_colorStyle;
			}
			void setColorStyle(GFXColorStyle_T colorStyle)
			{
				m_colorStyle = colorStyle;
			}

			GFXColorStyle() {}
			GFXColorStyle(GFXColorStyle_T colorStyle)
			{
				m_colorStyle = colorStyle;
			}

		private:
			GFXColorStyle_T m_colorStyle;
		};

		class GFXBox : public GFXMesh
		{
		public:
			GFXBox() : GFXMesh() {}
			GFXBox(glm::vec2 pos, glm::vec2 size) : GFXMesh()
			{
				CINFO("Loading new GFXBox...");
				m_pos = pos;
				m_size = size;
				std::vector<glm::vec3> v = gfx::PrimativeGenerator::generate_centered_square_mesh(1,1);
				VertexData data = gfx::PrimativeGenerator::pack_object(&v, GEN_COLOR, gfx::BLUE);
				init(data);
			}

		};

		class GFXText
		{
		public:
			std::string getText()
			{
				return m_text;
			}
			void setText(std::string text)
			{
				m_text = text;
			}

			GFXText() {}
			GFXText(std::string text)
			{
				m_text = text; 
			}

		protected:
			std::string m_text;
		};

		class GFXButton : public GFXColorStyle, public GFXComponent
		{

		};



		class GFXTest : public GFXUnit, public GFXLinker
		{
		public:
			void trigger()
			{
				CINFO("trigger executed");
				checkLinks(t_trigger);
			}

			GFXTest(int id)
			{
				m_id = id;
			}

			enum
			{
				t_trigger,
			};
		};



	}
}