#pragma once

#include "Logger.h"
#include "glm.h"
#include "GVertexBufferObject.h"
#include "GPrimativeFactory.h"
#include "GShaderProgram.h"
#include "GShaderVariableHandle.h"
#include "GContent.h"
#include "colors.h"
#include "StringFormat.h"
#include "GLinker.h"

using glw::engine::GContent;
using glw::engine::buffers::GVertexBufferObject;
using glw::engine::buffers::GPrimativeFactory;
using glw::engine::buffers::GArrayVertex;
using glw::engine::buffers::GArrayVec2;
using glw::engine::buffers::GArrayVec3;
using glw::engine::glsl::GShaderProgram;
using glw::engine::glsl::GShaderVariableHandleId;

using util::StringFormat;

using glw::meta::GLinker;

#define GNULLPTR NULL
#define GNULL_ID ""
#define GDEFAULT_ID "component"
#define GNULL_INDEX -1
#define GNULL_COLORSTYLE NULL
#define GRESIZE_NULL 0
#define GRESIZE_LEFT 1
#define GRESIZE_TOP 2
#define GRESIZE_RIGHT 3
#define GRESIZE_BOTTOM 4

#define GGUI_SHADER_BLOCK 0
#define GGUI_SHADER_FONT 1
#define GGUI_SHADER_TEXTURE 2
#define GGUI_DEFAULT_FONT_SIZE 20
#define GGUI_DEFAULT_PADDING 50


namespace glw
{
  namespace gui
  {
    namespace
    {
      const char * TRG = "GGUI";
      const char * GUI_SHADER_VERT = "../shaders/gui.vert";
      const char * GUI_SHADER_FRAG = "../shaders/gui.frag";
      const char * GUI_DEFAULT_FONT = "../textures/default_font.png";
    }


    class GGUI
    {
    public:
      
      GGUI();
      
      ~GGUI();
      
    private:
      
    };

    typedef std::string GUnitID;



    struct GContextShaderHandle_T
    {
      GShaderVariableHandle * colorHandle;
      GShaderVariableHandle * isFontHandle;
      GShaderVariableHandle * istextureHandle;
    };

    class GContextShaderProgram : public GShaderProgram
    {
    public:
      GContextShaderProgram() {}

      GContextShaderProgram(glm::mat4 * model_data,
                            glm::mat4 * view_data,
                            glm::mat4 * proj_data) :
        GShaderProgram(GUI_SHADER_VERT, GUI_SHADER_FRAG)
      {
        setModelMat4Handle(model_data);
        setViewMat4Handle(view_data);
        setProjMat4Handle(proj_data);
        setTexHandle();
        addHandle(GShaderVariableHandle("u_color"), m_color);
        addHandle(GShaderVariableHandle("u_is_font"), m_isFont);
        addHandle(GShaderVariableHandle("u_is_texture"), m_isTexture);
      }

      GShaderVariableHandle * getColorHandle()
      {
        return getHandle(m_color);
      }

      GShaderVariableHandle * getIsFontHandle()
      {
        return getHandle(m_isFont);
      }

      GShaderVariableHandle * getIsTextureHandle()
      {
        return getHandle(m_isTexture);
      }

      GContextShaderHandle_T getContextHandle()
      {
        return { getHandle(m_color), getHandle(m_isFont), getHandle(m_isTexture)};
      }

    private:
      GShaderVariableHandleId m_color, m_isFont, m_isTexture;

    };

    class GContext;

    class GUnit
    {
    public:
      GUnit()
      {
        m_id = GDEFAULT_ID;
        m_context = GNULLPTR;
      }
      GUnit(GUnitID id, GContext * context)
      {
        m_id = id;
        m_context = context;
      }
      GUnit(GUnit * component)
      {
        m_id = component->m_id;
        m_context = component->m_context;
      }

      GContext * getContext()
      {
        return m_context;
      }
      void setContext(GContext * context)
      {
        m_context = context;
      }

      GUnitID getId()
      {
        return m_id;
      }
      void setId(GUnitID id)
      {
        m_id = id;
      }

    private:
      GContext * m_context;
      GUnitID m_id;
    };

