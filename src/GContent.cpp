#include "GContent.h"
#include "Logger.h"
#include "GReturnCode.h"
#include <chrono>
#include <thread>
#include "GCamera.h"
#include "StringFormat.h"
#include <sstream>

using glw::engine::GContent;
using glw::engine::GContentLoop;
using glw::engine::GContentInit;
using glw::GReturnCode::GLW_SUCCESS;
using glw::GReturnCode::GLW_FAIL;
using glw::engine::GCamera;
using util::StringFormat;

namespace
{
  const char * TRG = "GCON";
  const char * __CLASSNAME__ = "GContent";
}

//Error callback  
void glw::engine::error_callback(int error, const char* description)
{
  LERROR(TRG, description, __FILE__, __LINE__, "GLFW", __func__);
}


GContent::GContent()
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
  LINFO(TRG, "Constructing default GContent.");
}

GContent::GContent(
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
  LINFO(TRG, "Constructing GContent.");
}


GContent::~GContent()
{
}

GReturnCode GContent::run(
    GContentLoop loop,
    GContentInit init,
    GLFWkeyfun key_callback,
    GLFWmousebuttonfun mouse_button_callback)
{
  GReturnCode success = GLW_SUCCESS;
  
  if (NULL == loop)
  {
    success = GLW_FAIL;
    LERROR(TRG, "GContentLoop is NULL. GContent is pointless without specifing a loop function.",
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }
  if (NULL == init)
  {
    LWARNING(TRG, "GContentInit is NULL.",
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }
  if (NULL == (m_keyfunc = key_callback))
  {
    LWARNING(TRG, "GLFWkeyfun is NULL.",
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }
  if (NULL == (m_mousebuttonfunc = mouse_button_callback))
  {
    LWARNING(TRG, "GLFWmousebuttonfun is NULL.", 
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }

  
  if (GLW_SUCCESS == success)
  {
    if (GLW_SUCCESS == initWindow(init, m_window))
    {
      LINFO(TRG, "Initialised content succesfully.");
    }
    else
    {
      success = GLW_FAIL;
      LERROR(TRG, "Failed to initialise content.",
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
    LINFO(TRG, "GContent terminated succesfully.");
  }
  else
  {
    LWARNING(TRG, "GContent terminated with errors, check logs for trace.",
        __FILE__, __LINE__, __CLASSNAME__, __func__);
  }
  
  return success;
}

void GContent::load3DPerspective()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	m_view = getPerspectiveView();
	m_projection = getPerspective();
}

void GContent::loadExternalOrtho()
{
	// Enable depth test
	glDisable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glDisable(GL_CULL_FACE);

	m_view = getExternalOrthoView();
	m_projection = getExternalOrtho();
}

void GContent::loadOrtho()
{
	// Enable depth test
  glDisable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
  glDisable(GL_CULL_FACE);

	m_view = getOrthoView();
	m_projection = getOrtho();
}

void GContent::loadPseudoIsometric()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	m_view = getPseudoIsometricView();
	m_projection = getPseudoIsometric();
}

void GContent::loadHyperPerspective()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	m_view = getHyperPerspectiveView();
	m_projection = getHyperPerspective();
}

glm::vec3 * GContent::getEyePos()
{
  return &m_eyePos;
}

glm::vec3 * GContent::getEyeLookPos()
{
  return &m_eyeLookPos;
}

glm::vec3 * GContent::getUp()
{
  return &m_up;
}

glm::mat4 * GContent::getModelMat()
{
  return &m_model;
}

glm::mat4 * GContent::getViewMat()
{
  return &m_view;
}

glm::mat4 * GContent::getProjMat()
{
  return &m_projection;
}

void GContent::setClearColor(const glm::vec3& color)
{
  m_clearColor = glm::vec4(color, 1.0f);
}

void GContent::setClearColor(const glm::vec4& color)
{
  m_clearColor = color;
}

void GContent::clearAlpha()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void GContent::clearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GContent::clearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GContent::clearColor()
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
}

void GContent::clearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GContent::clearAll()
{
  clearColor();
	clearBuffers();
}

void GContent::getWindowSize(glm::vec2& windowSize) const
{
  windowSize = m_windowSize;
}

glm::vec2 GContent::getWindowSize() const
{
  return m_windowSize;
}

void GContent::setWindowSize(glm::vec2 windowSize)
{
  m_windowSize = windowSize;
  m_aspectRatio = windowSize.x/windowSize.y;
}

void GContent::setEyePos(const glm::vec3& pos)
{
  m_eyePos = pos;
}

void GContent::setEyeLookPos(const glm::vec3& pos)
{
  m_eyeLookPos = pos;
}

void GContent::setUp(const glm::vec3& up)
{
  m_up = up;
}

void GContent::setCamera(GCamera& camera)
{
	m_eyePos = camera.getPosition();
	m_eyeLookPos = camera.getLookPosition();
	m_up = camera.getUp();
}

float GContent::getIsometricDepth() const
{
  return m_isometricDepth;
}

void GContent::setIsometricDepth(const float depth)
{
  m_isometricDepth = depth;
}

int GContent::getFrames() const
{
  return m_frames;
}

void GContent::setPrintFps(bool print)
{
  m_printFps = print;
}

void GContent::setFpsCap(int fpsCap)
{
  m_fpsCap = fpsCap;
}

int GContent::getFpsCap()
{
  return m_fpsCap;
}

float GContent::getFps()
{
  return m_fps;
}

GKeyboard * GContent::getKeyboard()
{
  return &m_keyboard;
}
GMouse * GContent::getMouse()
{
  return &m_mouse;
}

GLFWkeyfun GContent::getKeyfunc()
{
  return m_keyfunc;
}
GLFWmousebuttonfun GContent::getMousebuttonfunc()
{
  return m_mousebuttonfunc;
}

void GContent::exit()
{
  glfwSetWindowShouldClose(m_window, GL_TRUE);
}

glm::mat4 GContent::getExternalOrtho() const
{
	return glm::ortho(
		0.0f,
		m_windowSize.x,		
		m_windowSize.y,
		0.0f,
		-m_farZ, m_farZ);
	//return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}

glm::mat4 GContent::getExternalOrthoView() const
{
	return glm::mat4(1.0f);
	/*return glm::lookAt(
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 57.28996f * window_size.x),
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 0),
	glm::vec3(0, 1, 0));*/
}

glm::mat4 GContent::getOrtho() const
{
	return glm::ortho(
		0.0f + m_eyePos.x - m_windowSize.x / 2.0f,
		m_windowSize.x + m_eyePos.x - m_windowSize.x / 2.0f,
		0.0f + m_eyePos.y - m_windowSize.y / 2.0f,
		m_windowSize.y + m_eyePos.y - m_windowSize.y / 2.0f,
		-m_farZ, m_farZ);
	//return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}

glm::mat4 GContent::getOrthoView() const
{
	return glm::mat4(1.0f);
	/*return glm::lookAt(
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 57.28996f * window_size.x),
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 0),
	glm::vec3(0, 1, 0));*/
}

glm::mat4 GContent::getOrthoIsometric() const
{
	return glm::ortho(0.0f, m_windowSize.x, 0.0f, m_windowSize.y, -m_farZ, m_farZ);
	//return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}

glm::mat4 GContent::getOrthoIsometricView() const
{
	return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}

glm::mat4 GContent::getPseudoIsometric() const
{
	return glm::perspective(glm::radians(2.864f), m_aspectRatio, m_nearZ, m_farZ);
}

glm::mat4 GContent::getPseudoIsometricView() const
{
	return glm::lookAt(m_eyePos + (m_isometricDepth * 20.0f) * glm::vec3(1, 1, 1), m_eyePos, m_up);
}

glm::mat4 GContent::getPerspective() const
{
	return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearZ, m_farZ);
}

glm::mat4 GContent::getPerspectiveView() const
{
	return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}

glm::mat4 GContent::getHyperPerspective() const
{
	return glm::perspective(glm::radians(160.0f), m_aspectRatio, m_nearZ, m_farZ);
}

glm::mat4 GContent::getHyperPerspectiveView() const
{
	return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}


//GL graphics loop
GReturnCode GContent::glLoop(
        glw::engine::GContentLoop loop,
        GLFWwindow * window)
{
	LINFO(TRG, "Running GL loop...");
	LINFO(TRG, "==================");
	
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

    getMouse()->updateMouse();
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
    m_fps = 1000/totalFrameMs;
    if(m_printFps && !(m_frames % (int)m_fps))
		{
      LINFO(TRG, StringFormat("FPS %0").arg(m_fps).str());
		}
	} //Check if the ESC or Q key had been pressed or if the window had been closed  
	while (!glfwWindowShouldClose(window));

	LINFO(TRG, "==================");
	LINFO(TRG, "Window has closed. Application will now exit.");

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();

  return GLW_SUCCESS;
}

//GL window initialise
GReturnCode GContent::initWindow(
    glw::engine::GContentInit init,
    GLFWwindow * window)
{
  GReturnCode success = GLW_SUCCESS;

  LINFO(TRG, "Initialising new GContent...");

	//Set the error callback  
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		LERROR(TRG, "Failed to init GLFW", __FILE__, __LINE__, __CLASSNAME__, __func__);
	  success = GLW_FAIL;
	}
	else
	{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		LINFO(TRG, "GLFW Initialised.");
	}
	
	if(GLW_SUCCESS == success)	
	{
	  //Create a window and create its OpenGL context  
	  window = glfwCreateWindow(m_windowSize.x, m_windowSize.y, "Test Window", NULL, NULL);
	  //If the window couldn't be created  
	  if (!window)
	  {
		  LERROR(TRG, "Failed to open GLFW", __FILE__, __LINE__, __CLASSNAME__, __func__);
		  glfwTerminate();
      success = GLW_FAIL;
	  }
	  m_window = window;
	  LINFO(TRG, "Created GLFW window.");


	  //This function makes the context of the specified window current on the calling thread.   
	  glfwMakeContextCurrent(window);
	  LINFO(TRG, "GLFW window context set.");

	  //Sets the key callback  
    m_keyboard = GKeyboard(window, KEY_CALLBACK, CHARACTER_CALLBACK);
    m_mouse = GMouse(window, MOUSE_BUTTON_CALLBACK, CURSOR_POSITION_CALLBACK, SCROLL_CALLBACK);

	}
	
	if (GLW_SUCCESS == success)
  {
	  //Initialize GLEW  
	  GLenum err = glewInit();
	  //If GLEW hasn't initialized  
	  if (err != GLEW_OK)
	  {
		  LERROR(TRG, "Failed to to init GLEW.", __FILE__, __LINE__, __CLASSNAME__, __func__);
		  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		  glfwTerminate();
      success = GLW_FAIL;
	  }
	  LINFO(TRG, "GLEW Initialised.");

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
	}

  if (GLW_SUCCESS == success)
  {
    // init
    if (NULL != init)
    {
      LINFO(TRG, "GContent initialisation started...");
      if (GLW_SUCCESS == init())
      {
        LINFO(TRG, "GContent initialisation succesful.");
      }
      else
      {
        success = GLW_FAIL;
        LERROR(TRG, "GContent initialisation failed.",
            __FILE__, __LINE__, __CLASSNAME__, __func__);
      }
    }
  }
	
	return success;
}
