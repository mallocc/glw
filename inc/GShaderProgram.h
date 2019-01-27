#pragma once

#include "opengl.h"
#include "glm.h"

#include "GShaderVariableHandle.h"
#include "GMeshShaderHandle_T.h"

#include <map>

#define NULL_SHADER_PROGRAM_ID  255
#define VAR_LIGHT_POSITION      "u_light_pos"
#define VAR_LIGHT_COLOR         "u_light_color"
#define VAR_LIGHT_PROPERTIES    "u_light_properties"
#define VAR_EYE_POSITION        "u_eye_pos"
#define VAR_AMBIENT_COLOR       "u_ambient_color"
#define VAR_TEXTURE             "u_tex"
#define VAR_MATRIX_MODEL        "u_m"
#define VAR_TEXTURE_VIEW        "u_v"
#define VAR_TEXTURE_PROJECTION  "u_p"

using glw::engine::glsl::GShaderVariableHandle;
using glw::engine::glsl::GMeshShaderHandle_T;


namespace glw
{
  namespace engine
  {
    namespace glsl
    {
      typedef GLuint GShaderProgramId;
    
      class GShaderProgram
      {
      public:
        
        GShaderProgram();
        GShaderProgram(const char * vertexFilePath,
                       const char * fragmentFilePath);
        
        ~GShaderProgram();
        
        GReturnCode addHandle(GShaderVariableHandle handle);

			  void load();

			  GShaderVariableHandle * getHandle(GShaderVariableHandleId id);

			  GShaderProgramId getId();

			  GReturnCode setModelMat4Handle(glm::mat4 * mat);

			  GReturnCode setViewMat4Handle(glm::mat4 * mat);

			  GReturnCode setProjMat4Handle(glm::mat4 * mat);

			  GReturnCode setColorHandle();

			  GReturnCode setFlagHandle();

			  GReturnCode setTexHandle();
			  GReturnCode setTex1Handle();

			  GShaderVariableHandle * getModelMat4Handle();

			  GShaderVariableHandle * getViewMat4Handle();

			  GShaderVariableHandle * getProjMat4Handle();

			  GShaderVariableHandle * getColorHandle();

			  GShaderVariableHandle * getFlagHandle();

			  GShaderVariableHandle * getTexHandle();

			  GShaderVariableHandle * getTex1Handle();

			  GMeshShaderHandle_T getMeshShaderHandle();
			  void getMeshShaderHandle(GMeshShaderHandle_T& meshShaderHandle);
			  
			  bool isValid() const;

      private:
        bool m_valid;
      
        GShaderProgramId m_id;

				GLuint m_vertexShaderId;
				GLuint m_fragmentShaderId;

				const char * m_vertexFilePath;
				const char * m_fragmentFilePath;

				std::map<GShaderVariableHandleId, GShaderVariableHandle> m_handles;

				GShaderVariableHandleId
            m_modelMat, m_viewMat, m_projMat,
            m_color,
            m_flag,
            m_tex, m_tex1;
      };
    }
  }
}
