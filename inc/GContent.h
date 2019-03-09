#pragma once

#include "opengl.h"
#include "glm.h"
#include "GReturnCode.h"
#include "GCamera.h"
#include "GKeyboard.h"
#include "GMouse.h"

#include <string>

#include <stdio.h>

using glw::GReturnCode;
using glw::engine::GCamera;
using glw::engine::GKeyboard;
using glw::engine::GMouse;


namespace glw
{
  namespace engine
  {
    //Error callback  
    static void error_callback(int error, const char* description);

    typedef GReturnCode(*GContentLoop)();
    typedef GReturnCode(*GContentInit)();

    class GContent
    {
    public:
      
      static GContent& getInstance()
      {
          static GContent instance;
          return instance;
      }

      static GContent * getInstancePtr()
      {
          static GContent instance;
          return &instance;
      }

      static void KEY_CALLBACK(GLFWwindow* window, int key, int scancode, int action, int mods)
      {
        if(GLFW_PRESS == action || GLFW_REPEAT == action)
          glw::engine::GContent::getInstancePtr()->getKeyboard()->updateKey(key);
        (*glw::engine::GContent::getInstancePtr()->getKeyfunc())(window, key, scancode, action, mods);
      }

      static void CHARACTER_CALLBACK(GLFWwindow* window, unsigned int codepoint)
      {
        glw::engine::GContent::getInstancePtr()->getKeyboard()->updateChar(codepoint);
      }

      static void CURSOR_POSITION_CALLBACK(GLFWwindow* window, double xpos, double ypos)
      {
//        glw::engine::GContent::getInstancePtr()->getMouse()->updatePosition(xpos, ypos, true);
      }

      static void SCROLL_CALLBACK(GLFWwindow* window, double xoffset, double yoffset)
      {
        glw::engine::GContent::getInstancePtr()->getMouse()->updateScroll(xoffset, yoffset);
      }

      static void MOUSE_BUTTON_CALLBACK(GLFWwindow* window, int button, int action, int mods)
      {
        (*glw::engine::GContent::getInstancePtr()->getMousebuttonfunc())(window, button, action, mods);
      }

      GContent();
      
      GContent(
        const glm::vec3& windowSize, 
        const glm::vec3& eyePos, 
        const glm::vec3& eyeLookPos, 
        const glm::vec3& up, 
        const float fov, 
        const float aspectRatio, 
        const float nearZ, 
        const float farZ);
      
      ~GContent();
      
      GReturnCode run(
          GContentLoop loop,
          GContentInit init = NULL,
          GLFWkeyfun key_callback = NULL,
          GLFWmousebuttonfun mouse_button_callback = NULL);

      void load3DPerspective();
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
      void setWindowSize(glm::vec2 windowSize);

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

      float getFps();

      GKeyboard * getKeyboard();
      GMouse * getMouse();

      GLFWkeyfun getKeyfunc();
      GLFWmousebuttonfun getMousebuttonfunc();

      void exit();

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
      GReturnCode glLoop(GContentLoop loop, GLFWwindow * window);
		  //GL window initialise
		  GReturnCode initWindow(
          glw::engine::GContentInit init,
          GLFWwindow * window);

      // Attributes //

		  GLFWwindow * m_window;

      GLFWkeyfun m_keyfunc;
      GLFWmousebuttonfun m_mousebuttonfunc;

      GKeyboard m_keyboard;
      GMouse m_mouse;

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
      float m_fps = 0;

    };
  }
}


