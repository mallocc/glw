#include "GEngine.h"
#include "CLog.h"
#include "GReturnCode.h"
#include <chrono>
#include <thread>
#include "GCamera.h"
#include "StringFormat.h"

using glw::engine::GEngine;
using glw::GReturnCode::GLW_SUCCESS;
using glw::GReturnCode::GLW_FAIL;
using glw::engine::GCamera;
using util::StringFormat;

namespace
{
  const char * TRG = "GENG";
  const char * __CLASSNAME__ = "GEngine";
}

//Error callback  
void glw::engine::error_callback(int error, const char* description)
{
  CERROR(TRG, description, __FILE__, __LINE__, "NULL", __func__);
}


GEngine::GEngine()
        :
        m_windowSize(glm::vec2(800, 800)),
        m_eyePos(glm::vec3(0, 0, -5)),
        m_eyeLookPos(glm::vec3()),
        m_up(glm::vec3(0, 1, 0)),
        m_fov(45.0f),
        m_aspectRatio(m_windowSize.x / m_windowSize.y),
        m_nearZ(0.1f),
        m_farZ(1000.0f),
        m_isometricDepth(1.0f),
        m_frames(0)
{
  CINFO(TRG, "Constructing default GEngine.");
}

GEngine::GEngine(
        const glm::vec3& windowSize, 
        const glm::vec3& eyePos, 
        const glm::vec3& eyeLookPos, 
        const glm::vec3& up, 
        const float fov, 
        const float aspectRatio, 
        const float nearZ, 
        const float farZ) 
        :
        m_windowSize(windowSize),
        m_eyePos(eyePos),
        m_eyeLookPos(eyeLookPos),
        m_up(up),
        m_fov(fov),
        m_aspectRatio(aspectRatio),
        m_nearZ(nearZ),
        m_farZ(farZ),
        m_isometricDepth(1.0f),
        m_frames(0)
{
  CINFO(TRG, "Constructing GEngine.");
}


GEngine::~GEngine()
{
}

