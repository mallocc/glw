#pragma once

#include "opengl.h"
#include "glm.h"
#include "GReturnCode.h"
#include "GCamera.h"


using glw::GReturnCode;
using glw::engine::GCamera;


namespace glw
{
  namespace engine
  {
    //Error callback  
	  static void		error_callback(int error, const char* description);

		typedef GReturnCode(*GEngineLoop)();
		typedef GReturnCode(*GEngineInit)();

    class GEngine
    {
    public:
      
      GEngine();
      
      GEngine(
        const glm::vec3& windowSize, 
        const glm::vec3& eyePos, 
        const glm::vec3& eyeLookPos, 
        const glm::vec3& up, 
        const float fov, 
        const float aspectRatio, 
        const float nearZ, 
        const float farZ);
      
      ~GEngine();
      
      GReturnCode run(
        GEngineLoop loop, 
        GEngineInit init, 
        GLFWkeyfun key_func, 
        GLFWmousebuttonfun mouse_func);

			void loadPerspective();
			void loadExternalOrtho();
			void loadOrtho();
			void loadPseudoIsometric();
			void loadHyperPerspective();

      glm::vec3 * getEyePos();
			glm::vec3 * getEyeLookPos();
			glm::vec3 * getUp();

			glm::mat4 * getModelMat();
			glm::mat4 * getViewMat();
			glm::mat4 * getProjMat();
			
			void setClearColor(const glm::vec3& color);
			void setClearColor(const glm::vec4& color);

			void clearAlpha();
			void clearDepthBuffer();
			void clearColorBuffer();
			void clearColor();
			void clearBuffers();
			void clearAll();
			
			void getWindowSize(glm::vec2& windowSize) const;

      void setEyePos(const glm::vec3& pos);
			void setEyeLookPos(const glm::vec3& pos);
			void setUp(const glm::vec3& up);
			
			void setCamera(GCamera& camera);
			
			float getIsometricDepth() const;
			void setIsometricDepth(const float depth);
			
			int getFrames() const;
			
			void setPrintFps(bool print);
			void setFpsCap(int fpsCap);
			int getFpsCap();
			
    private:
      glm::mat4 getExternalOrtho() const;
		  glm::mat4 getExternalOrthoView() const;

		  glm::mat4 getOrtho() const;
		  glm::mat4 getOrthoView() const;

		  glm::mat4 getOrthoIsometric() const;
		  glm::mat4 getOrthoIsometricView() const;

		  glm::mat4 getPseudoIsometric() const;
		  glm::mat4 getPseudoIsometricView() const;

		  glm::mat4 getPerspective() const;
		  glm::mat4 getPerspectiveView() const;

		  glm::mat4 getHyperPerspective() const;
		  glm::mat4 getHyperPerspectiveView() const;

		  //GL graphics loop
		  GReturnCode glLoop(GEngineLoop loop, GLFWwindow * window);
		  //GL window initialise
		  GReturnCode initWindow(
		    glw::engine::GEngineInit init, 
		    GLFWkeyfun key_func, 
		    GLFWmousebuttonfun mouse_func,
		    GLFWwindow * window);

  
		  GLFWwindow * m_window;

		  glm::vec2 m_windowSize;

		  glm::vec3 m_eyePos;
			glm::vec3 m_eyeLookPos;
			glm::vec3 m_up;

		  glm::vec4 m_clearColor;

		  glm::mat4 m_model;
		  glm::mat4 m_view;
		  glm::mat4 m_projection;

      float m_fov;
      float m_aspectRatio;
      float m_nearZ;
      float m_farZ;
      float m_isometricDepth;

		  glm::vec2 m_oldMousPos;
      glm::vec2 m_newMousePos;

		  int m_frames = 0;
		  
		  bool m_printFps = false;
		  int m_fpsCap = 62;
    };
  }
}
