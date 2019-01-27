#pragma once

#include "glm.h"
#include "vector"
#include "GFXMesh.h"
#include "CLog.h"
#include "StringFormat.h"
#include "FBO.h"
#include "PrimativeGenerator.h"
#include "GLContent.h"
#include "GFXLinker.h"
#include "ImageLoader.h"
#include <map>

#define GFX_NULLPTR NULL
#define GFX_NULL_ID ""
#define GFX_DEFAULT_ID "component"
#define GFX_NULL_INDEX -1
#define GFX_NULL_COLORSTYLE NULL
#define GFX_RESIZE_NULL 0
#define GFX_RESIZE_LEFT 1
#define GFX_RESIZE_TOP 2
#define GFX_RESIZE_RIGHT 3
#define GFX_RESIZE_BOTTOM 4

#define GFX_GUI_SHADER_BLOCK 0
#define GFX_GUI_SHADER_FONT 1
#define GFX_GUI_SHADER_TEXTURE 2
#define GFX_GUI_DEFAULT_FONT_SIZE 20
#define GFX_GUI_DEFAULT_PADDING 50

namespace gfx
{
    namespace gui
	{

		const char * GFX_Courier_FONT = "textures/gfx_default.png";
		const char * GFX_Malgun_Gothic_FONT = "textures/Malgun_Gothic.png";

		// TYPEDEFS
		typedef std::string GFXID;
		struct GFXColorStyle_T
		{
			glm::vec4 colors[3];
		};
		class GFXManager;

		class GFXFont
		{
		public:
			// Draws the mesh including linking the model matrix
			void draw(unsigned char c, glm::mat4 modelMat, glm::vec2 pos, glm::vec2 scale, gfx::engine::MeshHandle_T handles)
			{
				handles.modelMatHandle->load(modelMat * glm::translate(glm::mat4(1.), glm::vec3(pos, 0)) *
					glm::scale(glm::mat4(1.), glm::vec3(scale, 0)));
				handles.colorHandle->load(m_color);
				handles.flagHandle->load(GFX_GUI_SHADER_FONT);
				drawArray(c, handles.textureHandle);
			}

			// Draws just the VBO and activating the texture
			void drawArray(unsigned char c, gfx::engine::VarHandle *textureHandle)
			{
				// load the textures
				if (m_tex != GL_TEXTURE0)
				{
					loadTextureHandle(textureHandle);
					glActiveTexture(GL_TEXTURE0 + m_tex);
					glBindTexture(GL_TEXTURE_2D, m_tex);
				}

				// draw the data
				glBindVertexArray(m_vao);
				glDrawArrays(GL_TRIANGLES, c * 6, 6);
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
			void loadTextureHandle(gfx::engine::VarHandle * handle)
			{
				handle->load(m_tex);
			}

			// Loads image file into a texture
			void loadTextures(const char *texfilename)
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

			void setColor(glm::vec4 color)
			{
				m_color = color;
			}
			void setColor(glm::vec3 color)
			{
				m_color = glm::vec4(color, 1);
			}
			
			GFXFont()
			{
			}
			GFXFont(const char *fontfile)
			{
				gfx::VertexData o = 
					gfx::PrimativeGenerator::pack_object(
						&gfx::PrimativeGenerator::generate_square_meshes(256),
						GEN_SQUAREUVS, gfx::WHITE);
				loadTextures(fontfile);
				init(o);
			}
		private:
			GLuint
				m_vao,
				m_buffer,
				m_tex = GL_TEXTURE0;
			int
				m_dataSize = 0;
			glm::vec4 m_color = gfx::WHITE_A;
		};

		// MESHES		
		class GFXBoxMesh : public GFXMesh
		{
		public:
			GFXBoxMesh() : GFXMesh() {}
			GFXBoxMesh(glm::vec2 pos, glm::vec2 size) : GFXMesh()
			{
				CINFO("Loading new GFXBox...");
				m_pos = pos;
				m_size = size;
				std::vector<glm::vec3> v = gfx::PrimativeGenerator::generate_square_border(0.02f, size.x / size.y);
				VertexData data = gfx::PrimativeGenerator::pack_object(&v, GEN_COLOR, gfx::BLUE);
				init(data);
			}
		};
		class GFXRectangleMesh : public GFXMesh
		{
		public:
			GFXRectangleMesh() : GFXMesh() {}
			GFXRectangleMesh(glm::vec2 pos, glm::vec2 size) : GFXMesh()
			{
				CINFO("Loading new GFXRectangleMesh...");
				m_pos = pos;
				m_size = size;
				std::vector<glm::vec3> v = gfx::PrimativeGenerator::generate_square_mesh(1, 1);
				VertexData data = gfx::PrimativeGenerator::pack_object(&v, GEN_COLOR, gfx::BLUE);
				init(data);
			}
		};
		class GFXCircleMesh : public GFXMesh
		{
		public:
			GFXCircleMesh() : GFXMesh() {}
			GFXCircleMesh(glm::vec2 pos, glm::vec2 size) : GFXMesh()
			{
				CINFO("Loading new GFXCircleMesh...");
				m_pos = pos;
				m_size = size;
				std::vector<glm::vec3> v = gfx::PrimativeGenerator::generate_centered_circle(20);
				VertexData data = gfx::PrimativeGenerator::pack_object(&v, GEN_COLOR, gfx::BLUE);
				init(data);
			}
		};

		// BASE CLASSES //
		class GFXUnit
		{
		public:
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
			void setId(GFXID id)
			{
				m_id = id;
			}

			GFXUnit()
			{
				m_id = GFX_DEFAULT_ID;
				m_manager = GFX_NULLPTR;
			}
			GFXUnit(GFXID id, GFXManager * manager)
			{
				m_id = id;
				m_manager = manager;
			}
			GFXUnit(GFXUnit * component)
			{
				m_id = component->m_id;
				m_manager = component->m_manager;
			}

		protected:
			GFXManager * m_manager;
			GFXID m_id;
		};
		
		class GFXComponent : public GFXUnit, public GFXMesh
		{
		public:
			bool isInside(gfx::engine::GLContent * content)
			{
				if (content != nullptr)
				{
					glm::vec2 mousePos = getRelativeMousePos(content);
					return mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x <= m_size.x && mousePos.y <= m_size.y;
				}
				return false;
			}

			glm::vec2 getRelativeMousePos(gfx::engine::GLContent * content)
			{
				glm::vec2 mousePos = content->getMousePos();
				GFXComponent * parent = m_parent;
				while (parent != GFX_NULLPTR)
				{
					mousePos -= parent->m_pos;
					parent = parent->getParent();
				}
				mousePos -= m_pos;
				return mousePos;
			}

