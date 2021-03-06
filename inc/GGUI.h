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
#include <pthread.h>
#include <set>


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
#define GGUI_BUTTON_ROLLOVER 50
#define GGUI_WINDOW_TITLBAR_HEIGHT 25
#define GGUI_WINDOW_DEADZONE 10

#define DCAST_COMPONENT(c, O,o) O* o = dynamic_cast<O*>(c)

#include <exception>
#include <typeinfo>
#include <stdexcept>

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
      const char * __CLASSNAME__ = "GGUISTUFF";
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

    class IGComponent : public GUnit
    {
    public:
      IGComponent() {}
      virtual ~IGComponent() {}

      virtual void createInnerComponents() = 0;

      virtual void init(GContext * context, IGComponent * parent) = 0;
      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle) = 0;

      virtual bool checkMouseEvents(int button, int action) = 0;
      virtual bool checkKeyEvents(int key, int action) = 0;

      virtual void validate() = 0;
      virtual void update() = 0;

      virtual glm::vec2 getPos() = 0;
      virtual glm::vec2 getSize() = 0;

      virtual std::string getComponentId() = 0;

      virtual void focusComponent() = 0;
      virtual void unfocusComponent() = 0;

      virtual bool isEnabled() = 0;
      virtual bool isVisible() = 0;

      virtual bool isInside(IGComponent* component) = 0;

      virtual bool isFocused() = 0;
    };

    class GGroup;
    class IGWindow
    {
    public:
      virtual GGroup& getChildren() = 0;
    };

    class GComponentTracker
    {
    public:

      static GComponentTracker& getInstance()
      {
          static GComponentTracker instance;
          return instance;
      }

      GComponentTracker()
        : m_components()
      {}

      bool tracking(std::string trackingId)
      {
        bool exists = false;

        std::map<std::string, IGComponent*>::iterator itor = m_components.find(trackingId);

        exists = (itor != m_components.end());

        if (exists)
        {
          itor->second->focusComponent();
        }

        return exists;
      }

      bool track(std::string trackingId, IGComponent* component)
      {
        bool exists = false;

        if (NULL != component)
        {
          std::map<std::string, IGComponent*>::iterator itor = m_components.find(trackingId);

          exists = (itor != m_components.end());

          if (exists)
          {
            itor->second->focusComponent();
          }
          else
          {
            m_components[trackingId] = component;
          }
        }
        else
        {
          LERROR("Trying to track a NULL component");
        }

        return !exists;
      }

      bool untrack(IGComponent* component)
      {
        bool success = false;

        if(NULL != component)
        {
          std::map<std::string, IGComponent*>::iterator itor = m_components.begin();

          bool found = false;

          while (itor != m_components.end() && !found)
          {
            if (itor->second == component)
            {
              m_components.erase(itor);
              found = true;
            }
            else
            {
              ++itor;
            }
          }

          success = found;
        }

        return success;
      }

    private:
      std::map<std::string, IGComponent*> m_components;
    };

