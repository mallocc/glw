#pragma once

#include "Logger.h"
#include "glm.h"
#include "GVertexBufferObject.h"
#include "GPrimativeFactory.h"
#include "GShaderProgram.h"
#include "GShaderVariableHandle.h"

using glw::engine::buffers::GVertexBufferObject;
using glw::engine::buffers::GPrimativeFactory;
using glw::engine::buffers::GArrayVertex;
using glw::engine::buffers::GArrayVec2;
using glw::engine::buffers::GArrayVec3;
using glw::engine::glsl::GShaderProgram;
using glw::engine::glsl::GShaderVariableHandleId;

namespace
{
  const char * GUI_SHADER_VERT = "../shaders/gui.vert";
  const char * GUI_SHADER_FRAG = "../shaders/gui.frag";
}

namespace glw
{
  namespace gui
  {
    class GGUI
    {
    public:
      
      GGUI();
      
      ~GGUI();
      
    private:
      
    };

    namespace
    {
      const char * TRG = "GGUI";
    }


    class GShape : public GVertexBufferObject
    {
    public:
      GShape() {}

      GShape(GArrayVertex data,
             glm::vec2 pos,
             glm::vec2 size) :
        GVertexBufferObject(data,
                            glm::vec3(),
                            glm::vec3(0,0,1),0.0f,
                            glm::vec3(0,0,1),0.0f)
      {
        m_pos = glm::vec3(pos, 0);
        m_scale = glm::vec3(size, 0);
        m_color = glm::vec4(0,0,0,1);
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
    private:
      glm::vec4 m_color;
      bool m_isFont;
      bool m_isTexture;
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

    private:
      GShaderVariableHandleId m_color, m_isFont, m_isTexture;

    };

    class GContext
    {
    public:

      GContext() {}

      void initShaderProgram(glm::mat4 * model_data,
                      glm::mat4 * view_data,
                      glm::mat4 * proj_data)
      {
        m_shaderProgram = GContextShaderProgram(model_data,view_data,proj_data);
      }

      void addShape(GShape shape)
      {
        m_shapes.push_back(shape);
      }

      void draw()
      {
        if (m_shaderProgram.isValid())
        {
          m_shaderProgram.load();
          for (GShape& shape : m_shapes)
          {
            m_shaderProgram.getColorHandle()->load(shape.getColor());
            m_shaderProgram.getIsFontHandle()->load(shape.isFont());
            m_shaderProgram.getIsTextureHandle()->load(shape.isTexture());
            shape.draw(m_shaderProgram.getShaderHandle());
          }
        }
        else
        {
          LINFO(TRG, "NULL shaderProgram");
        }
      }

    private:
      std::vector<GShape> m_shapes;

      GContextShaderProgram m_shaderProgram;
    };

    static GShape createBox(glm::vec2 pos, glm::vec2 size, float weight)
    {
      GArrayVec2 uv;
      GArrayVec3 v, c, n, t;

      GPrimativeFactory::squareBorder(v, weight, size.x/size.y);

      GPrimativeFactory::colourBuffer(c,glm::vec3(1),v.size());

      GArrayVertex o;
      GPrimativeFactory::packObject(o, v, c, n, t, uv);
      GShape shape(o, pos, size);
      return shape;
    }

    static GShape createRectangle(glm::vec2 pos, glm::vec2 size)
    {
      GArrayVec2 uv;
      GArrayVec3 v, c, n, t;

      GPrimativeFactory::squareMesh(v, 1, 1);

      GPrimativeFactory::colourBuffer(c,glm::vec3(1),v.size());

      GArrayVertex o;
      GPrimativeFactory::packObject(o, v, c, n, t, uv);
      GShape shape(o, pos, size);
      return shape;
    }

    static GShape createCircle(glm::vec2 pos, glm::vec2 size)
    {
      GArrayVec2 uv;
      GArrayVec3 v, c, n, t;

      GPrimativeFactory::circle(v, 32);

      GPrimativeFactory::colourBuffer(c,glm::vec3(1),v.size());

      GArrayVertex o;
      GPrimativeFactory::packObject(o, v, c, n, t, uv);
      GShape shape(o, pos, size);
      return shape;
    }
  }
}