			GFXComponent * getParent()
			{
				return m_parent;
			}
			void setParent(GFXComponent * parent)
			{
				m_parent = parent;
			}

			virtual GFXComponent * init(GFXManager * manager, GFXComponent * parent) = 0;
			virtual void validate() = 0;
			virtual void update(gfx::engine::GLContent * content) = 0;
			virtual bool checkEvents(gfx::engine::GLContent * content) = 0;
			virtual void draw(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles) = 0;

			GFXComponent * setColorStyle(GFXColorStyle_T * colorStyle)
			{
				if (m_colorStyle != GFX_NULL_COLORSTYLE)
					delete(m_colorStyle);
				m_colorStyle = new GFXColorStyle_T;
				m_colorStyle->colors[0] = colorStyle->colors[0];
				m_colorStyle->colors[1] = colorStyle->colors[1];
				m_colorStyle->colors[2] = colorStyle->colors[2];
				return this;
			}
			GFXColorStyle_T * getColorStyle()
			{
				return m_colorStyle;
			}

			void inheritColorStyle()
			{
				if (m_parent != GFX_NULLPTR)
				{
					m_colorStyle = m_parent->getColorStyle();
				}
			}

			void disable()
			{
				m_enabled = false;
			}
			void enable()
			{
				m_enabled = true;
			}
			bool isEnabled()
			{
				return m_enabled;
			}
			void visible()
			{
				m_visible = true;
			}
			void invisible()
			{
				m_visible = true;
			}
			bool isVisible()
			{
				return m_visible;
			}

			virtual std::string toString()
			{
				return alib::StringFormat("id = %0 pos = %1,%2 size = %3,%4").arg(getId()).arg(m_pos.x).arg(m_pos.y).arg(m_size.x).arg(m_size.y).str();
			}

			GFXComponent()
			{
			}
			GFXComponent(GFXMesh mesh) : GFXUnit(), GFXMesh(mesh)
			{
			}
		protected:
			GFXComponent * m_parent = GFX_NULLPTR;
			GFXColorStyle_T * m_colorStyle = GFX_NULL_COLORSTYLE;

			bool m_enabled = true;
			bool m_visible = true;
		};

		class GFXGroup
		{
		public:
			GFXGroup * add(GFXComponent * component)
			{
				m_group.push_back(component);
				return this;
			}
			
			int getComponent(GFXComponent * component)
			{
				for (int ix = 0; ix < m_group.size(); ++ix)
					if (m_group[ix] == component)
						return ix;
				return GFX_NULL_INDEX;
			}
			bool removeComponent(GFXComponent * component)
			{
				int id = getComponent(component);
				if (id != GFX_NULL_INDEX)
				{
					m_group.erase(m_group.begin() + id);
					return true;
				}

				bool success = false;
				for (GFXComponent * c : m_group)
						success |= dynamic_cast<GFXGroup*>(c)->removeComponent(component);
				return success;
			}
			
			int getComponent(GFXID id)
			{
				for (int ix = 0; ix < m_group.size(); ++ix)
					if (m_group[ix]->getId() == id)
						return ix;
				return GFX_NULL_INDEX;
			}
			bool componentExists(GFXID id)
			{
				int index = getComponent(id);
				if (index != GFX_NULL_INDEX)
					return true;

				bool success = false;
				for (GFXComponent * c : m_group)
						success |= dynamic_cast<GFXGroup*>(c)->componentExists(id);
				return success;
			}

			void drawGroup(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles)
			{
				for (GFXComponent * component : m_group)
					if (component->isVisible())
						component->draw(modelMat, handles);
			}

			bool checkGroupEvents(gfx::engine::GLContent * content)
			{
				for (int ix = m_group.size() - 1; ix >= 0; --ix)
					if (m_group[ix]->isEnabled() && m_group[ix]->checkEvents(content))
						return true;
				return false;
			}

			void updateGroup(gfx::engine::GLContent * content)
			{
				for (GFXComponent * component : m_group)
					component->update(content);
			}

			void initGroup(GFXManager * manager, GFXComponent * parent)
			{
				for (GFXComponent * component : m_group)
					component->init(manager, parent);
			}

			void validateGroup()
			{
				for (GFXComponent * component : m_group)
					component->validate();
			}

			glm::vec2 getMinimumBounds(float padding)
			{
				float left = 1000000;
				float right = 0.0f;
				float top = 0.0f;
				float bottom = 1000000;
				for (GFXComponent * component : m_group)
				{
					glm::vec2 size = component->getSize();
					glm::vec2 pos = component->getPos();
					left = min(pos.x, left);
					right = max(pos.x + size.x, right);
					bottom = min(pos.y, bottom);
					top = max(pos.y + size.y, top);
				}
				return glm::vec2(right + padding, top + padding);
			}

			GFXID verifyId(GFXID id)
			{
				if (id == GFX_NULL_ID)
					id = GFX_DEFAULT_ID;
				if (!componentExists(id))
					return id;

				int extra = 1;
				std::string newId;
				do
				{
					newId = alib::StringFormat("%0%1").arg(id).arg(extra).str();
					extra++;
				} while (getComponent(newId));

				return newId;
			}

			void bringForward(GFXComponent * component)
			{
				for (int i = 0; i < m_group.size(); ++i)
					if (m_group[i] == component)
					{
						if (i != m_group.size() - 1)
						{
							m_group.erase(m_group.begin() + i);
							m_group.insert(m_group.begin() + i + 1, component);
						}
						break;
					}
			}
			void bringBackward(GFXComponent * component)
			{
				for (int i = 0; i < m_group.size(); ++i)
					if (m_group[i] == component)
					{
						if (i != 0)
						{
							m_group.erase(m_group.begin() + i);
							m_group.insert(m_group.begin() + i - 1, component);
						}
						break;
					}
			}
			void bringToFront(GFXComponent * component)
			{
				for (int i = 0; i < m_group.size(); ++i)
					if (m_group[i] == component)
					{
						if (i != m_group.size() - 1)
						{
							m_group.erase(m_group.begin() + i);
							m_group.push_back(component);
						}
						break;
					}
			}
		private:
			std::vector<GFXComponent*> m_group;
		};

		// MANAGERS
		class GFXManager : public GFXComponent, public GFXGroup
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

			GFXComponent * addComponent(GFXComponent * component)
			{
				add(component);
				return this;
			}

			GFXID addId(GFXID id)
			{
				//id = verifyId(id);
				//gfxids.push_back(id);
				return id;
			}