    class IGComponent
    {
    public:
      IGComponent() {}
      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle) = 0;
      virtual bool isEnabled() = 0;
      virtual bool isVisible() = 0;
      virtual bool checkMouseEvents(int button, int action) = 0;
      virtual bool checkKeyEvents(int key, int action) = 0;
      virtual void init(GContext * context, IGComponent * parent) = 0;
    };

    class GGroup
    {
    public:

      void addComponent(IGComponent * component)
      {
        m_group.push_back(component);
      }

      void drawGroup(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        for (IGComponent * component : m_group)
        {
          if (component->isVisible())
          {
            component->draw(parentMatrix, shaderHandle, contextHandle);
          }
        }
      }

      bool checkGroupMouseEvents(int button, int action)
      {
        std::vector<IGComponent*>::reverse_iterator itor = m_group.rbegin();

        bool eventHappenedInChild = false;

        while(itor != m_group.rend() && !eventHappenedInChild)
        {
          IGComponent * component = *itor;
          if (component->isVisible())
          {
            if (component->isEnabled())
            {
              eventHappenedInChild = component->checkMouseEvents(button, action);
            }
          }
          ++itor;
        }

        return eventHappenedInChild;
      }

      bool checkGroupKeyEvents(int key, int action)
      {
        std::vector<IGComponent*>::reverse_iterator itor = m_group.rbegin();

        bool eventHappenedInChild = false;

        while(itor != m_group.rend() && !eventHappenedInChild)
        {
          IGComponent * component = *itor;
          if (component->isVisible())
          {
            if (component->isEnabled())
            {
              eventHappenedInChild = component->checkKeyEvents(key, action);
            }
          }
          ++itor;
        }

        return eventHappenedInChild;
      }

      void initGroup(GContext * context, IGComponent * parent)
      {
        for (IGComponent * component : m_group)
        {
          component->init(context, parent);
        }
      }

    private:
      std::vector<IGComponent*> m_group;
    };

    class GContext : public GGroup
    {
    public:

      GContext() {}

      GContext(GContent * content):
        m_content(content)
      {
      }

      GContent * getContent()
      {
        return m_content;
      }
      void setContent(GContent * content)
      {
        m_content = content;
      }

      void initShaderProgram(glm::mat4 * model_data,
                             glm::mat4 * view_data,
                             glm::mat4 * proj_data)
      {
        m_shaderProgram = GContextShaderProgram(model_data,view_data,proj_data);
      }

      void draw()
      {
        if(isValid())
        {
          m_shaderProgram.load();
          drawGroup(glm::mat4(1), m_shaderProgram.getShaderHandle(), m_shaderProgram.getContextHandle());
        }
        else
        {
          LERROR(TRG, "GContext is invalid.", __FILE__, __LINE__, "GContext", __func__);
        }
      }

      void init()
      {
        initGroup(this, GNULLPTR);
        m_isValid = true;
      }

      bool isValid()
      {
        return m_isValid && m_shaderProgram.isValid();
      }

      void checkMouseEvents(int button, int action)
      {
        if(isValid())
        {
          checkGroupMouseEvents(button, action);
        }
        else
        {
          LERROR(TRG, "GContext is invalid.", __FILE__, __LINE__, "GContext", __func__);
        }
      }

      void checkKeyEvents(int key, int action)
      {
        if(isValid())
        {
          checkGroupKeyEvents(key, action);
        }
        else
        {
          LERROR(TRG, "GContext is invalid.", __FILE__, __LINE__, "GContext", __func__);
        }
      }

    private:
      GContent * m_content;
      GContextShaderProgram m_shaderProgram;

      bool m_isValid = false;
    };

    class GComponent : public IGComponent, public GUnit
    {
    public:
      GComponent() {}

      GComponent(glm::vec2 pos, glm::vec2 size) :
        m_pos(pos),
        m_size(size)
      {
      }

      virtual bool isEnabled()
      {
        return m_enabled;
      }
      virtual bool isVisible()
      {
        return m_visible;
      }

      void setEnabled(const bool enabled)
      {
        m_enabled = enabled;
      }
      void setVisible(const bool visible)
      {
        m_visible = visible;
      }

      glm::vec2 getRelativeMousePos()
      {
        glm::vec2 mousePos = getContext()->getContent()->getMouse()->getMousePosition();
        GComponent * parent = m_parent;
        while (parent != GNULLPTR)
        {
          mousePos -= parent->m_pos;
          parent = parent->getParent();
        }
        mousePos -= m_pos;
        return mousePos;
      }