#define TRACK(id, ptr) glw::gui::GComponentTracker::getInstance().track(id, ptr)
#define IF_NOT_TRACKING(id) if (!glw::gui::GComponentTracker::getInstance().tracking(id)) \
    { std::string TRACKER_ID = id;
#define FI_NOT_TRACKING(ptr) TRACK(TRACKER_ID, ptr); }
#define UNTRACK(ptr) glw::gui::GComponentTracker::getInstance().untrack(ptr)
#define MAKE_UNIQUE(id, ptr) if (!TRACK(id, ptr)) { delete ptr; }

    class GGroup
    {
    public:

      ~GGroup()
      {
        for (IGComponent* obj : m_group)
        {
          if (NULL != obj)
          {
            delete obj;
          }
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
          if (NULL != component)
          {
            if (component->isVisible())
            {
              component->draw(parentMatrix, shaderHandle, contextHandle);
            }
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
          if (NULL != component)
          {
            if (component->isVisible())
            {
              if (component->isEnabled())
              {
                eventHappenedInChild = component->checkMouseEvents(button, action);
              }
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
          if (NULL != component)
          {
            if (component->isVisible())
            {
              if (component->isEnabled())
              {
                eventHappenedInChild = component->checkKeyEvents(key, action);
              }
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
          if (NULL != component)
          {
            component->init(context, parent);
          }
        }
      }

      void validateGroup()
      {
        for (IGComponent * component : m_group)
        {
          if (NULL != component)
          {
            component->validate();
          }
        }
      }

      void updateGroup()
      {
        if (NULL != &m_group)
        {
          for (IGComponent * component : m_group)
          {
            if (NULL != component)
            {
              component->update();
            }
          }
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
              m_group.push_back(component);
            }
            break;
          }
        }
      }

      int getComponent(IGComponent * component)
      {
        if (NULL != component)
          if (NULL != &m_group)
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
          if (NULL != component)
          {
            delete component;
            component = NULL;
          }
          return true;
        }

        bool success = false;
        if (NULL != &m_group)
        {
          for (IGComponent * c : m_group)
          {
            if (NULL != c)
            {
              success |= dynamic_cast<GGroup*>(c)->removeComponent(component);
              if (success) break;
            }
          }
        }

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
          if (NULL != component)
          {
            glm::vec2 size = component->getSize();
            glm::vec2 pos = component->getPos();
            left = std::min(pos.x, left);
            right = std::max(pos.x + size.x, right);
            bottom = std::min(pos.y, bottom);
            top = std::max(pos.y + size.y, top);
          }
        }
        return glm::vec2(right + padding, top + padding);
      }

      size_t size()
      {
        return m_group.size();
      }

      void printComponentTree(int level, std::string pre, bool capEnd = true)
      {
        for (int ix = 0; ix < m_group.size(); ++ix)
        {
          std::string tree;
          bool end = capEnd && ix == m_group.size() - 1;
          if (level > 0)
          {
            if (end)
              tree.append(std::string(1, '\\'));
            else
              tree.append(std::string(1, '+'));
            tree.append(std::string(3, '-'));
          }
          LINFO(StringFormat("%2%0%1").arg(tree).arg(m_group[ix]->getId()).arg(pre).str());
          std::string t = pre;
          if (GGroup * group = dynamic_cast<GGroup*>(m_group[ix]))
          {
            if (level > 0)
            {
              if (end)
                t.append(std::string(1, ' '));
              else
                t.append(std::string(1, '|'));
              t.append(std::string(3, ' '));
            }
            if (IGWindow * window = dynamic_cast<IGWindow*>(m_group[ix]))
            {
              bool hasChildren = window->getChildren().size() > 0;
              group->printComponentTree(level + 1, t, !hasChildren);
              if (hasChildren)
              {
                LINFO(StringFormat("%2%0%1").arg(tree).arg("[CHILDREN]").arg(t).str());
                t.append(std::string(4, ' '));
                window->getChildren().printComponentTree(level + 2, t);
              }
            }
            else
            {
              group->printComponentTree(level + 1, t);
            }
          }
        }
      }

      std::vector<IGComponent*>& getGroupComponents()
      {
        return m_group;
      }

      bool hasFocusedChild()
      {
        bool focusedChild = false;

        if (NULL != &m_group)
        {
          for (IGComponent * c : m_group)
          {
            if (NULL != c)
            {
              if (!(focusedChild |= c->isFocused()))
              {
                if (focusedChild |= dynamic_cast<GGroup*>(c)->hasFocusedChild())
                  break;
              }
              else
                break;
            }
          }
        }

        return focusedChild;
      }

    private:
      std::vector<IGComponent*> m_group;
    };

    class GContext : public GGroup
    {
    public:

      GContext()
      {
        pthread_mutex_init(&m_mutex, NULL);
      }

      GContext(GContent * content)
        : m_content(content)
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
          LERROR("GContext is invalid.");
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
          else
          {
            m_focused = GNULLPTR;
          }
        }
        else
        {
          LERROR("GContext is invalid.");
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
          LERROR("GContext is invalid.");
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
        if (NULL != m_focused)
        {
          m_focused->unfocusComponent();
        }
        m_focused = component;
      }
      bool isFocused(IGComponent * component)
      {
        return component == m_focused;
      }

      GContextShaderProgram& getShaderProgram()
      {
        return m_shaderProgram;
      }

    private:
      GContent * m_content;
      GContextShaderProgram m_shaderProgram;

      GColorStyle m_colorStyle = {glw::WHITE_A, glw::BLACK_A, glw::GREY_A};

      bool m_isValid = false;

      IGComponent * m_focused = GNULLPTR;

      pthread_mutex_t m_mutex;
    };

    class GComponent : public IGComponent
    {
    public:
      GComponent() {}

      GComponent(glm::vec2 pos, glm::vec2 size) :
        m_pos(pos),
        m_size(size)
      {
      }

      virtual ~GComponent()
      {
        if (isFocused())
        {
          getContext()->setFocused(NULL);
        }
        UNTRACK(this);
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

      void bringToFront()
      {
        IGComponent * component = getParent();
        if (component != GNULLPTR)
        {
          dynamic_cast<GGroup*>(component)->bringToFront(this);
        }
      }

      virtual void focusComponent()
      {
        if (getContext() != NULL)
        {
          getContext()->setFocused(this);
        }
      }

      void removeFromParent()
      {
        IGComponent * component = getParent();
        if (component != GNULLPTR)
        {
          dynamic_cast<GGroup*>(component)->removeComponent(this);
        }
      }

      bool makeUnique(std::string id)
      {
        bool success = true;

        if(!TRACK(id, this))
        {
          removeFromParent();
          success = false;
        }

        return success;
      }

      void initComponent(GContext *context, IGComponent *parent, GUnitID id)
      {
        setContext(context);
        setParent(parent);
        setId(id);
        inheritColorStyle();
      }

      virtual bool checkMouseEvents(int button, int action)
      {
        return false;
      }
      virtual bool checkKeyEvents(int key, int action)
      {
        return false;
      }
      virtual void validate() {}
      virtual void update() {}

      virtual bool isFocused()
      {
        bool focused = false;

        if (NULL != getContext())
        {
          focused = getContext()->isFocused(this);
        }

        return focused;
      }

      bool isInside(glm::vec2 point)
      {
        glm::vec2 relPos = point - getPos();
        return relPos.x >= 0 && relPos.x <= getSize().x &&
            relPos.y >= 0 && relPos.y <= getSize().y;
      }

      virtual bool isInside(IGComponent* component)
      {
        bool inside = NULL != component;

        if (inside)
        {
          inside =
              isInside(component->getPos()) ||
              isInside(component->getPos() + component->getSize()) ||
              isInside(component->getPos() + component->getSize() * glm::vec2(1,0)) ||
              isInside(component->getPos() + component->getSize() * glm::vec2(0,1));
        }

        return inside;
      }

      virtual void unfocusComponent() {}

      virtual std::string getComponentId()
      {
        return getId();
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

      static GFont * getDefault()
      {
        static GFont * instance;

        if (NULL == instance)
        {
          GArrayVec2 uv;
          GArrayVec3 v, c, n, t;

          GPrimativeFactory::squareMeshes(v, 256);

          GPrimativeFactory::squareMeshUVs(uv, 16, 16);

          GArrayVertex o;
          GPrimativeFactory::packObject(o, v, c, n, t, uv);
          instance = new GFont(o, GUI_DEFAULT_FONT);
        }

        return instance;
      }

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

    class GImage : public GVertexBufferObject
    {
    public:
      GImage() {}

      GImage(GArrayVertex data, const char * imagefile)
        :  GVertexBufferObject(data,
                               glm::vec3(),
                               glm::vec3(0,0,1), 0.0f,
                               glm::vec3(0,0,1), 0.0f,
                               glm::vec3(1),
                               imagefile)
      {
      }

      void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        if(NULL != contextHandle.colorHandle)
        {
          contextHandle.colorHandle->load(m_color);
        }
        if (NULL != contextHandle.isFontHandle)
        {
          contextHandle.isFontHandle->load(0);
        }
        if (NULL != contextHandle.istextureHandle)
        {
          contextHandle.istextureHandle->load(1);
        }
        GVertexBufferObject::draw(parentMatrix, shaderHandle);
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

    private:
      glm::vec4 m_color;
    };

    class GLabel : public GComponent
    {
    public:

      GLabel()
      {
        createInnerComponents();
      }

      GLabel(GFont font, std::string text, glm::vec2 pos, float height, glm::vec4 color) :
        GComponent(pos, glm::vec2(height)),
        m_font(font),
        m_text(text)
      {
        setColor(color);
        createInnerComponents();
      }

      virtual void createInnerComponents() {}

      virtual void init(GContext * context, IGComponent * parent)
      {
        GComponent::initComponent(context, parent, "label");
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
    static GFont createFont(const char * fontfile)
    {
      GArrayVec2 uv;
      GArrayVec3 v, c, n, t;

      GPrimativeFactory::squareMeshes(v, 256);

      GPrimativeFactory::squareMeshUVs(uv, 16, 16);

      GArrayVertex o;
      GPrimativeFactory::packObject(o, v, c, n, t, uv);
      GFont font(o, fontfile);
      return font;
    }
    static GLabel * createLabel(std::string text, glm::vec2 pos, float height, glm::vec4 color, bool centered = false)
    {
      GLabel * label = new GLabel(*GFont::getDefault(), text, pos, height, color);
      label->centerBoth(centered);
      return label;
    }
    static GImage createImage(const char * imagefile)
    {
      GArrayVec2 uv;
      GArrayVec3 v, c, n, t;

      GPrimativeFactory::squareMesh(v, 1, 1);

      GPrimativeFactory::squareMeshUVs(uv, 1, 1);

      GArrayVertex o;
      GPrimativeFactory::packObject(o, v, c, n, t, uv);
      GImage image(o, imagefile);
      return image;
    }

    class GClickable : public GComponent
    {
    public:

      GClickable() {}

      GClickable(glm::vec2 pos, glm::vec2 size, bool isTogglable = false) :
        GComponent(pos, size),
        m_isTogglable(isTogglable)
      {}

      bool onClickablePressed(bool isTrue)
      {
        if (isTrue)
        {
          m_onPressed = true;
          m_onDown = true;
          if (m_focusable)
          {
            this->focusComponent();
          }
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

      void setToggled(bool toggled)
      {
        m_toggleState = toggled;
      }

      bool isToggled()
      {
        return m_toggleState;
      }

      void setIsTogglable(bool isTogglable)
      {
        m_isTogglable = isTogglable;
      }

      virtual void updateCounters()
      {
        onClickableDown(isDown());
      }

      void setFocusable(bool focusable)
      {
        m_focusable = focusable;
      }

      bool getFocusable()
      {
        return m_focusable;
      }

    protected:

      bool m_onDown = false;
      bool m_onReleased = false;
      bool m_onPressed = false;

      bool m_isTogglable = false;
      bool m_toggleState = false;

      int m_heldCounter = 0;
      int m_heldThreshold = GGUI_BUTTON_ROLLOVER;

      bool m_focusable = true;
    };

    class GButton : public GGroup, public GClickable, public GLinker
    {
    public:
      GButton()
      {
        createInnerComponents();
      }

      GButton(glm::vec2 pos, glm::vec2 size, std::string text = "", bool isTogglable = false):
        GClickable(pos, size, isTogglable), m_text(text)
      {
        createInnerComponents();
      }

      virtual void createInnerComponents()
      {
        m_back = createRectangle(glm::vec2(), glm::vec2(), glm::vec4());
        m_label = createLabel(m_text, glm::vec2(), 0, glm::vec4());
        addComponent(m_label);
      }

      virtual void init(GContext *context, IGComponent *parent)
      {
        GComponent::initComponent(context, parent, "button");

        m_label->centerHorizontal(true);

        initGroup(context, this);
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

      void setText(std::string text)
      {
        m_label->setText(text);
      }

      TRIGGERS_BASE(
          GBUTTON,
          DEFINE_TRIGGER(onPressed),
          DEFINE_TRIGGER(onDown),
          DEFINE_TRIGGER(onReleased),
          DEFINE_TRIGGER(onToggledOn),
          DEFINE_TRIGGER(onToggledOff)):

      trigger_func onPressed()
      {
        if (isPressed())
        {
          LINKER_CALL(onPressed);
        }
      }

      trigger_func onDown()
      {
        if (isDown())
        {
          LINKER_CALL(onDown);
        }
      }

      trigger_func onReleased()
      {
        if (isReleased())
        {
          LINKER_CALL(onReleased);
        }
      }

      trigger_func onToggledOn()
      {
        if (isToggled())
        {
          LINKER_CALL(onToggledOn);
        }
      }

      trigger_func onToggledOff()
      {
        if (!isToggled())
        {
          LINKER_CALL(onToggledOff);
        }
      }

    private:
      std::string m_text;
      GShape m_back;
      GLabel * m_label;
    };

    class GWindow : public IGWindow, public GGroup, public GClickable, public GLinker
    {
    public:

      GWindow()
        : m_title("Untitled")
      {
        createInnerComponents();
      }

      GWindow(glm::vec2 pos, glm::vec2 size, std::string title)
        : GClickable(pos, size),
          m_title(title)
      {
        createInnerComponents();
      }

      ~GWindow() {}

      virtual void createInnerComponents()
      {
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
      }

      virtual void init(GContext * context, IGComponent * parent)
      {
        // Inherit properties
        GComponent::initComponent(context, parent, "window");

        // Init window components and its children components
        initGroup(context, this);
        initChildren(context, this);
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        m_back.draw(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
        drawGroup(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
        m_children.drawGroup(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
      }

      virtual bool checkMouseEvents(int button, int action)
      {
        bool eventHasHappened = false;

        // Window native buttons are highest priority
        if (eventHasHappened |= checkGroupMouseEvents(button, action))
        {
          onFocus();
        }

        // Window child components next in priority
        if (!eventHasHappened)
        {
          if (eventHasHappened |= m_children.checkGroupMouseEvents(button, action))
          {
            GComponent::bringToFront();
          }
        }

        // If nothing else happened check if the component was clicked at all
        if (!eventHasHappened)
        {
          eventHasHappened |= GClickable::checkMouseEvents(button, action);
        }

        onClose();
        onScale();
        onWindowMove();
        onResize();

        return eventHasHappened;
      }

      virtual bool checkKeyEvents(int key, int action)
      {
        bool eventHasHappened = false;

        eventHasHappened |= m_children.checkGroupKeyEvents(key, action);

        return eventHasHappened;
      }

      virtual void validate()
      {
        m_minSize = m_children.getMinimumBounds(GGUI_DEFAULT_PADDING);
        inflateToContent();

        m_back.setPos(glm::vec2());
        m_back.setSize(getSize());
        m_back.setColor(getColorStyle().background / 2.0f);

        m_titleBar->setPos(glm::vec2());
        m_titleBar->setSize(glm::vec2(getSize().x, GGUI_WINDOW_TITLBAR_HEIGHT));

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


        // Override colorstyles
        GColorStyle colorStyle = getColorStyle();

        m_back.setColor(getColorStyle().background/2.0f);

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

        validateGroup();
        m_children.validateGroup();

        confinePositionToWindow();
        confineSizeToContent();
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

      void addChildComponent(IGComponent * component)
      {
        m_children.addComponent(component);
      }

      void closeWindow()
      {
        removeFromParent();
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

        m_leftResizeBar->setVisible(resizable);
        m_rightResizeBar->setVisible(resizable);
        m_bottomResizeBar->setVisible(resizable);
        m_bottomLeftResizeBar->setVisible(resizable);
        m_bottomRightResizeBar->setVisible(resizable);
      }
      void setResizableVeritcal(bool resizable)
      {
        m_isResizableX = !resizable;
        m_isResizableY = resizable;

        m_bottomResizeBar->setVisible(resizable);
        m_bottomLeftResizeBar->setVisible(resizable);
        m_bottomRightResizeBar->setVisible(resizable);
      }
      void setResizableHorizontal(bool resizable)
      {
        m_isResizableX = resizable;
        m_isResizableY = !resizable;

        m_leftResizeBar->setVisible(resizable);
        m_rightResizeBar->setVisible(resizable);
        m_bottomLeftResizeBar->setVisible(resizable);
        m_bottomRightResizeBar->setVisible(resizable);
      }
      void setResizableBoth(bool resizable)
      {
        m_isResizableX = resizable;
        m_isResizableY = resizable;

        m_leftResizeBar->setVisible(resizable);
        m_rightResizeBar->setVisible(resizable);
        m_bottomResizeBar->setVisible(resizable);
        m_bottomLeftResizeBar->setVisible(resizable);
        m_bottomRightResizeBar->setVisible(resizable);
      }

      void setScalable(bool scalable)
      {
        m_scalable = scalable;

        m_maxmin->setVisible(scalable);
      }

      virtual void focusComponent()
      {
        onFocus();
      }

      virtual GGroup& getChildren()
      {
        return m_children;
      }

      TRIGGERS_BASE(
          GWINDOW,
          DEFINE_TRIGGER(onClose),
          DEFINE_TRIGGER(onScale),
          DEFINE_TRIGGER(onWindowMove),
          DEFINE_TRIGGER(onResize),
          DEFINE_TRIGGER(onFocus)):

      trigger_func onClose()
      {
        if (m_close->isReleasedOver())
        {
          LINKER_CALL(onClose);
          closeWindow();
        }
      }

      trigger_func onScale()
      {
        if (m_maxmin->isReleasedOver())
        {
          toggleMaximise();
          LINKER_CALL(onScale);
        }
      }

      trigger_func onWindowMove()
      {
        if (m_titleBar->isDragging())
        {
          LINKER_CALL(onWindowMove);
        }
      }

      trigger_func onResize()
      {
        if(m_isResizable && !m_maximised)
        {
          if (m_leftResizeBar->isDragging() || m_rightResizeBar->isDragging() || m_bottomResizeBar->isDragging())
          {
            LINKER_CALL(onResize);
          }
        }
      }

      trigger_func onFocus()
      {
        GComponent::focusComponent();
        GComponent::bringToFront();
        LINKER_CALL(onFocus);
      }

    protected:

      void initChildren(GContext * context, IGComponent * parent)
      {
        m_children.initGroup(context, parent);
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

      bool m_scalable = true;

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

    class GTextEdit : public GClickable, public GLinker
    {
    public:

      GTextEdit()
      {
        createInnerComponents();
      }

      GTextEdit(glm::vec2 pos, glm::vec2 size, std::string text, int fontSize, glm::vec4 color, bool editable = true)
        : GClickable(pos,size),
          m_text(text),
          m_fontSize(fontSize),
          m_font(*GFont::getDefault()),
          m_cursorPosition(text.size()),
          m_editable(editable)
      {
        setColor(color);
        createInnerComponents();
      }

      virtual void createInnerComponents() {}

      virtual void init(GContext *context, IGComponent *parent)
      {
        GComponent::initComponent(context, parent, "textedit");
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

      virtual bool checkMouseEvents(int button, int action)
      {
        bool eventHasHappened = false;

        if (m_editable)
        {
          eventHasHappened |= GClickable::checkMouseEvents(button, action);
        }

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

      TRIGGERS_BASE(
          GTEXTEDIT,
          DEFINE_TRIGGER(onType),
          DEFINE_TRIGGER(onPressed)):

      trigger_func onType()
      {
        LINKER_CALL(onType);
      }

      trigger_func onPressed()
      {
        if (isPressed())
        {
          LINKER_CALL(onPressed);
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

      bool m_editable;

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

    class GSlider : public GGroup, public GClickable, public GLinker
    {
    public:
      GSlider(glm::vec2 pos, glm::vec2 size, float value, float inc, float isVertical = false)
        : GClickable(pos, size),
          m_value(value),
          m_inc(inc),
          m_isVertical(isVertical)
      {
        createInnerComponents();
      }

      virtual void createInnerComponents()
      {
        if (m_isVertical)
        {
          m_down = new GButton(glm::vec2(0, m_size.y - m_size.x), glm::vec2(m_size.x, m_size.x), "\\/");
          addComponent(m_down);

          m_up = new GButton(glm::vec2(), glm::vec2(m_size.x, m_size.x), "/\\");
          addComponent(m_up);

          m_bar = new GButton(glm::vec2(0, m_size.x), glm::vec2(m_size.x, m_size.x), "=");
          addComponent(m_bar);
        }
        else
        {
          m_down = new GButton(glm::vec2(m_size.x - m_size.y, 0), glm::vec2(m_size.y, m_size.y), ">");
          addComponent(m_down);

          m_up = new GButton(glm::vec2(), glm::vec2(m_size.y, m_size.y), "<");
          addComponent(m_up);

          m_bar = new GButton(glm::vec2(m_size.y, 0), glm::vec2(m_size.y, m_size.y), "||");
          addComponent(m_bar);
        }
      }

      virtual void init(GContext * context, IGComponent * parent)
      {
        GComponent::initComponent(context, parent, "slider");

        initGroup(context, this);
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        drawGroup(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
      }

      virtual bool checkMouseEvents(int button, int action)
      {
        bool eventHasHappened = false;

        eventHasHappened |= checkGroupMouseEvents(button, action);

        onUp();
        onDown();
        onBarMove();

        return eventHasHappened;
      }

      virtual void validate()
      {
        snapValue();

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

      virtual void update()
      {
        m_down->updateCounters();
        m_up->updateCounters();

        if(m_down->isHeldOver())
        {
          increment();
          validate();
        }

        if(m_up->isHeldOver())
        {
          decrement();
          validate();
        }

        if (m_bar->isDragging())
        {
          glm::vec2 delta = getContext()->getContent()->getMouse()->getMouseDelta();
          if (m_isVertical)
          {
            setValue((m_bar->getPos().y + delta.y - m_size.x) / (m_size.y - m_size.x * 3));
          }
          else
          {
            setValue((m_bar->getPos().x + delta.x - m_size.y) / (m_size.x - m_size.y * 3));
          }
          validate();
        }

        if (m_bar->isReleased())
        {
          snapValue();
        }

        updateGroup();
      }

      void snapValue()
      {
        m_value = round(m_value / m_inc) / (1 / m_inc);
      }
      void setValue(float amount)
      {
        m_value = amount;
        m_value = std::min(m_value, 1.0f);
        m_value = std::max(m_value, 0.0f);
        char number[24];
        sprintf(number, "%.1f", m_value);
      }
      float getValue()
      {
        return m_value;
      }

      void increment()
      {
        m_value += m_inc;
        m_value = std::min(m_value, 1.0f);
        setValue(m_value);
      }

      void decrement()
      {
        m_value -= m_inc;
        m_value = std::max(m_value, 0.0f);
        setValue(m_value);
      }

      TRIGGERS_BASE(
          GSLIDER,
          DEFINE_TRIGGER(onDown),
          DEFINE_TRIGGER(onUp),
          DEFINE_TRIGGER(onBarMove)):

      trigger_func onDown()
      {
        if (m_down->isPressed() || m_down->isHeldOver())
        {
          increment();
          validate();
          LINKER_CALL(onDown);
        }
      }
      trigger_func onUp()
      {
        if (m_up->isPressed() || m_up->isHeldOver())
        {
          decrement();
          validate();
          LINKER_CALL(onUp);
        }
      }
      trigger_func onBarMove()
      {
        if (m_bar->isDragging())
        {
          validate();
          LINKER_CALL(onBarMove);
        }
      }


    private:
      float m_value;
      float m_inc;

      bool m_isVertical = false;

      GButton * m_down;
      GButton * m_up;
      GButton * m_bar;

    };

    class GSpinner : public GGroup, public GClickable, public GLinker
    {
    public:
      GSpinner(glm::vec2 pos, glm::vec2 size, float value = 0.0f, float inc = 1.0f)
        : GClickable(pos, size),
          m_value(value),
          m_inc(inc)
      {
        createInnerComponents();
      }

      virtual void createInnerComponents()
      {
        m_label = new GButton(glm::vec2(m_size.x / 6, 0), glm::vec2(m_size.x / 6 * 4, m_size.y), "0");
        addComponent(m_label);

        m_plus = new GButton(glm::vec2(), glm::vec2(m_size.x / 6, m_size.y), "+");
        addComponent(m_plus);

        m_minus = new GButton(glm::vec2(m_size.x / 6 * 5, 0), glm::vec2(m_size.x / 6, m_size.y), "-");
        addComponent(m_minus);
      }

      virtual void init(GContext * context, IGComponent * parent)
      {
        GComponent::initComponent(context, parent, "spinner");

        initGroup(context, this);
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        drawGroup(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
      }

      virtual bool checkMouseEvents(int button, int action)
      {
        bool eventHasHappened = false;

        eventHasHappened |= checkGroupMouseEvents(button, action);

        onIncrease();
        onDecrease();
        onReset();

        return eventHasHappened;
      }

      virtual void validate()
      {
        m_label->setPos(glm::vec2(m_size.x / 6, 0));
        m_label->setSize(glm::vec2(m_size.x / 6*4, m_size.y));

        m_plus->setPos(glm::vec2());
        m_plus->setSize(glm::vec2(m_size.x / 6, m_size.y));

        m_minus->setPos(glm::vec2(m_size.x / 6 * 5, 0));
        m_minus->setSize(glm::vec2(m_size.x / 6, m_size.y));

        validateGroup();
      }

      virtual void update()
      {
        m_plus->updateCounters();
        m_minus->updateCounters();

        if (m_plus->isHeldOver())
        {
          increase();
        }

        if (m_minus->isHeldOver())
        {
          decrease();
        }

        updateGroup();
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

      void increase()
      {
        m_value += m_inc;
        setValue(m_value);
        validate();
      }

      void decrease()
      {
        m_value -= m_inc;
        setValue(m_value);
        validate();
      }

      TRIGGERS_BASE(
          GSPINNER,
          DEFINE_TRIGGER(onIncrease),
          DEFINE_TRIGGER(onDecrease),
          DEFINE_TRIGGER(onReset)):

      trigger_func onIncrease()
      {
        if (m_plus->isReleasedOver())
        {
          increase();
          LINKER_CALL(onIncrease);
        }
      }
      trigger_func onDecrease()
      {
        if (m_minus->isReleasedOver())
        {
          decrease();
          LINKER_CALL(onDecrease);
        }
      }
      trigger_func onReset()
      {
        if (m_label->isReleasedOver())
        {
          setValue(0);
          validate();
          LINKER_CALL(onReset);
        }
      }

    private:
      float m_value;
      float m_inc;
      GButton * m_plus;
      GButton * m_minus;
      GButton * m_label;
    };

    template <class T>
    class GDropdown : public GGroup, public GClickable, public GLinker
    {
    public:

      GDropdown(glm::vec2 pos, glm::vec2 size)
        : GClickable(pos, size)
      {
        createInnerComponents();
      }

      virtual void createInnerComponents()
      {
        m_selected = new GButton(glm::vec2(), getSize(), getSelectedName(), true);
        addComponent(m_selected);
      }

      virtual void init(GContext * context, IGComponent * parent)
      {
        GComponent::initComponent(context, parent, "dropdown");

        initGroup(context, this);
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        drawGroup(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);

        if (m_selected->isToggled())
        {
          for (Item& item : m_items)
          {
            item.m_button->draw(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
          }
        }
      }

      virtual bool checkMouseEvents(int button, int action)
      {
        bool eventHasHappened = false;

        for (int i = 0; i < m_items.size(); ++i)
        {
          if (m_items[i].m_button->checkMouseEvents(button, action))
          {
            if (m_selected->isToggled())
            {
              if (m_items[i].m_button->isPressed())
              {
                m_selected->toggle();
                setSelectedId(i);
                m_items[i].m_button->validate();
                eventHasHappened |= true;
                onSelect();
              }
            }
          }
        }

        if (!eventHasHappened)
        {
          eventHasHappened |= checkGroupMouseEvents(button, action);
        }

        onDropdown();

        return eventHasHappened;
      }

      virtual void validate()
      {
        m_selected->setPos(glm::vec2());
        m_selected->setSize(getSize());
        m_selected->setText(getSelectedName());

        for (int ix = 0; ix < m_items.size(); ++ix)
        {
          glm::vec2 pos(0, (ix + 1) * getSize().y);

          m_items[ix].m_button->setPos(pos);
          m_items[ix].m_button->setSize(getSize());
          m_items[ix].m_button->validate();
        }

        validateGroup();
      }

      virtual void update()
      {
        if (!getContext()->isFocused(m_selected))
        {
          m_selected->setToggled(false);
        }
      }

      std::string getSelectedName()
      {
        std::string name;

        if (!m_items.empty())
        {
          name = m_items[m_selectedId].m_name;
        }

        return name;
      }

      T * getSelectedObj()
      {
        T * obj = NULL;

        if (!m_items.empty())
        {
          obj = m_items[m_selectedId].m_obj;
        }

        return obj;
      }

      void add(T& obj, std::string name)
      {
        if (GNULLPTR != getContext())
        {
          GButton * button = new GButton(glm::vec2(), getSize(), name);
          button->init(getContext(), this);

          m_items.push_back({button, &obj, name});
        }
        else
        {
        }
      }

      void setSelectedId(int id)
      {
        if (id >= 0 && id < m_items.size())
        {
          m_selectedId = id;
          m_selected->setText(getSelectedName());
        }
      }

      TRIGGERS_BASE(
          GDROPDOWN,
          DEFINE_TRIGGER(onSelect),
          DEFINE_TRIGGER(onDropdown)):

      trigger_func onSelect()
      {
        LINKER_CALL(onSelect);
      }

      trigger_func onDropdown()
      {
        if (m_selected->isPressed() && m_selected->isToggled())
        {
          LINKER_CALL(onDropdown);
        }
      }

    private:
      struct Item
      {
        GButton * m_button;
        T * m_obj;
        std::string m_name;
      };
      std::vector<Item> m_items;

      int m_selectedId = 0;

      GButton * m_selected;

      int getItemId(std::string name)
      {
        for (int i = 0; i < m_items.size(); ++i)
        {
          if (m_items[i].m_name == name)
          {
            return i;
          }
        }
      }
    };

    class GImageView : public GClickable, public GLinker
    {
    public:

      GImageView() {}

      GImageView(glm::vec2 pos, glm::vec2 size, const char * imagefile)
        : GClickable(pos, size),
          m_imagefile(imagefile)
      {
        createInnerComponents();
      }

      virtual void createInnerComponents() {}

      virtual void init(GContext *context, IGComponent *parent)
      {
        GComponent::initComponent(context, parent, "imageview");

        m_image = createImage(m_imagefile);
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        m_image.draw(parentMatrix, shaderHandle, contextHandle);
      }

      virtual void validate()
      {
        GComponent::validate();
        m_image.setPos(getPos());
        m_image.setSize(getSize());
      }

      GImage& getImage()
      {
        return m_image;
      }

      TRIGGERS_BASE(GIMAGEVIEW, DEFINE_TRIGGER_NONE):

    private:

      const char * m_imagefile;

      GImage m_image;

    };

    class GProgressBar : public GGroup, public GClickable, public GLinker
    {
    public:

      GProgressBar()
      {
        createInnerComponents();
      }

      GProgressBar(glm::vec2 pos, glm::vec2 size, float progressMin, float progressMax, bool vertical = false)
        : GClickable(pos, size),
          m_progressMin(progressMin),
          m_progressMax(progressMax),
          m_progressCurrent(m_progressMin),
          m_vertical(vertical)
      {
        createInnerComponents();
      }

      virtual void createInnerComponents()
      {
        m_back = createRectangle(getPos(), m_size, getColorStyle().background);
        m_front = createRectangle(getPos(), getSize() * glm::vec2(getPercentageProgress(), 1), getColorStyle().accent);
        m_label = createLabel("0", getPos() + glm::vec2(getSize().x / 2, 0), m_size.y, getColorStyle().foreground);

        addComponent(m_label);
      }

      virtual void init(GContext *context, IGComponent *parent)
      {
        GComponent::initComponent(context, parent, "progressbar");

        m_label->centerHorizontal(true);

        initGroup(context, this);
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        m_back.draw(parentMatrix, shaderHandle, contextHandle);
        m_front.draw(parentMatrix, shaderHandle, contextHandle);
        m_label->draw(parentMatrix, shaderHandle, contextHandle);
      }

      virtual bool checkMouseEvents(int button, int action)
      {
        bool eventHasHappened = false;

        eventHasHappened |= GClickable::checkMouseEvents(button, action);

        return eventHasHappened;

      }

      virtual void validate()
      {
        m_back.setPos(getPos());
        m_back.setSize(getSize());
        m_back.setColor(getColorStyle().background);

        if (m_vertical)
        {
          m_front.setPos(getPos() + glm::vec2(0, getSize().y) * glm::vec2(1, 1 - getPercentageProgress()));
          m_front.setSize(getSize() * glm::vec2(1, getPercentageProgress()));
          m_front.setColor(getColorStyle().accent);
        }
        else
        {
          m_front.setPos(getPos());
          m_front.setSize(getSize() * glm::vec2(getPercentageProgress(), 1));
          m_front.setColor(getColorStyle().accent);
        }

        m_label->setPos(getPos() + glm::vec2(getSize().x / 2, 0));
        m_label->setColor(getColorStyle().foreground);
        m_label->setHeight(getSize().y);

        validateGroup();
      }

      virtual void update()
      {
        m_label->update();
      }

      void setProgress(float progress)
      {
        if (progress >= m_progressMin && progress <= m_progressMax)
        {
          m_progressCurrent = progress;
          char number[24];
          sprintf(number, "%.1f", m_progressCurrent);
          m_label->setText(number);
          validate();
        }
      }

      float getProgress()
      {
        return m_progressCurrent;
      }

      TRIGGERS_BASE(GPROGRESSBAR, DEFINE_TRIGGER_NONE):

    private:

      GShape m_back;
      GShape m_front;
      GLabel * m_label;

      float m_progressMin;
      float m_progressMax;
      float m_progressCurrent;

      bool m_vertical;

      float getPercentageProgress()
      {
        return m_progressCurrent / (m_progressMax - m_progressMin);
      }

    };

    class GCheckBox : public GClickable, public GLinker
    {
    public:
      GCheckBox()
      {
        createInnerComponents();
      }

      GCheckBox(glm::vec2 pos, glm::vec2 size)
        : GClickable(pos, size, true)
      {
        createInnerComponents();
      }

      virtual void createInnerComponents()
      {
        m_border = createBox(getPos(), getSize(), 0.1f, getColorStyle().accent);
        m_check = createRectangle(getPos(), getSize(), getColorStyle().accent);
      }

      virtual void init(GContext *context, IGComponent *parent)
      {
        GComponent::initComponent(context, parent, "checkbox");
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        m_border.draw(parentMatrix, shaderHandle, contextHandle);
        if (isToggled())
        {
          m_check.draw(parentMatrix, shaderHandle, contextHandle);
        }
      }

      virtual bool checkMouseEvents(int button, int action)
      {
        bool eventHasHappened = false;

        eventHasHappened |= GClickable::checkMouseEvents(button, action);

        return eventHasHappened;

      }

      virtual void validate()
      {
        m_border.setPos(getPos());
        m_border.setSize(getSize());
        m_border.setColor(getColorStyle().accent);
        m_check.setPos(getPos() + getSize() * 0.2f);
        m_check.setSize(getSize() * 0.6f);
        m_check.setColor(getColorStyle().accent);
      }

      TRIGGERS_BASE(GCHECKBOX, DEFINE_TRIGGER_NONE):

    private:

      GShape m_border;
      GShape m_check;

    };

    class GPane : public GComponent, public GGroup
    {
    public:

      GPane()
      {
        createInnerComponents();
      }

      GPane(glm::vec2 pos, glm::vec2 size)
        : GComponent(pos, size)
      {
        createInnerComponents();
      }

      virtual void createInnerComponents() {}

      virtual void init(GContext * context, IGComponent * parent)
      {
        GComponent::initComponent(context, parent, "pane");

        initGroup(context, this);
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        drawGroup(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
      }

      virtual bool checkMouseEvents(int button, int action)
      {
        return checkGroupMouseEvents(button, action);
      }

      virtual bool checkKeyEvents(int key, int action)
      {
        return checkGroupKeyEvents(key, action);
      }

      virtual void validate()
      {
        GComponent::validate();
        validateGroup();
      }

      virtual void update()
      {
        GComponent::update();
        updateGroup();
      }

    private:
    };

    class GContainer : public GClickable, public GGroup
    {
    public:

      GContainer()
      {
        createInnerComponents();
      }

      GContainer(glm::vec2 pos, glm::vec2 size)
        : GClickable(pos, size)
      {
        createInnerComponents();
      }

      virtual void createInnerComponents() {}

      virtual void init(GContext * context, IGComponent * parent)
      {
        GComponent::initComponent(context, parent, "container");

        initGroup(context, this);
      }

      virtual void draw(glm::mat4 parentMatrix, GShaderHandle_T shaderHandle, GContextShaderHandle_T contextHandle)
      {
        drawGroup(parentMatrix * getRelativeModelMatrix(), shaderHandle, contextHandle);
      }

      virtual bool checkMouseEvents(int button, int action)
      {
        return checkGroupMouseEvents(button, action) || GClickable::checkMouseEvents(button, action);
      }

      virtual bool checkKeyEvents(int key, int action)
      {
        return checkGroupKeyEvents(key, action) || GClickable::checkKeyEvents(key, action);
      }

      virtual void validate()
      {
        GComponent::validate();
        validateGroup();
      }

      virtual void update()
      {
        GComponent::update();
        updateGroup();
      }

    private:
    };

    class GDialog : public GWindow
    {
    public:

      GDialog()
      {
        createInnerComponents();
      }

      GDialog(glm::vec2 pos, std::string message,
              std::string confirmText = "Okay",
              std::string cancelText = "Cancel",
              std::string title = "")
        : GWindow(pos, glm::vec2(), title),
          m_messageText(message),
          m_confirmText(confirmText),
          m_cancelText(cancelText)
      {
        createInnerComponents();
      }

      virtual void createInnerComponents()
      {
        m_message = new GTextEdit(glm::vec2(GGUI_DEFAULT_PADDING), glm::vec2(220, GGUI_DEFAULT_PADDING), m_messageText, GGUI_DEFAULT_FONT_SIZE, getColorStyle().foreground, false);
        addChildComponent(m_message);

        m_imageView = new GImageView(m_message->getPos() + glm::vec2(m_message->getSize().x, 0), glm::vec2(35), "../textures/question_mark.png");
        addChildComponent(m_imageView);

        m_confirm = new GButton(glm::vec2(50, m_message->getSize().y + GGUI_DEFAULT_PADDING), glm::vec2(100, GGUI_DEFAULT_FONT_SIZE), m_confirmText);
        addChildComponent(m_confirm);
        LINKER_NEW_LINK(m_confirm, GButton::T_onPressed, ACTION(*this, &GDialog::onConfirm));

        m_cancel = new GButton(glm::vec2(200, m_message->getSize().y + GGUI_DEFAULT_PADDING), glm::vec2(100, GGUI_DEFAULT_FONT_SIZE), m_cancelText);
        addChildComponent(m_cancel);
        LINKER_NEW_LINK(m_cancel, GButton::T_onPressed, ACTION(*this, &GDialog::onCancel));
      }

      virtual void init(GContext *context, IGComponent *parent)
      {
        GWindow::init(context, parent);
        setId("dialog");

        setResizeable(false);
        setScalable(false);
      }

      virtual void validate()
      {
        GWindow::validate();

        m_message->setPos(glm::vec2(GGUI_DEFAULT_PADDING));
        m_message->setSize(glm::vec2(220, GGUI_DEFAULT_PADDING));
        m_message->setColor(getColorStyle().foreground);

        m_imageView->setPos(m_message->getPos() + glm::vec2(m_message->getSize().x, 0));
        m_imageView->setSize(glm::vec2(35));

        m_confirm->setPos(glm::vec2(50, m_message->getSize().y + GGUI_DEFAULT_PADDING));
        m_confirm->setSize(glm::vec2(100, GGUI_DEFAULT_FONT_SIZE));

        m_cancel->setPos(glm::vec2(200, m_message->getSize().y + GGUI_DEFAULT_PADDING));
        m_cancel->setSize(glm::vec2(100, GGUI_DEFAULT_FONT_SIZE));
      }

      void finish()
      {
        removeFromParent();
      }

      virtual void addConfirmCallback(glw::meta::GAction action)
      {
        LINKER_LINK(GDialog::T_onConfirm, action);
      }

      virtual void addCancelCallback(glw::meta::GAction action)
      {
        LINKER_LINK(GDialog::T_onCancel, action);
        LINKER_LINK(GDialog::T_onClose, action);
      }

      TRIGGERS_DERIVED(
          GDIALOG, GWINDOW,
          DEFINE_TRIGGER(onConfirm),
          DEFINE_TRIGGER(onCancel),
          DEFINE_TRIGGER(onClose)):

      trigger_func onConfirm()
      {
        m_outcome = true;
        LINKER_CALL(onConfirm);
        finish();
      }

      trigger_func onCancel()
      {
        m_outcome = false;
        LINKER_CALL(onCancel);
        finish();
      }

      trigger_func onClose()
      {
        m_outcome = false;
        LINKER_CALL(onClose);
        finish();
      }

    private:
      std::string m_confirmText;
      std::string m_cancelText;
      std::string m_messageText;

      GImageView * m_imageView;
      GButton * m_confirm;
      GButton * m_cancel;
      GTextEdit * m_message;

      bool m_outcome = false;
    };

    static GDialog * createDialog(GContext& context, IGComponent *parent,
                                  std::string message,
                                  std::string title = "Message",
                                  std::string confirmText = "Okay",
                                  std::string cancelText = "Cancel")
    {
      glm::vec2 windowSize;
      context.getContent()->getWindowSize(windowSize);
      GDialog * dialog = new GDialog(windowSize / 2.0f, message, confirmText, cancelText, title);
      if (dynamic_cast<GGroup*>(parent) != GNULLPTR)
      {
        GGroup * group = dynamic_cast<GGroup*>(parent);
        group->addComponent(dialog);
      }
      dialog->init(&context, parent);
      dialog->validate();
      dialog->setPos(windowSize / 2.0f - dialog->getSize() / 2.0f);
      return dialog;
    }

  }
}



/*
  virtual void draw(glm::mat4 parentMatrix, glw::engine::glsl::GShaderHandle_T shaderHandle, glw::gui::GContextShaderHandle_T contextHandle)
  {

  }
  virtual bool checkKeyEvents(int key, int action)
  {
    return false;
  }
  virtual bool checkMouseEvents(int button, int action)
  {
    return false;
  }
  virtual void init(glw::gui::GContext *context, IGComponent *parent)
  {

  }
  virtual void validate()
  {

  }
  virtual void update()
  {

  }
*/