			void setId(GFXID id, GFXComponent * component)
			{
				for (int ix = 0; ix < m_gfxids.size(); ++ix)
					if (m_gfxids[ix] == component->getId())
						m_gfxids.erase(m_gfxids.begin() + ix);
				component->setId(addId(id));
			}

			bool checkEvents(gfx::engine::GLContent * content)
			{
				return checkGroupEvents(content);
			}
			void update(gfx::engine::GLContent * content)
			{
				updateGroup(content);
			}

			void draw(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles)
			{
				drawGroup(modelMat, handles);
			}

			void validate()
			{
				validateGroup();
			}

			GFXComponent * init(GFXManager * manager, GFXComponent * parent)
			{
				initGroup(manager, parent);
				return this;
			}

			void init()
			{
				init(this, this);
			}

			void setFocused(GFXComponent * component)
			{
				m_focused = component;
			}
			bool isFocused(GFXComponent * component)
			{
				return component == m_focused;
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
			std::vector<GFXID> m_gfxids;
			GFXComponent * m_focused = GFX_NULLPTR;
		};

		class GFXContainer : public GFXComponent, public GFXGroup
		{
		public:
			GFXContainer * addComponent(GFXComponent * component)
			{
				add(component);
				return this;
			}

			GFXComponent * init(GFXManager * manager, GFXComponent * parent)
			{
				setManager(manager);
				setParent(parent);
				setId(manager->addId("container"));
				if (m_colorStyle == GFX_NULL_COLORSTYLE)
					inheritColorStyle();
				initGroup(manager, this);
				return this;
			}

			void validate()
			{
				validateGroup();
			}

			void update(gfx::engine::GLContent * content)
			{
				updateGroup(content);
			}

			bool checkEvents(gfx::engine::GLContent * content)
			{
				return checkGroupEvents(content);
			}

			void draw(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles)
			{
				drawMesh(modelMat, handles);
				drawGroup(modelMat * getRelativeModelMat(), handles);
			}

			GFXContainer() : GFXComponent() {}
			GFXContainer(glm::vec2 pos, glm::vec2 size, glm::vec4 color) : GFXComponent() 
			{
				std::vector<glm::vec3> v = gfx::PrimativeGenerator::generate_square_mesh(1, 1);
				GFXMesh miniMesh = gfx::gui::GFXMesh(
					pos,
					size,
					gfx::PrimativeGenerator::pack_object(&v, GEN_COLOR, gfx::WHITE),
					color);
				m_pos = miniMesh.m_pos;
				m_size = miniMesh.m_size;
				m_vao = miniMesh.m_vao;
				m_buffer = miniMesh.m_buffer;
				m_dataSize = miniMesh.m_dataSize;
				m_rotation = miniMesh.m_rotation;
				m_theta = miniMesh.m_theta;
				m_color = miniMesh.m_color;
			}
			GFXContainer(GFXMesh mesh) : GFXComponent(mesh) {}
		private:
		};
		

		class GFXLabel : public GFXComponent
		{
		public:

			GFXComponent * init(GFXManager * manager, GFXComponent * parent)
			{
				setManager(manager);
				setParent(parent);
				setId(manager->addId("label"));
				m_font = GFXFont(GFX_Courier_FONT);
				return this;
			}
			void validate()
			{

			}
			void update(gfx::engine::GLContent * content)
			{

			}
			bool checkEvents(gfx::engine::GLContent * content)
			{
				return false;
			}
			void draw(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles)
			{
				for (int ix = 0; ix < m_text.length(); ix++)
					m_font.draw(m_text[ix], modelMat, glm::vec2(ix * m_size.x / 2, m_size.y / 2) + m_pos, m_size, handles);
			}

			float getLength()
			{
				return m_size.x / 2 * m_text.size();
			}

			void center()
			{
				m_pos -= glm::vec2(getLength() / 2, m_size.y);
			}

			void setText(std::string text)
			{
				m_text = text;
			}

			void setColor(glm::vec4 color)
			{
				m_font.setColor(color);
			}

			GFXLabel(std::string text, int fontSize)
			{
				m_text = text;
				m_size = glm::vec2(fontSize, fontSize);
			}

		protected:
			GFXFont m_font;
			std::string m_text;
		};


		// CLICKABLES
		class GFXClickable : public GFXComponent
		{
		public:
			bool onPressed(bool isTrue)
			{
				if (isTrue)
				{
					m_onDown = true;
					m_manager->setFocused(this);
				}
				return isTrue;
			}
			bool onDown(bool isTrue)
			{
				if (isTrue)
				{
					m_heldCounter++;
					m_onDown = true;
				}
				return isTrue;
			}
			bool onReleased(bool isTrue)
			{
				if (isTrue)
				{
					m_heldCounter = 0;
					m_onDown = false;
					m_onReleased = true;
				}
				else
					m_onReleased = false;
				return isTrue;
			}

			bool isPressed(gfx::engine::GLContent * content)
			{ 
				return content->getKeyboardEvents()->isKeyPressed(VK_LBUTTON) &&
					isInside(content);
			}
			bool isDown(gfx::engine::GLContent * content)
			{
				return content->getKeyboardEvents()->isKeyDown(VK_LBUTTON) &&
					m_onDown &&
					isInside(content);
			}
			bool isHeld(gfx::engine::GLContent * content) 
			{
				return m_heldCounter > m_heldThreshold;
			}
			bool isReleased(gfx::engine::GLContent * content)
			{
				return content->getKeyboardEvents()->isKeyReleased(VK_LBUTTON) &&
					m_onDown;
			}
			bool isDragging(gfx::engine::GLContent * content)
			{
				return	glm::length(content->getMouseDelta()) > 0 && m_onDown;
			}
			bool isHovering(gfx::engine::GLContent * content)
			{
				return isInside(content);
			}
			bool isReleasedOver(gfx::engine::GLContent * content)
			{
				return isReleased(content) && isHovering(content);
			}
			bool isHeldOver(gfx::engine::GLContent * content)
			{
				return m_heldCounter > m_heldThreshold && isHovering(content);
			}

			bool isKeyPressed(gfx::engine::GLContent * content, char key)
			{
				return content->getKeyboardEvents()->isKeyPressed(key) && m_manager->isFocused(this);
			}
			bool isKeyDown(gfx::engine::GLContent * content, char key)
			{
				return content->getKeyboardEvents()->isKeyDown(key) && m_manager->isFocused(this);
			}
			bool isKeyTyped(gfx::engine::GLContent * content, char key)
			{
				return content->getKeyboardEvents()->isKeyTyped(key) && m_manager->isFocused(this);
			}
			bool isKeyReleased(gfx::engine::GLContent * content, char key)
			{
				return content->getKeyboardEvents()->isKeyReleased(key) && m_manager->isFocused(this);
			}