      bool isMouseInside()
      {
        glm::vec2 mousePos = getRelativeMousePos();
        return mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x <= m_size.x && mousePos.y <= m_size.y;
      }

      GComponent * getParent()
      {
          return m_parent;
      }
      void setParent(GComponent * parent)
      {
          m_parent = parent;
      }
      void setParent(IGComponent * parent)
      {
          m_parent = static_cast<GComponent*>(parent);
      }

    protected:
      glm::vec2 m_pos;
      glm::vec2 m_size;
    private:
      GComponent * m_parent;

      bool m_enabled = true;
      bool m_visible = true;

    };

    class GShape : public GVertexBufferObject
    {
    public:
      GShape() {}

      GShape(GArrayVertex data,
             glm::vec2 pos,
             glm::vec2 size,
             glm::vec4 color) :
        GVertexBufferObject(data,
                            glm::vec3(),
                            glm::vec3(0,0,1),0.0f,
                            glm::vec3(0,0,1),0.0f)
      {
        m_pos = glm::vec3(pos, 0);
        m_scale = glm::vec3(size, 0);
        m_color = color;
      }

      void setPos(glm::vec2 pos)
      {
        m_pos = glm::vec3(pos, 0);
      }

      glm::vec2 getPos()
      {
        return glm::vec2(m_pos.x, m_pos.y);
      }

      void setSize(glm::vec2 size)
      {
        m_scale = glm::vec3(size, 0);
      }

      glm::vec2 getSize()
      {
        return glm::vec2(m_scale.x, m_scale.y);
      }

      glm::vec4 getColor()
      {
        return m_color;
      }

      bool isFont()
      {
        return m_isFont;
      }

      bool isTexture()
      {
        return m_isTexture;
      }

      void setColor(glm::vec4 color)
      {
        m_color = color;
      }

      void isFont(bool isFont)
      {
        m_isFont = isFont;
      }

      void isTexture(bool isTexture)
      {
        m_isTexture = isTexture;
      }

