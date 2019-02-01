#pragma once

#include "opengl.h"
#include "glm.h"

#include "GShaderHandle_T.h"
#include "GVertex_T.h"
#include "GShaderVariableHandle.h"
#include "GReturnCode.h"

#include <vector>


using glw::engine::glsl::GShaderHandle_T;
using glw::engine::buffers::GVertex_T;
using glw::engine::glsl::GShaderVariableHandle;
using glw::GReturnCode;


namespace glw
{
  namespace engine
  {
    namespace buffers
    {
      class GVertexBufferObject
      {
      public:
        
        GVertexBufferObject();
        
        // Texture filename, Vertex data pack, world position, 
        // dynamic axis of rotation, and amount, static axis of rotation, 
        // and amount, scale vector. 
        GVertexBufferObject(
				    std::vector<GVertex_T>& data,
				    glm::vec3 pos,
				    glm::vec3 rotation,
				    GLfloat theta,
				    glm::vec3 preRotation,
				    GLfloat preTheta,
            glm::vec3 scale = glm::vec3(1),
            const char * texfilename = "NULL");
        
        ~GVertexBufferObject();
        
        // Buffers Vertex data into the VBO
			  GReturnCode init(std::vector<GVertex_T> * d);

        // Loads image file into a texture
        void loadTextures(const char * texfilename);

        // Draws the vbo including linking the model matrix
        void draw(int wireFrame, GShaderHandle_T handles);

			  // Draws just the VBO and activating the texture
        void drawArray(int wireFrame);

			  // Override the texture handle seperately
			  void loadTextureHandle(GShaderVariableHandle * handle);

			  // Sets the texture
			  void setTex(GLuint tex);

			  // Get the model matrix
			  glm::mat4 getModelMat();
        
      	GLuint
				  m_vao,
				  m_buffer,
          m_tex = 0;
			  int
				  m_dataSize = 0;

			  glm::vec3
				  m_rotation = glm::vec3(0, 1, 0),
				  m_preRotation = glm::vec3(0, 1, 0),
				  m_scale = glm::vec3(1, 1, 1),
				  m_pos, m_vel;
			  GLfloat
				  m_theta,
				  m_preTheta;
      };

      typedef std::vector<GVertexBufferObject> GArrayVBO;
    }
  }
}