			char getKeyTyped(gfx::engine::GLContent * content)
			{
				if (m_manager->isFocused(this))
					return content->getKeyboardEvents()->getTyped();
				return -1;
			}

			// not finished
			bool isDoubleClicked(gfx::engine::GLContent * content)
			{
				return m_doubleClickCounter - clickTime(content) < m_doubleClickThreshold;
			}

			void check(gfx::engine::GLContent * content)
			{
				onPressed(isPressed(content));
				onDown(isDown(content));
				onReleased(isReleased(content));
				//onKeyPressed(isKeyPressed(content, content->getKeyboardEvents()->getKeyPressed()));
				//onKeyDown(isKeyDown(content, content->getKeyboardEvents()->getKeyPressed()));
				//onKeyReleased(isKeyReleased(content, content->getKeyboardEvents()->getKeyPressed()));
			}

			GFXClickable() {}
			GFXClickable(GFXMesh mesh) : GFXComponent(mesh) {}
		protected:

			int clickTime(gfx::engine::GLContent * content)
			{
				if (isPressed(content))
				{
					if (m_doubleClickCounter == 0)
						m_doubleClickCounter = content->getFrames();
					else
						return content->getFrames();
				}
				return 0;
			}

			bool m_onDown = false;
			bool m_onReleased = false;
						
			int m_heldCounter = 0;
			int m_heldThreshold = 25;
			int m_doubleClickCounter = 0;
			int m_doubleClickThreshold = 50;
		};



		class GFXButton : public GFXClickable, public GFXLinker<GFXButton>
		{
		public:

			void update(gfx::engine::GLContent * content)
			{
				m_back->setPos(m_pos);
				if(m_toggledState)
					m_back->setColor(glm::vec3(m_colorStyle->colors[0]) / 2.0f);
				
				//if (isHovering(content))
				//	m_back->setColor(m_colorStyle->colors[0] * 1.5f);
				//else
				//	m_back->setColor(m_colorStyle->colors[0]);
				if (isDown(content))
					m_back->setColor(m_colorStyle->colors[0] / 1.5f);
				else
					m_back->setColor(m_colorStyle->colors[0]);
					
			}
			bool checkEvents(gfx::engine::GLContent * content)
			{
				check(content);
				onButtonPressed(content);
				onButtonDown(content);
				onButtonReleased(content);
				return m_onDown;
			}
			void draw(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles)
			{
				m_back->setPos(m_pos);
				m_back->drawMesh(modelMat, handles);
				if(m_text != "")
					 m_label->draw(modelMat, handles);
			}
		
			GFXComponent * init(GFXManager * manager, GFXComponent * parent)
			{
				setManager(manager);
				setParent(parent);
				setId(manager->addId("button"));
				if (m_colorStyle == GFX_NULL_COLORSTYLE)
					inheritColorStyle();

				m_back = new GFXRectangleMesh(m_pos, m_size);
				m_back->setColor(m_colorStyle->colors[0]);
				if (m_text != "")
				{
					m_label = new GFXLabel(m_text, GFX_GUI_DEFAULT_FONT_SIZE);
					m_label->init(manager, this);
					m_label->setColor(m_colorStyle->colors[2]);
					m_label->setPos(m_pos + m_size / 2.0f);
					m_label->center();
				}
				return this;
			}

			void validate()
			{
				m_back->setPos(m_pos);
				m_back->setSize(m_size);
				if (m_text != "")
				{
					m_label->setPos(m_pos + m_size / 2.0f);
					m_label->center();
				}
			}
			
			void onButtonPressed(gfx::engine::GLContent * content)
			{
				if (isPressed(content))
				{
					m_toggledState = !m_toggledState && m_isToggleable;
					callTrigger(&GFXButton::onButtonPressed);
				}
			}
			void onButtonDown(gfx::engine::GLContent * content)
			{
				if (isDown(content))
				{
					callTrigger(&GFXButton::onButtonDown);
				}
			}
			void onButtonReleased(gfx::engine::GLContent * content)
			{
				if (isReleased(content))
				{
					callTrigger(&GFXButton::onButtonReleased);
				}
			}
			
			bool isToggled()
			{
				return m_toggledState;
			}

			void setText(std::string text)
			{
				m_text = text;
				m_label->setText(text);
			}

			GFXButton(glm::vec2 pos, glm::vec2 size)
			{
				m_pos = pos;
				m_size = size;
				m_text = "";
			}
			GFXButton(glm::vec2 pos, glm::vec2 size, std::string text)
			{
				m_pos = pos;
				m_size = size;
				m_text = text;
			}
						
		protected:			
			bool m_toggledState = false;
			bool m_isToggleable = false;

			std::string	m_text;
			GFXRectangleMesh * m_back;
			GFXLabel * m_label;
		};

		class GFXWindow : public GFXClickable, public GFXLinker<GFXWindow>
		{
		public:
			void update(gfx::engine::GLContent * content)
			{
				// get local mouse delta
				glm::vec2 delta = content->getMouseDelta();				

				// if top grab bar is being dragged
				if (m_bar->isDragging(content))
				{
					m_pos += delta;
					confinePositionToWindow(content);
					validate();
				}

				glm::vec2 pos = m_pos;
				glm::vec2 size = m_size;
				
				// if the window wants to be resized and isnt maximised
				if (m_isResizable && !m_maximised)
				{
					// horizontal grab bars resize
					if (m_isResizableX)
						if (m_leftResizeBar->isDragging(content))
						{						
							pos.x += delta.x;
							size.x -= delta.x;
							if (!isSmallerThanMin(size, m_minSize))
							{
								m_pos = pos;
								m_size = size;
								confineSizeToContent(content);
								confinePositionToWindow(content);
								validate();
							}
						}
						else if (m_rightResizeBar->isDragging(content))
						{
							size.x += delta.x;
							if (!isSmallerThanMin(size, m_minSize))
							{
								m_size = size;
								confineSizeToContent(content);
								confinePositionToWindow(content);
								validate();
							}
						}
					// vetical grab bars resize
					if (m_isResizableY)
						if (m_bottomResizeBar->isDragging(content))
						{
							size.y += delta.y;
							if (!isSmallerThanMin(size, m_minSize))
							{
								m_pos = pos;
								m_size = size;
								confineSizeToContent(content);
								confinePositionToWindow(content);
								validate();
							}
						}	
					// corner grabs resize
					if (m_isResizableX && m_isResizableY)
						if (m_bottomLeftResizeBar->isDragging(content))
						{
							pos.x += delta.x;
							size.x -= delta.x;
							size.y += delta.y;
							if (!isSmallerThanMin(size, m_minSize))
							{
								m_pos = pos;
								m_size = size;
								confineSizeToContent(content);
								confinePositionToWindow(content);
								validate();
							}
						}
						else if (m_bottomRightResizeBar->isDragging(content))
						{
							size.x += delta.x;
							size.y += delta.y;
							if (!isSmallerThanMin(size, m_minSize))
							{
								m_pos = pos;
								m_size = size;
								confineSizeToContent(content);
								confinePositionToWindow(content);
								validate();
							}
						}
				} // if m_resizeable

				// update children
				m_components.update(content);
				m_group.updateGroup(content);

			}
			bool checkEvents(gfx::engine::GLContent * content)
			{
				onWindowPressed(content);
				onWindowDown(content);
				onWindowReleased(content);
				onWindowDragging(content);
				onWindowMove(content);
				onResize(content);
				onClose(content);
				onWindowScaled(content);
				bool focused = m_components.checkEvents(content) || m_group.checkGroupEvents(content) || m_onDown;
				if(focused) 
					focus();
				return focused;
			}
			void draw(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles)
			{
				m_components.draw(modelMat*getRelativeModelMat(), handles);
				m_group.drawGroup(modelMat*getRelativeModelMat(), handles);
			}

