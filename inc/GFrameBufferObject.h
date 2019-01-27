#pragma once

#include "glm.h"
#include "opengl.h"
#include "GMeshShaderHandle_T.h"
#include "GMesh.h"


using glw::engine::glsl::GMeshShaderHandle_T;
using glw::engine::buffers::GMesh;


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

        void draw(GMeshShaderHandle_T handles);

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

        GMesh m_fboMesh;

        // Creates a FBO and its render texture and depth buffer
        GReturnCode createFrameBufferObject();

        void createFrameBufferObjectMesh();
      };
    }
  }
}
