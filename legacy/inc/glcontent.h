#pragma once

#include "opengl.h"
#include "glm.h"
#include "GLCamera.h"
#include "KeyboardEvents.h"
#include <chrono>
#include <thread>




namespace gfx
{
	//Error callback  
	static void		error_callback(int error, const char* description)
	{
		fputs(description, stderr);
		_fgetchar();
	}

	namespace engine
	{
		typedef void(*GLContentLoop)();
		typedef void(*GLContentInit)();

		class GLContent
		{
		public:
			void run(GLContentLoop loop, GLContentInit init, GLFWkeyfun key_func, GLFWmousebuttonfun mouse_func);

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

			void setClearColor(glm::vec3 color);

			void setClearColor(glm::vec4 color);

			void clearAlpha();
			void clearDepthBuffer();
			void clearColorBuffer();
			void clearColor();
			void clearBuffers();
			void clearAll();

			glm::vec2 getWindowSize();

			void setEyePos(glm::vec3 pos);

			void setEyeLookPos(glm::vec3 pos);

			void setUp(glm::vec3 up);

			void setCamera(gfx::engine::GLCamera * camera);

			float getIsometricDepth();

			void setIsometricDepth(float depth);

			glm::vec2 getMousePos();
			glm::vec2 getMouseDelta();

			alib::KeyboardEvents * getKeyboardEvents();

			int getFrames()
			{
				return m_frames;
			}

		private:
			glm::mat4 getExternalOrtho();
			glm::mat4 getExternalOrthoView();

			glm::mat4 getOrtho();
			glm::mat4 getOrthoView();

			glm::mat4 getOrthoIsometric();
			glm::mat4 getOrthoIsometricView();

			glm::mat4 getPseudoIsometric();
			glm::mat4 getPseudoIsometricView();

			glm::mat4 getPerspective();
			glm::mat4 getPerspectiveView();

			glm::mat4 getHyperPerspective();
			glm::mat4 getHyperPerspectiveView();


			//GL graphics loop
			void			glLoop(GLContentLoop loop, GLFWwindow * window);
			//GL window initialise
			GLFWwindow *				initWindow(gfx::engine::GLContentInit init, GLFWkeyfun key_func, GLFWmousebuttonfun mouse_func);

			void handleKeyEvent();
		public:
			//constructors

			GLContent();

			GLContent(glm::vec3 windowSize, glm::vec3 eyePos, glm::vec3 eyeLookPos, glm::vec3 up, float fov, float aspectRatio, float nearZ, float farZ);

			char m_keyDown = NULL;

		private:
			GLFWwindow * m_window;

			glm::vec2
				m_windowSize = glm::vec2(800, 800);

			glm::vec3
				m_eyePos = glm::vec3(0, 0, -5),
				m_eyeLookPos,
				m_up = glm::vec3(0, 1, 0);

			glm::vec4 m_clearColor;

			glm::mat4
				m_model,
				m_view,
				m_projection;

			float
				m_fov = 45.0f,
				m_aspectRatio = m_windowSize.x / m_windowSize.y,
				m_nearZ = 0.1f,
				m_farZ = 1000.0f,
				m_isometricDepth = 1.0f;

			alib::KeyboardEvents m_keyboard;

			glm::vec2 m_oldMousPos, m_newMousePos;

			int m_frames = 0;

			
		};

	}
}





