#pragma once

#include "opengl.h"
#include "glm.h"
#include "GReturnCode.h"
#include "GShaderProgram.h"
#include "GMesh.h"
#include "GFrameBufferObject.h"


using glw::GReturnCode;
using glw::engine::glsl::GShaderProgram;
using glw::engine::glsl::GShaderProgramId;
using glw::engine::buffers::GMesh;
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

			  GReturnCode addProgram(
				  const char * vertex_file_path,
				  const char * fragment_file_path
			  );
			  
			  GReturnCode addProgram(
				  const char * vertex_file_path,
				  const char * fragment_file_path,
				  glm::mat4 * model_data,
				  glm::mat4 * view_data,
				  glm::mat4 * proj_data,
				  GShaderProgramId& shaderProgramId
			  );

			  GReturnCode addProgram(GShaderProgram program);

			  GShaderProgram * getProgram(GShaderProgramId id);

			  GShaderProgram * getCurrentProgram();

        void drawMesh(GMesh& mesh);

        void drawMeshes(std::vector<GMesh*>& meshes);

        void drawFrameBufferObject(GFrameBufferObject& fbo);

      private:
        std::map<GShaderProgramId, GShaderProgram> m_shaderPrograms;
			  GShaderProgramId m_currentProgram;
      };
    }
  }
}
