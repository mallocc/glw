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
#include <algorithm>

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
#define GGUI_WINDOW_TITLBAR_HEIGHT 25
#define GGUI_WINDOW_DEADZONE 10


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

    struct GColorStyle
    {
      glm::vec4 foreground;
      glm::vec4 background;
      glm::vec4 accent;
    };

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
      ~IGComponent() {}
      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle) = 0;
      virtual bool isEnabled() = 0;
      virtual bool isVisible() = 0;
      virtual bool checkMouseEvents(int button, int action) = 0;
      virtual bool checkKeyEvents(int key, int action) = 0;
      virtual void init(GContext * context, IGComponent * parent) = 0;
      virtual void validate() = 0;
      virtual glm::vec2 getPos() = 0;
      virtual glm::vec2 getSize() = 0;
      virtual void update() = 0;
    };

    class GGroup
    {
    public:

      ~GGroup()
      {
        for (IGComponent* obj : m_group)
        {
          delete obj;
        }
        m_group.clear();
      }

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

      void validateGroup()
      {
        for (IGComponent * component : m_group)
        {
          component->validate();
        }
      }

      void updateGroup()
      {
        for (IGComponent * component : m_group)
        {
          component->update();
        }
      }

      void bringToFront(IGComponent * component)
      {
        for (int i = 0; i < m_group.size(); ++i)
        {
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
      }

      int getComponent(IGComponent * component)
      {
        for (int ix = 0; ix < m_group.size(); ++ix)
          if (m_group[ix] == component)
            return ix;
        return GNULL_INDEX;
      }
      bool removeComponent(IGComponent * component)
      {
        int id = getComponent(component);
        if (id != GNULL_INDEX)
        {
          IGComponent * component = m_group[id];
          m_group.erase(m_group.begin() + id);
          component->~IGComponent();
          delete(component);
          return true;
        }

        bool success = false;
        for (IGComponent * c : m_group)
          success |= dynamic_cast<GGroup*>(c)->removeComponent(component);
        return success;
      }

      glm::vec2 getMinimumBounds(float padding)
      {
        float left = 1000000;
        float right = 0.0f;
        float top = 0.0f;
        float bottom = 1000000;
        for (IGComponent * component : m_group)
        {
          glm::vec2 size = component->getSize();
          glm::vec2 pos = component->getPos();
          left = std::min(pos.x, left);
          right = std::max(pos.x + size.x, right);
          bottom = std::min(pos.y, bottom);
          top = std::max(pos.y + size.y, top);
        }
        return glm::vec2(right + padding, top + padding);
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

      void validate()
      {
        if (isValid())
        {
          validateGroup();
        }
      }

      void update()
      {
        if (isValid())
        {
          updateGroup();
        }
      }

      bool isValid()
      {
        return m_isValid && m_shaderProgram.isValid();
      }

      void checkMouseEvents(int button, int action)
      {
        if(isValid())
        {
          if (checkGroupMouseEvents(button, action))
          {
            validate();
          }
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
          if (checkGroupKeyEvents(key, action))
          {
            validate();
          }
        }
        else
        {
          LERROR(TRG, "GContext is invalid.", __FILE__, __LINE__, "GContext", __func__);
        }
      }

      void setColorStyle(GColorStyle colorStyle)
      {
        m_colorStyle = colorStyle;
      }
      GColorStyle getColorStyle()
      {
        return m_colorStyle;
      }

      void setFocused(IGComponent * component)
      {
        m_focused = component;
      }
      bool isFocused(IGComponent * component)
      {
        return component == m_focused;
      }


    private:
      GContent * m_content;
      GContextShaderProgram m_shaderProgram;

      GColorStyle m_colorStyle = {glw::WHITE_A, glw::BLACK_A, glw::GREY_A};

      bool m_isValid = false;

      IGComponent * m_focused = GNULLPTR;
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

      void setPos(const glm::vec2 pos)
      {
        m_pos = pos;
      }
      glm::vec2 getPos()
      {
        return m_pos;
      }

      void setSize(const glm::vec2 size)
      {
        m_size = size;
      }
      glm::vec2 getSize()
      {
        return m_size;
      }

      glm::mat4 getRelativeModelMatrix()
      {
        return glm::translate(glm::mat4(1), glm::vec3(m_pos, 0.0f));
      }

      void setColorStyle(GColorStyle colorStyle)
      {
        m_colorStyle = colorStyle;
      }
      GColorStyle getColorStyle()
      {
        return m_colorStyle;
      }

      void inheritColorStyle()
      {
        if (m_parent != GNULLPTR)
        {
          setColorStyle(m_parent->getColorStyle());
        }
        else if (getContext() != GNULLPTR)
        {
          setColorStyle(getContext()->getColorStyle());
        }
      }

    protected:
      glm::vec2 m_pos;
      glm::vec2 m_size;
    private:
      GComponent * m_parent;

      bool m_enabled = true;
      bool m_visible = true;

      GColorStyle m_colorStyle;
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
        inheritColorStyle();
      }

      virtual void validate() {}
      virtual void update() {}

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

      void setHeight(float height)
      {
        setSize(glm::vec2(height));
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
          getContext()->setFocused(this);
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

      GButton(glm::vec2 pos, glm::vec2 size, std::string text = "", bool isTogglable = false):
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
        inheritColorStyle();

        glm::vec2 pos = m_pos;
        m_back = createRectangle(getPos(), m_size, getColorStyle().accent);
        pos.x += m_size.x / 2;
        m_label = createLabel(m_text, pos, m_size.y, getColorStyle().foreground);
        m_label->centerHorizontal(true);
      }

      virtual void validate()
      {
        m_back.setPos(getPos());
        m_back.setSize(getSize());
        m_back.setColor(getColorStyle().accent);
        m_label->setPos(getPos() + glm::vec2(getSize().x / 2, 0));
        m_label->setColor(getColorStyle().foreground);
        m_label->setHeight(getSize().y);
      }

      virtual void update()
      {
        m_label->update();
      }

      void setColor(glm::vec4 color)
      {
        m_back.setColor(color);
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

    class GWindow : public GGroup, public GClickable, public GLinker<GWindow>
    {
    public:

      GWindow()
        : m_title("Untitled")
      {}

      GWindow(glm::vec2 pos, glm::vec2 size, std::string title)
        : GClickable(pos, size),
          m_title(title)
      {
      }

      ~GWindow() {}

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        m_back.draw(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
        drawGroup(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
        m_children.drawGroup(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
      }
      virtual bool checkMouseEvents(int button, int action)
      {
        bool eventHasHappened = false;

        eventHasHappened |= checkGroupMouseEvents(button, action);

        if (!eventHasHappened)
        {
          eventHasHappened |= m_children.checkGroupMouseEvents(button, action);
        }
        if (!eventHasHappened)
        {
          eventHasHappened |= GClickable::checkMouseEvents(button, action);
        }

        onClose();
        onScale();
        onWindowMove();
        onResize();

        if (eventHasHappened)
        {
          focus();
        }

        return eventHasHappened;
      }
      virtual bool checkKeyEvents(int key, int action)
      {
        bool eventHasHappened = false;

        eventHasHappened |= m_children.checkGroupKeyEvents(key, action);

        return eventHasHappened;
      }
      virtual void init(GContext * context, IGComponent * parent)
      {
        // Inherit properties
        setContext(context);
        setParent(parent);
        setId("window");
        inheritColorStyle();

        // Create background of the window
        m_back = createRectangle(glm::vec2(), getSize(), getColorStyle().background/2.0f);

        // Create the resize bars
        m_leftResizeBar = new GButton(glm::vec2(), glm::vec2(GGUI_WINDOW_DEADZONE, getSize().y));
        addComponent(m_leftResizeBar);
        m_rightResizeBar = new GButton(glm::vec2(getSize().x - GGUI_WINDOW_DEADZONE, 0), glm::vec2(GGUI_WINDOW_DEADZONE, getSize().y));
        addComponent(m_rightResizeBar);
        m_bottomResizeBar = new GButton(glm::vec2(0, getSize().y - GGUI_WINDOW_DEADZONE), glm::vec2(getSize().x, GGUI_WINDOW_DEADZONE));
        addComponent(m_bottomResizeBar);
        m_bottomLeftResizeBar = new GButton(glm::vec2(0, getSize().y - GGUI_WINDOW_DEADZONE), glm::vec2(GGUI_WINDOW_DEADZONE));
        addComponent(m_bottomLeftResizeBar);
        m_bottomRightResizeBar = new GButton(getSize() - glm::vec2(GGUI_WINDOW_DEADZONE), glm::vec2(GGUI_WINDOW_DEADZONE));
        addComponent(m_bottomRightResizeBar);

        // Create title bar
        m_titleBar = new GButton(glm::vec2(), glm::vec2(getSize().x - GGUI_WINDOW_TITLBAR_HEIGHT * 2, GGUI_WINDOW_TITLBAR_HEIGHT), m_title);
        addComponent(m_titleBar);

        // Create close windows buttons
        m_close = new GButton(glm::vec2(getSize().x - GGUI_WINDOW_TITLBAR_HEIGHT, 0), glm::vec2(GGUI_WINDOW_TITLBAR_HEIGHT), "X");
        addComponent(m_close);
        m_maxmin = new GButton(glm::vec2(getSize().x - GGUI_WINDOW_TITLBAR_HEIGHT * 2, 0), glm::vec2(GGUI_WINDOW_TITLBAR_HEIGHT), "[]");
        addComponent(m_maxmin);

        // Init window components and its children components
        initGroup(context, this);
        m_children.initGroup(context, this);

        // Override colorstyles
        GColorStyle colorStyle = getColorStyle();

        colorStyle.accent = colorStyle.background / 2.0f;
        m_leftResizeBar->setColorStyle(colorStyle);
        m_rightResizeBar->setColorStyle(colorStyle);
        m_bottomResizeBar->setColorStyle(colorStyle);
        m_bottomLeftResizeBar->setColorStyle(colorStyle);
        m_bottomRightResizeBar->setColorStyle(colorStyle);

        colorStyle.accent = glw::RED_A;
        m_close->setColorStyle(colorStyle);

        colorStyle.accent = glw::ORANGE_A;
        m_maxmin->setColorStyle(colorStyle);
      }
      virtual void validate()
      {
        m_minSize = m_children.getMinimumBounds(GGUI_DEFAULT_PADDING);
        inflateToContent();

        m_back.setPos(glm::vec2());
        m_back.setSize(getSize());
        m_back.setColor(getColorStyle().background / 2.0f);

        m_titleBar->setPos(glm::vec2());
        m_titleBar->setSize(glm::vec2(getSize().x - GGUI_WINDOW_TITLBAR_HEIGHT * 2, GGUI_WINDOW_TITLBAR_HEIGHT));

        m_leftResizeBar->setPos(glm::vec2());
        m_leftResizeBar->setSize(glm::vec2(GGUI_WINDOW_DEADZONE, getSize().y));

        m_rightResizeBar->setPos(glm::vec2(getSize().x - GGUI_WINDOW_DEADZONE, 0));
        m_rightResizeBar->setSize(glm::vec2(GGUI_WINDOW_DEADZONE, getSize().y));

        m_bottomResizeBar->setPos(glm::vec2(0, getSize().y - GGUI_WINDOW_DEADZONE));
        m_bottomResizeBar->setSize(glm::vec2(getSize().x, GGUI_WINDOW_DEADZONE));

        m_bottomLeftResizeBar->setPos(glm::vec2(0, getSize().y - GGUI_WINDOW_DEADZONE));
        m_bottomLeftResizeBar->setSize(glm::vec2(GGUI_WINDOW_DEADZONE));

        m_bottomRightResizeBar->setPos(getSize() - glm::vec2(GGUI_WINDOW_DEADZONE));
        m_bottomRightResizeBar->setSize(glm::vec2(GGUI_WINDOW_DEADZONE));

        m_close->setPos(glm::vec2(getSize().x - GGUI_WINDOW_TITLBAR_HEIGHT, 0));
        m_close->setSize(glm::vec2(GGUI_WINDOW_TITLBAR_HEIGHT));

        m_maxmin->setPos(glm::vec2(getSize().x - GGUI_WINDOW_TITLBAR_HEIGHT * 2, 0));
        m_maxmin->setSize(glm::vec2(GGUI_WINDOW_TITLBAR_HEIGHT));

        validateGroup();
        m_children.validateGroup();
      }
      virtual void update()
      {
        glm::vec2 delta = getContext()->getContent()->getMouse()->getMouseDelta();

        if (m_titleBar->isDragging())
        {
          m_pos += delta;
          confinePositionToWindow();
          validate();
        }

        glm::vec2 pos = m_pos;
        glm::vec2 size = m_size;

        // if the window wants to be resized and isnt maximised
        if (m_isResizable && !m_maximised)
        {
          // horizontal grab bars resize
          if (m_isResizableX)
            if (m_leftResizeBar->isDragging())
            {
              pos.x += delta.x;
              size.x -= delta.x;
              if (!isSmallerThanMin(size, m_minSize))
              {
                m_pos = pos;
                m_size = size;
                confineSizeToContent();
                confinePositionToWindow();
                validate();
              }
            }
            else if (m_rightResizeBar->isDragging())
            {
              size.x += delta.x;
              if (!isSmallerThanMin(size, m_minSize))
              {
                m_size = size;
                confineSizeToContent();
                confinePositionToWindow();
                validate();
              }
            }
          // vetical grab bars resize
          if (m_isResizableY)
            if (m_bottomResizeBar->isDragging())
            {
              size.y += delta.y;
              if (!isSmallerThanMin(size, m_minSize))
              {
                m_pos = pos;
                m_size = size;
                confineSizeToContent();
                confinePositionToWindow();
                validate();
              }
            }
          // corner grabs resize
          if (m_isResizableX && m_isResizableY)
            if (m_bottomLeftResizeBar->isDragging())
            {
              pos.x += delta.x;
              size.x -= delta.x;
              size.y += delta.y;
              if (!isSmallerThanMin(size, m_minSize))
              {
                m_pos = pos;
                m_size = size;
                confineSizeToContent();
                confinePositionToWindow();
                validate();
              }
            }
            else if (m_bottomRightResizeBar->isDragging())
            {
              size.x += delta.x;
              size.y += delta.y;
              if (!isSmallerThanMin(size, m_minSize))
              {
                m_pos = pos;
                m_size = size;
                confineSizeToContent();
                confinePositionToWindow();
                validate();
              }
            }
        } // if m_resizeable

        updateGroup();
        m_children.updateGroup();
      }

      void focus()
      {
        IGComponent * component = getParent();
        if (component != GNULLPTR)
        {
          dynamic_cast<GGroup*>(component)->bringToFront(this);
        }
      }

      void addChildComponent(IGComponent * component)
      {
        m_children.addComponent(component);
      }

      void closeWindow()
      {
        if (getContext() != GNULLPTR)
        {
          getContext()->removeComponent(this);
        }
      }

      void maximiseWindow()
      {
        if (getContext() != GNULLPTR)
        {
          if (getContext()->getContent() != GNULLPTR)
          {
            m_oldPos = m_pos;
            m_oldSize = m_size;

            m_pos = glm::vec2();
            getContext()->getContent()->getWindowSize(m_size);

            validate();
          }
        }
      }
      void minimiseWindow()
      {
        m_pos = m_oldPos;
        m_size = m_oldSize;

        validate();
      }
      void toggleMaximise()
      {
        m_maximised = !m_maximised;
        if (m_maximised)
          maximiseWindow();
        else
          minimiseWindow();
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

      triggers:

      void onClose()
      {
        if (m_close->isReleasedOver())
        {
          callTrigger(&GWindow::onClose);
          closeWindow();
        }
      }

      void onScale()
      {
        if (m_maxmin->isReleasedOver())
        {
          toggleMaximise();
          callTrigger(&GWindow::onScale);
        }
      }

      void onWindowMove()
      {
        if (m_titleBar->isDragging())
        {
          callTrigger(&GWindow::onWindowMove);
        }
      }

      void onResize()
      {
        if(m_isResizable && !m_maximised)
        {
          if (m_leftResizeBar->isDragging() || m_rightResizeBar->isDragging() || m_bottomResizeBar->isDragging())
          {
            callTrigger(&GWindow::onResize);
          }
        }
      }

    private:
      GShape m_back;
      GButton * m_titleBar;
      GButton * m_leftResizeBar;
      GButton * m_rightResizeBar;
      GButton * m_bottomResizeBar;
      GButton * m_bottomLeftResizeBar;
      GButton * m_bottomRightResizeBar;
      GButton * m_close;
      GButton * m_maxmin;
      std::string m_title;
      GGroup m_children;

      glm::vec2 m_oldPos;
      glm::vec2 m_oldSize;
      glm::vec2 m_minSize;

      bool m_maximised = false;

      bool m_isResizable = true;
      bool m_isResizableX = m_isResizable;
      bool m_isResizableY = m_isResizable;

      void confinePositionToWindow()
      {
        glm::vec2 windowSize;
        getContext()->getContent()->getWindowSize(windowSize);

        m_pos.x = std::min(windowSize.x - m_size.x, m_pos.x);
        m_pos.x = std::max(0.0f, m_pos.x);
        m_pos.y = std::min(windowSize.y - m_size.y, m_pos.y);
        m_pos.y = std::max(0.0f, m_pos.y);
      }

      void inflateToContent()
      {
        m_size.x = std::max(m_minSize.x, m_size.x);
        m_size.y = std::max(m_minSize.y, m_size.y);
      }

      bool isSmallerThanMin(glm::vec2 a, glm::vec2 b)
      {
        return a.x < b.x || a.y < b.y;
      }

      void confineSizeToContent()
      {
        glm::vec2 windowSize;
        getContext()->getContent()->getWindowSize(windowSize);

        if (m_pos.x + m_size.x > windowSize.x)
        {
          m_size.x += windowSize.x - (m_pos.x + m_size.x);
        }
        if (m_pos.x < 0)
        {
          m_size.x += m_pos.x;
        }
        if (m_pos.y + m_size.y > windowSize.y)
        {
          m_size.y += windowSize.y - (m_pos.y + m_size.y);
        }
      }
    };

    class GTextEdit : public GClickable, public GLinker<GTextEdit>
    {
    public:

      GTextEdit() {}

      GTextEdit(glm::vec2 pos, glm::vec2 size, std::string text, int fontSize, glm::vec4 color)
        : GClickable(pos,size),
          m_text(text),
          m_fontSize(fontSize),
          m_font(createFont()),
          m_cursorPosition(text.size())
      {
        setColor(color);
      }

      virtual void init(GContext *context, IGComponent *parent)
      {
        setContext(context);
        setParent(parent);
        setId("textEdit");
        inheritColorStyle();
      }
      virtual void validate() {}
      virtual void update() {}
      virtual bool checkMouseEvents(int button, int action)
      {
        bool eventHasHappened = false;

        eventHasHappened |= GClickable::checkMouseEvents(button, action);

        onPressed();

        return eventHasHappened;
      }
      virtual bool checkKeyEvents(int key, int action)
      {
        bool eventHasHappened = false;

        if (getContext()->isFocused(this))
        {
          if (GLFW_PRESS == action || GLFW_REPEAT == action)
          {
            if (key == GLFW_KEY_BACKSPACE)
            {
              if (m_text.size() > 0)
              {
                m_text.erase(m_text.begin() + m_cursorPosition - 1);
                m_cursorPosition--;
                m_cursorBlinkTimer = 0;
              }
              eventHasHappened = true;
            }
            if (key == GLFW_KEY_RIGHT)
            {
              if (m_cursorPosition < m_text.size())
              {
                m_cursorPosition++;
                m_cursorBlinkTimer = 0;
              }
              eventHasHappened = true;
            }
            if (key == GLFW_KEY_LEFT)
            {
              if (m_cursorPosition > 0)
              {
                m_cursorPosition--;
                m_cursorBlinkTimer = 0;
              }
              eventHasHappened = true;
            }
          }

          unsigned char key = 0U;
          key = getContext()->getContent()->getKeyboard()->popTypedInputChar();
          if (key != 0)
          {
            if (getContext()->isFocused(this))
            {
              insertChar(key);
              eventHasHappened = true;
              onType();
            }
          }
        }

        return eventHasHappened;
      }
      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
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
            {
              if (canDrawCursor())
              {
                drawCursor(parentMatrix, shaderHandle, contextHandle, ix, iy);
              }
            }
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
          {
            break;
          }
          m_font.draw(m_text[ic],
                      parentMatrix,
                      glm::vec2(ix, iy) + m_pos,
                      glm::vec2(m_fontSize),
                      shaderHandle,
                      contextHandle);

          if (m_cursorPosition == ic)
          {
            if (canDrawCursor())
            {
              drawCursor(parentMatrix, shaderHandle, contextHandle, ix, iy);
            }
          }
        }
        if(canDrawCursor())
        {
          if (!endOfText)
          {
            drawCursor(parentMatrix, shaderHandle, contextHandle, m_size.x, iy - m_fontSize);
          }
          else if(m_cursorPosition == m_text.size() || m_text.size() == 0)
          {
            drawCursor(parentMatrix, shaderHandle, contextHandle, ix, iy);
          }
        }
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

      void setMultiline(bool isMultiline)
      {
        m_isMultiline = isMultiline;
      }

      triggers:

      void onType()
      {
        callTrigger(&GTextEdit::onType);
      }

      void onPressed()
      {
        if (isPressed())
        {
          callTrigger(&GTextEdit::onPressed);
        }
      }

    private:
      int m_fontSize = GGUI_DEFAULT_FONT_SIZE;
      int m_cursorPosition = 0;
      bool m_isMultiline = true;
      int m_cursorBlinkTimer = 0;
      int m_cursorBlinkTime = 50;
      GFont m_font;
      std::string m_text;

      void insertChar(char c)
      {
        m_text.insert(m_cursorPosition,1,c);
        m_cursorPosition++;
        m_cursorBlinkTimer = 0;
      }

      bool canDrawCursor()
      {
        return (m_cursorBlinkTimer % m_cursorBlinkTime) < m_cursorBlinkTime / 2 && getContext()->isFocused(this);
      }

      void drawCursor(glm::mat4 parentMatrix,  GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle, float ix, float iy)
      {
        m_font.draw('|',
                    parentMatrix,
                    glm::vec2(ix, iy) + m_pos,
                    glm::vec2(m_fontSize),
                    shaderHandle,
                    contextHandle);
      }
    };
  }
}