			GFXWindow * addComponent(GFXComponent * component)
			{
				m_group.add(component);
				return this;
			}

			void validate()
			{
				m_minSize = m_group.getMinimumBounds(GFX_GUI_DEFAULT_PADDING);
				inflateToContent();

				m_components.setPos(glm::vec2());
				m_components.setSize(m_size);

				m_leftResizeBar->setSize(glm::vec2(m_deadzone, m_size.y));
				m_rightResizeBar->setPos(glm::vec2(m_size.x - m_deadzone, 0));
				m_rightResizeBar->setSize(glm::vec2(m_deadzone, m_size.y));
				m_bottomResizeBar->setPos(glm::vec2(0, m_size.y - m_deadzone));
				m_bottomResizeBar->setSize(glm::vec2(m_size.x, m_deadzone));
				m_bottomRightResizeBar->setPos(glm::vec2(m_size.x - m_deadzone, m_size.y - m_deadzone));
				m_bottomLeftResizeBar->setPos(glm::vec2(0, m_size.y - m_deadzone));

				m_bar->setSize(glm::vec2(m_size.x, m_topBarSize));

				m_close->setPos(glm::vec2(m_size.x - m_topBarSize, 0));
				m_maxmin->setPos(glm::vec2(m_size.x - m_topBarSize*2, 0));

				m_group.validateGroup();
				m_components.validate();				
			}

			GFXComponent * init(GFXManager * manager, GFXComponent * parent)
			{
				setManager(manager);
				setParent(parent);
				setId(manager->addId("window"));
				if (m_colorStyle == GFX_NULL_COLORSTYLE)
					inheritColorStyle();

				// window ui components container
				m_components = gfx::gui::GFXContainer(glm::vec2(), m_size, m_colorStyle->colors[2]);
				m_components.setAlpha(0.8f);
				m_components.setColorStyle(m_colorStyle);

				// alter colorstyle for each component
				GFXColorStyle_T * colorStyle = new GFXColorStyle_T;
				colorStyle->colors[0] = glm::vec4(0);
				colorStyle->colors[1] = m_colorStyle->colors[1];
				colorStyle->colors[2] = m_colorStyle->colors[2];

				// left resize grab
				m_leftResizeBar = new gfx::gui::GFXButton(glm::vec2(), glm::vec2(m_deadzone, m_size.y));
				m_components.addComponent(m_leftResizeBar);
				m_leftResizeBar->setColorStyle(colorStyle);
				// right resize grab
				m_rightResizeBar = new gfx::gui::GFXButton(glm::vec2(m_size.x - m_deadzone, 0), glm::vec2(m_deadzone, m_size.y));
				m_components.addComponent(m_rightResizeBar);
				m_rightResizeBar->setColorStyle(colorStyle);
				// bottom resize grab
				m_bottomResizeBar = new gfx::gui::GFXButton(glm::vec2(0, m_size.y - m_deadzone), glm::vec2(m_size.x, m_deadzone));
				m_components.addComponent(m_bottomResizeBar);
				m_bottomResizeBar->setColorStyle(colorStyle);
				// bottom left resize grab
				m_bottomLeftResizeBar = new gfx::gui::GFXButton(glm::vec2(0, m_size.y - m_deadzone), glm::vec2(m_deadzone, m_deadzone));
				m_components.addComponent(m_bottomLeftResizeBar);
				m_bottomLeftResizeBar->setColorStyle(colorStyle);
				// bottom right resize grab
				m_bottomRightResizeBar = new gfx::gui::GFXButton(glm::vec2(m_size.x - m_deadzone, m_size.y - m_deadzone), glm::vec2(m_deadzone, m_deadzone));
				m_components.addComponent(m_bottomRightResizeBar);
				m_bottomRightResizeBar->setColorStyle(colorStyle);

				// top move grab
				m_bar = new gfx::gui::GFXButton(glm::vec2(), glm::vec2(m_size.x, m_topBarSize), m_title);
				m_components.addComponent(m_bar);

				// close window button
				m_close = new gfx::gui::GFXButton(glm::vec2(m_size.x - m_topBarSize, 0), glm::vec2(m_topBarSize, m_topBarSize));
				m_components.addComponent(m_close);
				colorStyle->colors[0] = colorStyle->colors[2];
				m_close->setColorStyle(colorStyle);

				// scale window button
				m_maxmin = new gfx::gui::GFXButton(glm::vec2(m_size.x - m_topBarSize * 2, 0), glm::vec2(m_topBarSize, m_topBarSize));
				m_components.addComponent(m_maxmin);
				colorStyle->colors[0] = colorStyle->colors[1];
				m_maxmin->setColorStyle(colorStyle);
				
				// initialise container
				m_components.init(manager, this);
				m_group.initGroup(manager, this);

				return this;
			}

			void onWindowPressed(gfx::engine::GLContent * content)
			{
				if (onPressed(isPressed(content)))
				{
					callTrigger(&GFXWindow::onWindowPressed);
				}
			}
			void onWindowDown(gfx::engine::GLContent * content)
			{
				if (onDown(isDown(content)))
				{
					callTrigger(&GFXWindow::onWindowDown);
				}
			}
			void onWindowReleased(gfx::engine::GLContent * content)
			{
				if (onReleased(isReleased(content)))
				{
					callTrigger(&GFXWindow::onWindowReleased);
				}
			}
			void onWindowDragging(gfx::engine::GLContent * content)
			{
				if (isDragging(content))
				{
					callTrigger(&GFXWindow::onWindowDragging);
				}
			}
			void onWindowMove(gfx::engine::GLContent * content)
			{
				if (m_bar->isDragging(content))
				{
					callTrigger(&GFXWindow::onWindowMove);
				}
			}

