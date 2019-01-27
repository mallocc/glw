#include "GLContent.h"
#include "CLog.h"
#include "StringFormat.h"
#include "thread"
#include "KeyboardEvents.h"

using gfx::engine::GLContent;

namespace
{
	const char * CLASSNAME = "GLContent";
}

void GLContent::run(gfx::engine::GLContentLoop graphics_loop, gfx::engine::GLContentInit init, GLFWkeyfun key_func, GLFWmousebuttonfun mouse_func)
{
	std::thread(&alib::KeyboardEvents::run, m_keyboard).detach();
	glLoop(graphics_loop, initWindow(init, key_func, mouse_func));
}

void GLContent::loadPerspective()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	m_view = getPerspectiveView();
	m_projection = getPerspective();
}

void GLContent::loadExternalOrtho()
{
	// Enable depth test
	glDisable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glDisable(GL_CULL_FACE);

	m_view = getExternalOrthoView();
	m_projection = getExternalOrtho();
}

void GLContent::loadOrtho()
{
	// Enable depth test
	glDisable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glDisable(GL_CULL_FACE);

	m_view = getOrthoView();
	m_projection = getOrtho();
}

void GLContent::loadPseudoIsometric()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	m_view = getPseudoIsometricView();
	m_projection = getPseudoIsometric();
}

void GLContent::loadHyperPerspective()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	m_view = getHyperPerspectiveView();
	m_projection = getHyperPerspective();
}

glm::vec3 * GLContent::getEyePos()
{
	return &m_eyePos;
}

glm::vec3 * GLContent::getEyeLookPos()
{
	return &m_eyeLookPos;
}

glm::vec3 * GLContent::getUp()
{
	return &m_up;
}

glm::mat4 * GLContent::getModelMat()
{
	return &m_model;
}
glm::mat4 * GLContent::getViewMat()
{
	return &m_view;
}
glm::mat4 * GLContent::getProjMat()
{
	return &m_projection;
}

void GLContent::setClearColor(glm::vec3 color)
{
	m_clearColor = glm::vec4(color, 1.0f);
}

void GLContent::setClearColor(glm::vec4 color)
{
	m_clearColor = color;
}

void GLContent::clearAlpha()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
void GLContent::clearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}
void GLContent::clearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
void GLContent::clearColor()
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
}
void GLContent::clearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GLContent::clearAll()
{
	clearBuffers();
	clearColor();
}

glm::vec2 GLContent::getWindowSize()
{
	return m_windowSize;
}

void GLContent::setEyePos(glm::vec3 pos)
{
	m_eyePos = pos;
}

void GLContent::setEyeLookPos(glm::vec3 pos)
{
	m_eyeLookPos = pos;
}

void GLContent::setUp(glm::vec3 up)
{
	this->m_up = up;
}

void GLContent::setCamera(gfx::engine::GLCamera * camera)
{
	m_eyePos = camera->m_pos;
	m_eyeLookPos = camera->m_dir + camera->m_pos;
	m_up = camera->m_up;
}

float GLContent::getIsometricDepth()
{
	return m_isometricDepth;
}

void GLContent::setIsometricDepth(float depth)
{
	m_isometricDepth = depth;
}

glm::vec2 GLContent::getMousePos()
{
	double xpos, ypos;
	glfwGetCursorPos(m_window, &xpos, &ypos);
	return glm::vec2(xpos, ypos);
}
glm::vec2 GLContent::getMouseDelta()
{
	return m_newMousePos - m_oldMousPos;
}

alib::KeyboardEvents * GLContent::getKeyboardEvents()
{
	return &m_keyboard;
}