GReturnCode GEngine::run(
        GEngineLoop loop, 
        GEngineInit init,
        GLFWkeyfun key_func, 
        GLFWmousebuttonfun mouse_func)
{
  GReturnCode success = GLW_SUCCESS;
  
  if (NULL == loop)
  {
    success = GLW_FAIL;
    CERROR(TRG, "GEngineLoop is NULL. GEngine is pointless without specifing a loop function.",
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }
  if (NULL == init)
  {
    CWARNING(TRG, "GEngineInit is NULL.", 
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }
  if (NULL == key_func)
  {
    CWARNING(TRG, "GLFWkeyfun is NULL.",
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }
  if (NULL == mouse_func)
  {
    CWARNING(TRG, "GLFWmousebuttonfun is NULL.", 
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }
  
  if (GLW_SUCCESS == success)
  {
    if (GLW_SUCCESS == initWindow(init, key_func, mouse_func, m_window))
    {
      CINFO(TRG, "Initialised content succesfully.");
    }
    else
    {
      success = GLW_FAIL;
      CERROR(TRG, "Failed to initialise content.",
          __FILE__, __LINE__, __CLASSNAME__, __func__);
    }
  }
  
  if (GLW_SUCCESS == success)
  {
    if (GLW_SUCCESS != glLoop(loop, m_window))
    {
      success = GLW_FAIL;
    } 
  }
  
  if (GLW_SUCCESS == success)
  {
    CINFO(TRG, "GEngine terminated succesfully.");
  }
  else
  {
    CWARNING(TRG, "GEngine terminated with errors, check logs for trace.", 
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }
  
  return success;
}

void GEngine::loadPerspective()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	m_view = getPerspectiveView();
	m_projection = getPerspective();
}

void GEngine::loadExternalOrtho()
{
	// Enable depth test
	glDisable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glDisable(GL_CULL_FACE);

	m_view = getExternalOrthoView();
	m_projection = getExternalOrtho();
}

void GEngine::loadOrtho()
{
	// Enable depth test
	glDisable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glDisable(GL_CULL_FACE);

	m_view = getOrthoView();
	m_projection = getOrtho();
}

void GEngine::loadPseudoIsometric()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	m_view = getPseudoIsometricView();
	m_projection = getPseudoIsometric();
}

void GEngine::loadHyperPerspective()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	m_view = getHyperPerspectiveView();
	m_projection = getHyperPerspective();
}

glm::vec3 * GEngine::getEyePos()
{
  return &m_eyePos;
}

glm::vec3 * GEngine::getEyeLookPos()
{
  return &m_eyeLookPos;
}

glm::vec3 * GEngine::getUp()
{
  return &m_up;
}

glm::mat4 * GEngine::getModelMat()
{
  return &m_model;
}

glm::mat4 * GEngine::getViewMat()
{
  return &m_view;
}

glm::mat4 * GEngine::getProjMat()
{
  return &m_projection;
}

void GEngine::setClearColor(const glm::vec3& color)
{
  m_clearColor = glm::vec4(color, 1.0f);
}

void GEngine::setClearColor(const glm::vec4& color)
{
  m_clearColor = color;
}

void GEngine::clearAlpha()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void GEngine::clearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GEngine::clearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GEngine::clearColor()
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
}

void GEngine::clearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GEngine::clearAll()
{
  clearColor();
	clearBuffers();
}

void GEngine::getWindowSize(glm::vec2& windowSize) const
{
  windowSize = m_windowSize;
}

void GEngine::setEyePos(const glm::vec3& pos)
{
  m_eyePos = pos;
}

void GEngine::setEyeLookPos(const glm::vec3& pos)
{
  m_eyeLookPos = pos;
}

void GEngine::setUp(const glm::vec3& up)
{
  m_up = up;
}

void GEngine::setCamera(GCamera& camera)
{
	m_eyePos = camera.getPosition();
	m_eyeLookPos = camera.getLookPosition();
	m_up = camera.getUp();
}

float GEngine::getIsometricDepth() const
{
  return m_isometricDepth;
}

void GEngine::setIsometricDepth(const float depth)
{
  m_isometricDepth = depth;
}

int GEngine::getFrames() const
{
  return m_frames;
}

void GEngine::setPrintFps(bool print)
{
  m_printFps = print;
}

void GEngine::setFpsCap(int fpsCap)
{
  m_fpsCap = fpsCap;
}

int GEngine::getFpsCap()
{
  return m_fpsCap;
}

glm::mat4 GEngine::getExternalOrtho() const
{
	return glm::ortho(
		0.0f,
		m_windowSize.x,		
		m_windowSize.y,
		0.0f,
		-m_farZ, m_farZ);
	//return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}

glm::mat4 GEngine::getExternalOrthoView() const
{
	return glm::mat4(1.0f);
	/*return glm::lookAt(
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 57.28996f * window_size.x),
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 0),
	glm::vec3(0, 1, 0));*/
}

glm::mat4 GEngine::getOrtho() const
{
	return glm::ortho(
		0.0f + m_eyePos.x - m_windowSize.x / 2.0f,
		m_windowSize.x + m_eyePos.x - m_windowSize.x / 2.0f,
		0.0f + m_eyePos.y - m_windowSize.y / 2.0f,
		m_windowSize.y + m_eyePos.y - m_windowSize.y / 2.0f,
		-m_farZ, m_farZ);
	//return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}

glm::mat4 GEngine::getOrthoView() const
{
	return glm::mat4(1.0f);
	/*return glm::lookAt(
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 57.28996f * window_size.x),
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 0),
	glm::vec3(0, 1, 0));*/
}

glm::mat4 GEngine::getOrthoIsometric() const
{
	return glm::ortho(0.0f, m_windowSize.x, 0.0f, m_windowSize.y, -m_farZ, m_farZ);
	//return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}

glm::mat4 GEngine::getOrthoIsometricView() const
{
	return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}

glm::mat4 GEngine::getPseudoIsometric() const
{
	return glm::perspective(glm::radians(2.864f), m_aspectRatio, m_nearZ, m_farZ);
}

glm::mat4 GEngine::getPseudoIsometricView() const
{
	return glm::lookAt(m_eyePos + (m_isometricDepth * 20.0f) * glm::vec3(1, 1, 1), m_eyePos, m_up);
}

glm::mat4 GEngine::getPerspective() const
{
	return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearZ, m_farZ);
}

glm::mat4 GEngine::getPerspectiveView() const
{
	return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}

glm::mat4 GEngine::getHyperPerspective() const
{
	return glm::perspective(glm::radians(160.0f), m_aspectRatio, m_nearZ, m_farZ);
}

glm::mat4 GEngine::getHyperPerspectiveView() const 
{
	return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}


//GL graphics loop
GReturnCode GEngine::glLoop(
        glw::engine::GEngineLoop loop, 
        GLFWwindow * window)
{
	CINFO(TRG, "Running GL loop...");
	CINFO(TRG, "==================");
	

	
	//Main Loop  
	do
	{
		m_frames++;

		// start clock for this tick
		auto start = std::chrono::high_resolution_clock::now();

	  glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (NULL != loop)
    {
      loop();
    }

		//Swap buffers  
		glfwSwapBuffers(window);
		
		glfwPollEvents();

		// stop clock
		auto finish = std::chrono::high_resolution_clock::now();
		int frameMs = float(std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count());	
	  
	  int timeCapMs = 1000/m_fpsCap;	
		long leftOverWaitMs = timeCapMs - frameMs;
		leftOverWaitMs = leftOverWaitMs < 0 ? 0 : leftOverWaitMs;
		
		// throttle the graphics loop to cap at a certain fps
		std::this_thread::sleep_for(std::chrono::milliseconds(leftOverWaitMs));
		
		// log fps
		int totalFrameMs = frameMs+leftOverWaitMs+1;
    if(m_printFps && !(m_frames % (1000/totalFrameMs)))
		{
		  CINFO(TRG, StringFormat("FPS %0").arg(1000/totalFrameMs).str());
		}
	} //Check if the ESC or Q key had been pressed or if the window had been closed  
	while (!glfwWindowShouldClose(window));

	CINFO(TRG, "==================");
	CINFO(TRG, "Window has closed. Application will now exit.");

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();

  return GLW_SUCCESS;
}

//GL window initialise
GReturnCode GEngine::initWindow(
        glw::engine::GEngineInit init, 
        GLFWkeyfun key_func, 
        GLFWmousebuttonfun mouse_func,
        GLFWwindow * window)
{
  GReturnCode success = GLW_SUCCESS;

	CINFO(TRG, "Initialising new GEngine...");

	//Set the error callback  
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		CERROR(TRG, "Failed to init GLFW", __FILE__, __LINE__, __CLASSNAME__, __func__);
	  success = GLW_FAIL;
	}
	else
	{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		CINFO(TRG, "GLFW Initialised.");
	}
	
	if(GLW_SUCCESS == success)	
	{
	  //Create a window and create its OpenGL context  
	  window = glfwCreateWindow(m_windowSize.x, m_windowSize.y, "Test Window", NULL, NULL);
	  //If the window couldn't be created  
	  if (!window)
	  {
		  CERROR(TRG, "Failed to open GLFW", __FILE__, __LINE__, __CLASSNAME__, __func__);
		  glfwTerminate();
      success = GLW_FAIL;
	  }
	  m_window = window;
	  CINFO(TRG, "Created GLFW window.");


	  //This function makes the context of the specified window current on the calling thread.   
	  glfwMakeContextCurrent(window);
	  CINFO(TRG, "GLFW window context set.");

	  //Sets the key callback  
	  glfwSetKeyCallback(window, key_func);
	  glfwSetMouseButtonCallback(window, mouse_func);
	}
	
	if (GLW_SUCCESS == success)
  {
	  //Initialize GLEW  
	  GLenum err = glewInit();
	  //If GLEW hasn't initialized  
	  if (err != GLEW_OK)
	  {
		  CERROR(TRG, "Failed to to init GLEW.", __FILE__, __LINE__, __CLASSNAME__, __func__);
		  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		  glfwTerminate();
      success = GLW_FAIL;
	  }
	  CINFO(TRG, "GLEW Initialised.");

	  // Enable depth test
	  glEnable(GL_DEPTH_TEST);
	  // Accept fragment if it closer to the camera than the former one
	  glDepthFunc(GL_LESS);
	  // Cull triangles which normal is not towards the camera
	  glEnable(GL_CULL_FACE);
	  // enable texturineg
	  glEnable(GL_TEXTURE_2D);

	  glEnable(GL_BLEND);

	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	  glClearDepth(1.0f);
	  
    // init
    if (NULL != init)
    {
      CINFO(TRG, "GEngine initialisation started...");  
      if (GLW_SUCCESS == init())
      {
        CINFO(TRG, "GEngine initialisation succesful.");        
      }
      else
      {
        success = GLW_FAIL;
        CERROR(TRG, "GEngine initialisation failed.", 
            __FILE__, __LINE__, __CLASSNAME__, __func__);
      }
    }
	}
	
	return success;
}