			void closeWindow()
			{
				if (!m_manager->removeComponent(this))
				{
					CERROR("failed to close window.", __FILE__, __LINE__, "GFXWindow", __func__);
				}
			}
			void onClose(gfx::engine::GLContent * content)
			{
				if (m_close->isReleasedOver(content))
				{
					callTrigger(&GFXWindow::onClose);
					closeWindow();
				}
			}

			void maximiseWindow(gfx::engine::GLContent * content)
			{
				m_oldPos = m_pos;
				m_oldSize = m_size;

				m_pos = glm::vec2();
				m_size = content->getWindowSize();

				validate();
			}
			void minimiseWindow()
			{
				m_pos = m_oldPos;
				m_size = m_oldSize;

				validate();
			}
			void toggleMaximise(gfx::engine::GLContent * content)
			{
				m_maximised = !m_maximised;
				if (m_maximised)
					maximiseWindow(content);
				else
					minimiseWindow();
			}
			void onWindowScaled(gfx::engine::GLContent * content) 
			{
				if (m_maxmin->isReleasedOver(content))
				{
					toggleMaximise(content);
					callTrigger(&GFXWindow::onWindowScaled);
				}
			}

			void onResize(gfx::engine::GLContent * content)
			{
				if(m_isResizable && !m_maximised)
					if (m_leftResizeBar->isDragging(content) || m_rightResizeBar->isDragging(content) || m_bottomResizeBar->isDragging(content))
					{
						callTrigger(&GFXWindow::onResize);
					}
			}
			
			void setResizeable(bool resizable)
			{
				m_isResizable = resizable;
			}
			void setResizableVeritcal(bool resizeable)
			{
				m_isResizableX = !resizeable;
				m_isResizableY = resizeable;
			}
			void setResizableHorizontal(bool resizeable)
			{
				m_isResizableX = resizeable;
				m_isResizableY = !resizeable;
			}
			void setResizableBoth(bool resizeable)
			{
				m_isResizableX = resizeable;
				m_isResizableY = resizeable;
			}
 
			void focus()
			{
				if(m_parent != GFX_NULLPTR)
						dynamic_cast<GFXGroup*>(m_parent)->bringToFront(this);
			}

			GFXWindow(glm::vec2 pos, glm::vec2 size)
			{
				m_size = size;
				m_pos = pos;
				m_minSize = glm::vec2();
				m_title = "Untitled Window";
			}
		protected:
			bool isSmallerThanMin(glm::vec2 a, glm::vec2 b)
			{
				return a.x < b.x || a.y < b.y;
			}
			void confineSizeToContent(gfx::engine::GLContent * content)
			{
				if (m_pos.x + m_size.x > content->getWindowSize().x)
					m_size.x += content->getWindowSize().x - (m_pos.x + m_size.x);
				if (m_pos.x < 0)
					m_size.x += m_pos.x;
				if (m_pos.y + m_size.y> content->getWindowSize().y)
					m_size.y += content->getWindowSize().y - (m_pos.y + m_size.y);
			}
			void confinePositionToWindow(gfx::engine::GLContent * content)
			{
				m_pos.x = min(content->getWindowSize().x - m_size.x, m_pos.x);
				m_pos.x = max(0, m_pos.x);
				m_pos.y = min(content->getWindowSize().y - m_size.y, m_pos.y);
				m_pos.y = max(0, m_pos.y);				
			}
			void inflateToContent()
			{
				m_size.x = max(m_minSize.x, m_size.x);
				m_size.y = max(m_minSize.y, m_size.y);
			}

			bool m_isResizable = true;
			bool m_isResizableX = m_isResizable;
			bool m_isResizableY = m_isResizable;

			bool m_windowMoving = false;
			bool m_maximised = false;
		
			float m_deadzone = 10;
			float m_topBarSize = 20;
			glm::vec2 m_oldPos;
			glm::vec2 m_oldSize;
			glm::vec2 m_minSize;;
			GFXButton * m_bar;
			GFXButton * m_leftResizeBar;
			GFXButton * m_rightResizeBar;
			GFXButton * m_bottomResizeBar;
			GFXButton * m_bottomLeftResizeBar;
			GFXButton * m_bottomRightResizeBar;
			GFXButton * m_close;
			GFXButton * m_maxmin;
			std::string m_title;
			GFXGroup m_group;
			GFXContainer m_components;
		};

		class GFXSpinner : public GFXContainer, public GFXLinker<GFXSpinner>
		{
		public:
			GFXComponent * init(GFXManager * manager, GFXComponent * parent)
			{
				setManager(manager);
				setParent(parent);
				setId(manager->addId("spinner"));
				if (m_colorStyle == GFX_NULL_COLORSTYLE)
					inheritColorStyle();

				m_label = new gfx::gui::GFXButton(glm::vec2(m_size.x / 6, 0), glm::vec2(m_size.x / 6 * 4, m_size.y), "0");				
				addComponent(m_label);

				m_plus = new gfx::gui::GFXButton(glm::vec2(), glm::vec2(m_size.x / 6, m_size.y), "+");
				addComponent(m_plus);				

				m_minus = new gfx::gui::GFXButton(glm::vec2(m_size.x / 6 * 5, 0), glm::vec2(m_size.x / 6, m_size.y), "-");
				addComponent(m_minus);				

				initGroup(manager, this);

				return this;
			}
			void validate()
			{
				m_label->setPos(glm::vec2(m_size.x / 6, 0));
				m_label->setSize(glm::vec2(m_size.x / 6*4, m_size.y));

				m_plus->setPos(glm::vec2());
				m_plus->setSize(glm::vec2(m_size.x / 6, m_size.y));

				m_minus->setPos(glm::vec2(m_size.x / 6 * 5, 0));
				m_minus->setSize(glm::vec2(m_size.x / 6, m_size.y));

				validateGroup();
			}
			void update(gfx::engine::GLContent * content)
			{
				updateGroup(content);
			}
			bool checkEvents(gfx::engine::GLContent * content)
			{
				onIncrease(content);
				onDecrease(content);
				onReset(content);
				return checkGroupEvents(content);
			}
			void draw(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles)
			{
				drawGroup(modelMat * getRelativeModelMat(), handles);
			}

