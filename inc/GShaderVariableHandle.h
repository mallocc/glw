#pragma once

#include "opengl.h"
#include "glm.h"

#include "GReturnCode.h"

using glw::GReturnCode;

namespace glw
{
  namespace engine
  {
    namespace glsl
    {
      typedef GLuint GShaderVariableHandleId;
    
      class GShaderVariableHandle
      {
      public:
        
        GShaderVariableHandle();
        GShaderVariableHandle(const char * variableIdentifier);
        GShaderVariableHandle(const char * variableIdentifier, glm::mat4 * data);
        GShaderVariableHandle(const char * variableIdentifier, glm::vec3 * data);
        GShaderVariableHandle(const char * variableIdentifier, glm::vec4 * data);
        GShaderVariableHandle(const char * variableIdentifier, GLfloat   * data);
        GShaderVariableHandle(const char * variableIdentifier, GLuint    * data);
        GShaderVariableHandle(const char * variableIdentifier, int       * data);
        
        ~GShaderVariableHandle();
        
        GReturnCode init(const GLuint program);

		    void load();
		    void load(glm::mat4 data);
		    void load(glm::vec3 data);
		    void load(glm::vec4 data);
		    void load(GLfloat data);
		    void load(GLuint data);
		    void load(int data);

		    glm::mat4 * getMat4();
		    glm::vec3 * getVec3();
        glm::vec4 * getVec4();
        GLfloat   * getGLfloat();
        GLuint    * getGLuint();
        int       * getInt();

		    const char * getVariableIdentifier();		    
		    GShaderVariableHandleId getHandleId();

      private:

        GShaderVariableHandleId m_id;
        
        int               m_type;
        
        const char * m_variableIdentifier = "";
        
        glm::mat4  * m_data_m4    = NULL;
        glm::vec3  * m_data_v3    = NULL;
        glm::vec4  * m_data_v4    = NULL;
        GLfloat    * m_data_glf   = NULL;
        GLuint     * m_data_gli   = NULL;
        int        * m_data_i     = NULL;
      };
    }
  }
}