glm::mat4 GLContent::getExternalOrtho()
{
	return glm::ortho(
		0.0f,
		m_windowSize.x,		
		m_windowSize.y,
		0.0f,
		-m_farZ, m_farZ);
	//return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}
glm::mat4 GLContent::getExternalOrthoView()
{
	return glm::mat4(1.0f);
	/*return glm::lookAt(
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 57.28996f * window_size.x),
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 0),
	glm::vec3(0, 1, 0));*/
}

glm::mat4 GLContent::getOrtho()
{
	return glm::ortho(
		0.0f + m_eyePos.x - m_windowSize.x / 2.0f,
		m_windowSize.x + m_eyePos.x - m_windowSize.x / 2.0f,
		0.0f + m_eyePos.y - m_windowSize.y / 2.0f,
		m_windowSize.y + m_eyePos.y - m_windowSize.y / 2.0f,
		-m_farZ, m_farZ);
	//return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}
glm::mat4 GLContent::getOrthoView()
{
	return glm::mat4(1.0f);
	/*return glm::lookAt(
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 57.28996f * window_size.x),
	glm::vec3(0.5f * window_size.x, 0.5f * window_size.y, 0),
	glm::vec3(0, 1, 0));*/
}

glm::mat4 GLContent::getOrthoIsometric()
{
	return glm::ortho(0.0f, m_windowSize.x, 0.0f, m_windowSize.y, -m_farZ, m_farZ);
	//return glm::perspective(glm::radians(1.0f), 1.0f, 0.1f, 100.0f * window_size.x);
}
glm::mat4 GLContent::getOrthoIsometricView()
{
	return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}

glm::mat4 GLContent::getPseudoIsometric()
{
	return glm::perspective(glm::radians(2.864f), m_aspectRatio, m_nearZ, m_farZ);
}
glm::mat4 GLContent::getPseudoIsometricView()
{
	return glm::lookAt(m_eyePos + (m_isometricDepth * 20.0f) * glm::vec3(1, 1, 1), m_eyePos, m_up);
}

glm::mat4 GLContent::getPerspective()
{
	return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearZ, m_farZ);
}
glm::mat4 GLContent::getPerspectiveView()
{
	return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}

glm::mat4 GLContent::getHyperPerspective()
{
	return glm::perspective(glm::radians(160.0f), m_aspectRatio, m_nearZ, m_farZ);
}
glm::mat4 GLContent::getHyperPerspectiveView()
{
	return glm::lookAt(m_eyePos, m_eyeLookPos, m_up);
}


void GLContent::handleKeyEvent()
{


}

//GL graphics loop
void			GLContent::glLoop(gfx::engine::GLContentLoop graphics_loop, GLFWwindow * window)
{
	CINFO("Running GL loop...");
	//Main Loop  
	do
	{
		m_frames++;

		// start clock for this tick
		auto start = std::chrono::high_resolution_clock::now();

		handleKeyEvent();

		m_oldMousPos = m_newMousePos;
		m_newMousePos = getMousePos();
		m_keyboard.run();

		graphics_loop();

		//Swap buffers  
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		m_keyDown = NULL;
		glfwPollEvents();

		// stop clock
		auto finish = std::chrono::high_resolution_clock::now();
		int ms = float(std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count());
		long newWait = 5 - ms;// -(gm.gameSpeed);
		newWait = newWait < 0 ? 0 : newWait;
		// throttle the graphics loop to cap at a certain fps
		std::this_thread::sleep_for(std::chrono::milliseconds(newWait));
	} //Check if the ESC or Q key had been pressed or if the window had been closed  
	while (!glfwWindowShouldClose(window));

	CINFO("Window has closed. Application will now exit.");

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
//GL window initialise
GLFWwindow *				GLContent::initWindow(gfx::engine::GLContentInit init, GLFWkeyfun key_func, GLFWmousebuttonfun mouse_func)
{
	CINFO("Initialising new GLContext...");

	GLFWwindow * window;

	//Set the error callback  
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW  
	if (!glfwInit())
	{
		CERROR("failed to init GLFW", __FILE__, __LINE__, CLASSNAME, "initWindow");
		exit(EXIT_FAILURE);
	}
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	CINFO("    GLFW Initialised");

	
	//Create a window and create its OpenGL context  
	window = glfwCreateWindow(m_windowSize.x, m_windowSize.y, "Test Window", NULL, NULL);
	//If the window couldn't be created  
	if (!window)
	{
		CERROR("failed to open GLFW", __FILE__, __LINE__, CLASSNAME, "initWindow");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	m_window = window;
	CINFO("    Created GLFW window");


	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(window);
	CINFO("    GLFW window context set");

	//Sets the key callback  
	//glfwSetKeyCallback(window, key_func);
	//glfwSetMouseButtonCallback(window, mouse_func);
	
	//Initialize GLEW  
	GLenum err = glewInit();
	//If GLEW hasn't initialized  
	if (err != GLEW_OK)
	{
		CERROR("failed to to init GLEW", __FILE__, __LINE__, CLASSNAME, "initWindow");
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	CINFO("    GLEW Initialised");

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
	init();

	return window;
}

GLContent::GLContent() {}

GLContent::GLContent(glm::vec3 window_size, glm::vec3 eye_pos, glm::vec3 eye_look_pos, glm::vec3 up, float fov, float aspect_ratio, float near_z, float far_z)
{
	this->m_windowSize = window_size;
	this->m_eyePos = eye_pos;
	this->m_eyeLookPos = eye_look_pos;
	this->m_up = up;
	this->m_fov = fov;
	this->m_aspectRatio = aspect_ratio;
	this->m_nearZ = near_z;
	this->m_farZ = far_z;
}