			void onIncrease(gfx::engine::GLContent * content)
			{
				if (m_plus->isReleasedOver(content) || m_plus->isHeldOver(content))
				{
					m_value += m_inc;
					setValue(m_value);
					validate();
					callTrigger(&GFXSpinner::onIncrease);
				}
			}
			void onDecrease(gfx::engine::GLContent * content)
			{
				if (m_minus->isReleasedOver(content) || m_minus->isHeldOver(content))
				{
					m_value -= m_inc;
					setValue(m_value);
					validate();
					callTrigger(&GFXSpinner::onDecrease);
				}
			}
			void onReset(gfx::engine::GLContent * content)
			{
				if (m_label->isReleasedOver(content))
				{
					setValue(0);
					validate();
					callTrigger(&GFXSpinner::onReset);
				}
			}
			
			void setValue(float amount)
			{
				m_value = amount;
				char number[24];
				sprintf(number, "%.1f", m_value);
				m_label->setText(number);
			}
			float getValue()
			{
				return m_value;
			}

			GFXSpinner(glm::vec2 pos, glm::vec2 size, float value, float inc)
			{
				m_pos = pos;
				m_size = size;
				m_value = value;
				m_inc = inc;
			}
		protected:
			
			float m_value;
			float m_inc;
			GFXButton * m_plus;
			GFXButton * m_minus;
			GFXButton * m_label;
		};

		class GFXTextEdit : public GFXClickable
		{
		public:

			GFXComponent * init(GFXManager * manager, GFXComponent * parent)
			{
				setManager(manager);
				setParent(parent);
				setId(manager->addId("textEdit"));
				m_font = GFXFont(GFX_Courier_FONT);
				return this;
			}
			void validate()
			{

			}
			void update(gfx::engine::GLContent * content)
			{

			}
			bool checkEvents(gfx::engine::GLContent * content)
			{
				check(content);
				keyTyped(content);
				return m_onDown;
			}
			void draw(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles)
			{
				++m_cursorBlinkTimer;
				bool endOfText = true;
				float ix, iy;
				int ic;
				for (ix = 0, iy = 0, ic = 0; ic < m_text.length(); ix += m_fontSize / 2.0f, ic++)
				{		
					if (ix + m_fontSize / 2 > m_size.x)
					{						
						iy += m_fontSize;
						ix = 0;
						if (!m_isMultiline)
						{
							endOfText = false;
							break;
						}
					}
					while (m_text[ic] == '\n')
					{ 
						if (m_cursorPosition == ic)
							if (canDrawCursor())
								drawCursor(modelMat, handles, ix, iy);
						iy += m_fontSize;
						ix = 0;		
						ic++;
						if (iy + m_fontSize > m_size.y)
						{
							endOfText = false;
							break;
						}						
					}
					if (iy + m_fontSize > m_size.y)
					{
						endOfText = false;
						break;
					}		
					if (ic == m_text.length())
						break;
					m_font.draw(m_text[ic], modelMat, glm::vec2(ix, iy) + m_pos, glm::vec2(m_fontSize), handles);				
					if (m_cursorPosition == ic)
						if (canDrawCursor())
							drawCursor(modelMat, handles, ix, iy);
				}
				if(canDrawCursor())
					if (!endOfText)
						drawCursor(modelMat, handles, m_size.x, iy - m_fontSize);
					else if(m_cursorPosition == m_text.size() || m_text.size() == 0)
						drawCursor(modelMat, handles, ix, iy);
			}

			bool canDrawCursor()
			{
				return (m_cursorBlinkTimer % m_cursorBlinkTime) < m_cursorBlinkTime / 2 && m_manager->isFocused(this);
			}
			void drawCursor(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles, float ix, float iy)
			{
				m_font.draw('|', modelMat, glm::vec2(ix - m_fontSize / 3, iy - m_fontSize / 3) + m_pos, glm::vec2(m_fontSize, m_fontSize * 1.5), handles);
			}
			
			glm::vec2 getCursorPosOffset(int pos)
			{
				float ix, iy;
				int ic;
				for (ix = 0, iy = 0, ic = 0; ic < m_text.length() && ic < pos; ix += m_fontSize / 2.0f, ic++)
				{
					if (ix + m_fontSize / 2 > m_size.x)
					{
						iy += m_fontSize;
						ix = 0;
						if (!m_isMultiline)
							break;
					}
					while (m_text[ic] == '\n')
					{
						iy += m_fontSize;
						ix = 0;
						ic++;
						if (iy + m_fontSize > m_size.y)
							break;
					}
					if (iy + m_fontSize > m_size.y || ic == m_text.length())
						break;
				}
				return glm::vec2(ix, iy);
			}
			int getEOLPos(int start)
			{
				float ix;
				int ic = start;
				if (m_text[ic] == '\n')
					ic++;
				for (ix = getCursorPosOffset(start).x, ic = start; ic < m_text.length(); ix += m_fontSize / 2.0f, ic++)
				{
					if (ix + m_fontSize / 2 > m_size.x)
						break;
					while (m_text[ic] == '\n')
						break;
				}
				return ic;
			}
			int getSOLPos(int start)
			{
				float ix;
				int ic;
				for (ix = getCursorPosOffset(start).x, ic = start; ic >= 0; ix -= m_fontSize / 2.0f, ic--)
				{
					if (ix <= 0)
						break;
					while (m_text[ic] == '\n')
						break;
				}
				return ic;
			}

			void setText(std::string text)
			{
				m_text = text;
			}
			std::string getText()
			{
				return m_text;
			}

			void setColor(glm::vec4 color)
			{
				m_font.setColor(color);
			}

			void insertChar(char c)
			{
				m_text.insert(m_cursorPosition,1,c);
				m_cursorPosition++;
				m_cursorBlinkTimer = 0;
			}

			void keyTyped(gfx::engine::GLContent * content)
			{
				char key = content->getKeyboardEvents()->getTyped();
				if (key != -1 && m_manager->isFocused(this))
				{
					insertChar(key);					
				}
				if (isKeyTyped(content, VK_RETURN) && m_manager->isFocused(this))
				{
					insertChar('\n');
				}
				if (isKeyTyped(content, VK_BACK) && m_manager->isFocused(this))
				{
					if (m_text.size() > 0)
					{
						m_text.erase(m_text.begin() + m_cursorPosition - 1);
						m_cursorPosition--;
						m_cursorBlinkTimer = 0;
					}
				}
				if (isKeyTyped(content, VK_TAB) && m_manager->isFocused(this))
				{
					insertChar(' ');
					insertChar(' ');
				}
				if (isKeyTyped(content, VK_RIGHT) && m_manager->isFocused(this))
				{
					if (m_cursorPosition < m_text.size())
					{
						m_cursorPosition++;
						m_cursorBlinkTimer = 0;
					}
				}
				if (isKeyTyped(content, VK_LEFT) && m_manager->isFocused(this))
				{
					if (m_cursorPosition > 0)
					{
						m_cursorPosition--;
						m_cursorBlinkTimer = 0;
					}
				}
				if (isKeyTyped(content, VK_HOME) && m_manager->isFocused(this))
				{
					//m_cursorPosition = getSOLPos(m_cursorPosition);
				}
				if (isKeyTyped(content, VK_END) && m_manager->isFocused(this))
				{
					//m_cursorPosition = getEOLPos(m_cursorPosition);
				}

				if (isKeyTyped(content, VK_DOWN) && m_manager->isFocused(this))
				{
					CINFO(alib::StringFormat("%0").arg(m_cursorPosition).str());
				}
			}