      void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        if(NULL != contextHandle.colorHandle)
        {
          contextHandle.colorHandle->load(m_color);
        }
        if (NULL != contextHandle.isFontHandle)
        {
          contextHandle.isFontHandle->load(false);
        }
        if (NULL != contextHandle.istextureHandle)
        {
          contextHandle.istextureHandle->load(false);
        }
        GVertexBufferObject::draw(parentMatrix, shaderHandle);
      }
    private:
      glm::vec4 m_color;
      bool m_isFont;
      bool m_isTexture;
    };

    class GFont : public GVertexBufferObject
    {
    public:
      GFont() {}

      GFont(GArrayVertex data, const char *fontfile) :
        GVertexBufferObject(data,
                            glm::vec3(),
                            glm::vec3(0,0,1), 0.0f,
                            glm::vec3(0,0,1), 0.0f,
                            glm::vec3(1),
                            fontfile),
        m_color(glw::BLACK_A)
      {
      }

      glm::vec4 getColor()
      {
        return m_color;
      }
      void setColor(glm::vec4 color)
      {
        m_color = color;
      }

      void draw(unsigned char c, glm::mat4 parentMatrix, glm::vec2 pos, glm::vec2 scale, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        if(NULL != 	shaderHandle.modelMatHandle)
        {
          shaderHandle.modelMatHandle->load(parentMatrix *
                                            glm::translate(glm::mat4(1.), glm::vec3(pos, 0)) *
                                            glm::scale(glm::mat4(1.), glm::vec3(scale, 0)));
        }
        if(NULL != shaderHandle.textureHandle)
        {
          shaderHandle.textureHandle->load(m_tex);
        }
        if(NULL != contextHandle.colorHandle)
        {
          contextHandle.colorHandle->load(m_color);
        }
        if (NULL != contextHandle.isFontHandle)
        {
          contextHandle.isFontHandle->load(1);
        }
        if (NULL != contextHandle.istextureHandle)
        {
          contextHandle.istextureHandle->load(0);
        }
        drawArray(c * 6, 6);
      }
    private:
      glm::vec4 m_color;
    };

    class GLabel : public GComponent
    {
    public:

      GLabel() {}

      GLabel(GFont font, std::string text, glm::vec2 pos, float height, glm::vec4 color) :
        GComponent(pos, glm::vec2(height)),
        m_font(font),
        m_text(text)
      {
        setColor(color);
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        glm::vec2 pos = m_pos -
                        glm::vec2(getLength() / 2, m_size.y / 2) *
                        glm::vec2(m_centerHorizontal, m_centerVertical);

        for (int ix = 0; ix < m_text.length(); ix++)
        {
          m_font.draw(m_text[ix],
                      parentMatrix,
                      glm::vec2(ix * m_size.x / 2, 0) + pos,
                      m_size,
                      shaderHandle,
                      contextHandle);
        }
      }

      virtual bool checkMouseEvents(int button, int action) { return false; }
      virtual bool checkKeyEvents(int key, int action) { return false; }

      virtual void init(GContext * context, IGComponent * parent)
      {
        setContext(context);
        setParent(parent);
        setId("label");
      }

      float getLength()
      {
        return m_size.x / 2 * m_text.size();
      }

      void centerBoth(bool center)
      {
        m_centerHorizontal = m_centerVertical = center;
      }

      void centerHorizontal(bool center)
      {
        m_centerHorizontal = center;
      }

      void centerVertical(bool center)
      {
        m_centerVertical = center;
      }

      void setText(std::string text)
      {
        m_text = text;
      }

      void setColor(glm::vec4 color)
      {
        m_font.setColor(color);
      }
    private:
      GFont m_font;
      std::string m_text;
      bool m_centerHorizontal = false;
      bool m_centerVertical = false;
    };

    static GShape createBox(glm::vec2 pos, glm::vec2 size, float weight, glm::vec4 color)
    {
      GArrayVec2 uv;
      GArrayVec3 v, c, n, t;

      GPrimativeFactory::squareBorder(v, weight, size.x/size.y);

      GPrimativeFactory::colourBuffer(c, color, v.size());

      GArrayVertex o;
      GPrimativeFactory::packObject(o, v, c, n, t, uv);
      GShape shape(o, pos, size, color);
      return shape;
    }
    static GShape createRectangle(glm::vec2 pos, glm::vec2 size, glm::vec4 color)
    {
      GArrayVec2 uv;
      GArrayVec3 v, c, n, t;

      GPrimativeFactory::squareMesh(v, 1, 1);

      GPrimativeFactory::colourBuffer(c, color, v.size());

      GArrayVertex o;
      GPrimativeFactory::packObject(o, v, c, n, t, uv);
      GShape shape(o, pos, size, color);
      return shape;
    }
    static GShape createCircle(glm::vec2 pos, glm::vec2 size, glm::vec4 color)
    {
      GArrayVec2 uv;
      GArrayVec3 v, c, n, t;

      GPrimativeFactory::circle(v, 32);

      GPrimativeFactory::colourBuffer(c, color, v.size());

      GArrayVertex o;
      GPrimativeFactory::packObject(o, v, c, n, t, uv);
      GShape shape(o, pos, size, color);
      return shape;
    }
    static GFont createFont()
    {
      GArrayVec2 uv;
      GArrayVec3 v, c, n, t;

      GPrimativeFactory::squareMeshes(v, 256);

      GPrimativeFactory::squareMeshUVs(uv, 16, 16);

      GArrayVertex o;
      GPrimativeFactory::packObject(o, v, c, n, t, uv);
      GFont font(o, GUI_DEFAULT_FONT);
      return font;
    }
    static GLabel * createLabel(std::string text, glm::vec2 pos, float height, glm::vec4 color, bool centered = false)
    {
      GLabel * label = new GLabel(createFont(), text, pos, height, color);
      label->centerBoth(centered);
      return label;
    }

    class GClickable : public GComponent
    {
    public:

      GClickable() {}

      GClickable(glm::vec2 pos, glm::vec2 size, bool isTogglable = false) :
        GComponent(pos, size),
        m_isTogglable(isTogglable)
      {
      }

      bool onClickablePressed(bool isTrue)
      {
        if (isTrue)
        {
          m_onPressed = true;
          m_onDown = true;
//          getContext()->setFocused(this);
        }
        else
        {
          m_onPressed = false;
        }
        return isTrue;
      }
      bool onClickableDown(bool isTrue)
      {
        if (isTrue)
        {
          m_heldCounter++;
          m_onDown = true;
        }
        return isTrue;
      }
      bool onClickableReleased(bool isTrue)
      {
        if (isTrue)
        {
          m_heldCounter = 0;
          m_onDown = false;
          m_onReleased = true;
          m_onPressed = false;
          if(isHovering())
          {
            toggle();
          }
        }
        else
        {
          m_onReleased = false;
        }
        return isTrue;
      }

      bool isPressed()
      {
        return m_onPressed;
      }
      bool isDown()
      {
        return m_onDown;
      }
      bool isHeld()
      {
        return m_heldCounter > m_heldThreshold;
      }
      bool isReleased()
      {
        return m_onReleased;
      }
      bool isDragging()
      {
        return glm::length(getContext()->getContent()->getMouse()->getMouseDelta()) > 0 && m_onDown;
      }
      bool isHovering()
      {
        return isMouseInside();
      }
      bool isReleasedOver()
      {
        return isReleased() && isHovering();
      }
      bool isHeldOver()
      {
        return isHeld() && isHovering();
      }

      virtual bool checkMouseEvents(int key, int action)
      {
        bool eventHasHappened = false;

        if (isHovering())
        {
          eventHasHappened |= onClickablePressed(key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !m_onDown);
          eventHasHappened |= onClickableDown(key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && m_onDown);
        }
        eventHasHappened |= onClickableReleased(key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && m_onDown);

        return eventHasHappened;
      }

      void toggle()
      {
        if (m_isTogglable)
        {
          m_toggleState = !m_toggleState;
        }
      }

      bool isToggled()
      {
        return m_toggleState;
      }

      void setIsTogglable(bool isTogglable)
      {
        m_isTogglable = isTogglable;
      }

    protected:

      bool m_onDown = false;
      bool m_onReleased = false;
      bool m_onPressed = false;

      bool m_isTogglable = false;
      bool m_toggleState = false;

      int m_heldCounter = 0;
      int m_heldThreshold = 25;
    };

    class GButton : public GClickable, public GLinker<GButton>
    {
    public:
      GButton() {}

      GButton(glm::vec2 pos, glm::vec2 size, std::string text, bool isTogglable = false):
        GClickable(pos, size, isTogglable), m_text(text)
      {
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        glm::vec4 oldColor = m_back.getColor();
        glm::vec4 color = oldColor;

        if (isToggled())
        {
          color /= 2.0f;
        }
        if (isDown())
        {
          color /= 1.5f;
        }

        m_back.setColor(color);

        m_back.draw(parentMatrix, shaderHandle, contextHandle);
        m_label->draw(parentMatrix, shaderHandle, contextHandle);

        m_back.setColor(oldColor);
      }

      virtual bool checkMouseEvents(int button, int action)
      {
        bool eventHasHappened = false;

        eventHasHappened |= GClickable::checkMouseEvents(button, action);

        onPressed();
        onDown();
        onReleased();
        onToggledOff();
        onToggledOn();

        return eventHasHappened;

      }

      virtual bool checkKeyEvents(int key, int action)
      {
        return false;
      }

      virtual void init(GContext *context, IGComponent *parent)
      {
        setContext(context);
        setParent(parent);
        setId("button");

        glm::vec2 pos = m_pos;
        m_back = createRectangle(m_pos, m_size, glw::BLUE_A);
        pos.x += m_size.x / 2;
        m_label = createLabel(m_text, pos, m_size.y, glw::WHITE_A);
        m_label->centerHorizontal(true);
      }

      triggers:

      void onPressed()
      {
        if (isPressed())
        {
          callTrigger(&GButton::onPressed);
        }
      }

      void onDown()
      {
        if (isDown())
        {
          callTrigger(&GButton::onDown);
        }
      }

      void onReleased()
      {
        if (isReleased())
        {
          callTrigger(&GButton::onReleased);
        }
      }

      void onToggledOn()
      {
        if (isToggled())
        {
          callTrigger(&GButton::onToggledOn);
        }
      }

      void onToggledOff()
      {
        if (!isToggled())
        {
          callTrigger(&GButton::onToggledOff);
        }
      }

    private:
      std::string m_text;
      GShape m_back;
      GLabel * m_label;
    };




  }
}
