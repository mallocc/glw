#pragma once

#include "opengl.h"
#include "glm.h"
#include "GReturnCode.h"
#include "GShaderProgram.h"
#include "GVertexBufferObject.h"
#include "GFrameBufferObject.h"


using glw::GReturnCode;
using glw::engine::glsl::GShaderProgram;
using glw::engine::glsl::GShaderProgramId;
using glw::engine::buffers::GVertexBufferObject;
using glw::engine::buffers::GFrameBufferObject;


namespace glw
{
  namespace engine
  {
    namespace glsl
    {
      class GShaderProgramManager
      {
      public:
        
        GShaderProgramManager();
        
        ~GShaderProgramManager();
        
        GShaderProgram * loadProgram(GShaderProgramId id);

			  GReturnCode addNewProgram(
				  const char * vertex_file_path,
				  const char * fragment_file_path
			  );
			  
			  GReturnCode addNewProgram(
				  const char * vertex_file_path,
				  const char * fragment_file_path,
				  glm::mat4 * model_data,
				  glm::mat4 * view_data,
				  glm::mat4 * proj_data,
				  GShaderProgramId& shaderProgramId
			  );

			  GReturnCode addNewProgram(GShaderProgram program);

			  GShaderProgram * getProgram(GShaderProgramId id);

			  GShaderProgram * getCurrentProgram();

        void drawVBO(GVertexBufferObject& vbo);

        void drawVBOs(std::vector<GVertexBufferObject*>& vbos);
        void drawVBOs(std::vector<GVertexBufferObject>& vbos);

        void drawFBO(GFrameBufferObject& fbo);

      private:
        std::map<GShaderProgramId, GShaderProgram> m_shaderPrograms;
			  GShaderProgramId m_currentProgram;
      };
    }
  }
}