			void setMultiline(bool isMultiline)
			{
				m_isMultiline = isMultiline;
			}

			GFXTextEdit(std::string text, int fontSize, glm::vec2 pos, glm::vec2 size)
			{
				m_fontSize = fontSize;
				m_text = alib::StringFormat(text).str();
				m_cursorPosition = m_text.size();
				m_pos = pos;
				m_size = size;
			}

		protected:
			int m_fontSize = GFX_GUI_DEFAULT_FONT_SIZE;
			int m_cursorPosition = 0;
			bool m_isMultiline = false;
			int m_cursorBlinkTimer = 0;
			int m_cursorBlinkTime = 50;
			GFXFont m_font;
			std::string m_text;
		};

		class GFXScrollBar : public GFXContainer, public GFXLinker<GFXScrollBar>
		{
		public:
			void update(gfx::engine::GLContent * content)
			{
				if (m_bar->isDragging(content))
				{
					if (m_isVertical)
						setValue((m_bar->m_pos.y + content->getMouseDelta().y - m_size.x) / (m_size.y - m_size.x * 3));
					else
						setValue((m_bar->m_pos.x + content->getMouseDelta().x - m_size.y) / (m_size.x - m_size.y * 3));
				}
				
				if (m_bar->isReleasedOver(content))
					snapValue();

				updateGroup(content);
			}
			bool checkEvents(gfx::engine::GLContent * content)
			{
				onUp(content);
				onDown(content);
				onBarMove(content);
				return checkGroupEvents(content);
			}
			void draw(glm::mat4 modelMat, gfx::engine::MeshHandle_T handles)
			{
				drawGroup(modelMat * getRelativeModelMat(), handles);
			}

			GFXComponent * init(GFXManager * manager, GFXComponent * parent)
			{
				setManager(manager);
				setParent(parent);
				setId(manager->addId("scrollbar"));
				if (m_colorStyle == GFX_NULL_COLORSTYLE)
					inheritColorStyle();

				if (m_isVertical)
				{
					m_down = new gfx::gui::GFXButton(glm::vec2(0, m_size.y - m_size.x), glm::vec2(m_size.x, m_size.x), "\\/");
					addComponent(m_down);

					m_up = new gfx::gui::GFXButton(glm::vec2(), glm::vec2(m_size.x, m_size.x), "/\\");
					addComponent(m_up);

					m_bar = new gfx::gui::GFXButton(glm::vec2(0, m_size.x), glm::vec2(m_size.x, m_size.x), "=");
					addComponent(m_bar);
				}
				else
				{
					m_down = new gfx::gui::GFXButton(glm::vec2(m_size.x - m_size.y, 0), glm::vec2(m_size.y, m_size.y), ">");
					addComponent(m_down);

					m_up = new gfx::gui::GFXButton(glm::vec2(), glm::vec2(m_size.y, m_size.y), "<");
					addComponent(m_up);

					m_bar = new gfx::gui::GFXButton(glm::vec2(m_size.y, 0), glm::vec2(m_size.y, m_size.y), "||");
					addComponent(m_bar);
				}

				initGroup(manager, this);

				return this;
			}

			void validate()
			{
				if (m_isVertical)
				{
					m_down->setPos(glm::vec2(0, m_size.y - m_size.x));
					m_down->setSize(glm::vec2(m_size.x, m_size.x));
					m_up->setPos(glm::vec2());
					m_up->setSize(glm::vec2(m_size.x, m_size.x));
					m_bar->setPos(glm::vec2(0, getValue() * (m_size.y - m_size.x * 3) + m_size.x));
					m_bar->setSize(glm::vec2(m_size.x, m_size.x));
				}
				else
				{
					m_down->setPos(glm::vec2(m_size.x - m_size.y, 0));
					m_down->setSize(glm::vec2(m_size.y, m_size.y));
					m_up->setPos(glm::vec2());
					m_up->setSize(glm::vec2(m_size.y, m_size.y));
					m_bar->setPos(glm::vec2(getValue() * (m_size.x - m_size.y * 3) + m_size.y, 0));
					m_bar->setSize(glm::vec2(m_size.y, m_size.y));
				}

				validateGroup();
			}

			void snapValue()
			{
				m_value = round(m_value / m_inc) / (1 / m_inc);
			}
			void setValue(float amount)
			{
				m_value = amount;
				m_value = min(m_value, 1);
				m_value = max(m_value, 0);
				char number[24];
				sprintf(number, "%.1f", m_value);
			}
			float getValue()
			{
				return m_value;
			}

			void onDown(gfx::engine::GLContent * content)
			{
				if (m_down->isPressed(content) || m_down->isHeldOver(content))
				{
					m_value += m_inc;
					m_value = min(m_value, 1);
					setValue(m_value);
					validate();
					callTrigger(&GFXScrollBar::onDown);
				}
			}
			void onUp(gfx::engine::GLContent * content)
			{
				if (m_up->isPressed(content) || m_up->isHeldOver(content))
				{
					m_value -= m_inc;
					m_value = max(m_value, 0);
					setValue(m_value);
					validate();
					callTrigger(&GFXScrollBar::onUp);
				}
			}
			void onBarMove(gfx::engine::GLContent * content)
			{
				if (m_bar->isDragging(content))
				{
					validate();
					callTrigger(&GFXScrollBar::onBarMove);
				}
			}

			GFXScrollBar(glm::vec2 pos, glm::vec2 size, float value, float inc, bool isVertical)
			{
				m_pos = pos;
				m_size = size;
				m_value = value;
				m_inc = inc;
				m_isVertical = isVertical;
			}
		protected:
			float m_value;
			float m_inc;

			bool m_isVertical = false;

			GFXButton * m_down;
			GFXButton * m_up;
			GFXButton * m_bar;
		};
	}
}