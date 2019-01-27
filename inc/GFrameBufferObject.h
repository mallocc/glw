#pragma once

#include "glm.h"
#include "opengl.h"
#include "GShaderHandle_T.h"
#include "GVertexBufferObject.h"


using glw::engine::glsl::GShaderHandle_T;
using glw::engine::buffers::GVertexBufferObject;


namespace glw
{
  namespace engine
  {
    namespace buffers
    {
      typedef GLuint GFrameBufferObjectId;

      // This is an FBO struct
      class GFrameBufferObject
      {
      public:

        GFrameBufferObject();

        ~GFrameBufferObject();

        // Frame size
        GFrameBufferObject(int w, int h);

        GFrameBufferObject(glm::vec2 windowSize);

        void draw(GShaderHandle_T handles);

        // Binds FBO for render
        void bind();

        // Unbinds all FBOs
        static void unbind();

        // Gets FBO ID
        GFrameBufferObjectId getId();

        void setTopLeft(glm::vec2 topLeft);
        void setSize(glm::vec2 size);

      private:
        GFrameBufferObjectId m_id;

        GLuint m_tex, m_depth;

        int m_width = 0, m_height = 0;

        GVertexBufferObject m_vbo;

        // Creates a FBO and its render texture and depth buffer
        GReturnCode createFBO();

        void createVBO();
      };
    }
  }
}
