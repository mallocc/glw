#include <stdio.h>
#include "Logger.h"
#include "GEngine.h"
#include "colors.h"
#include "GShaderVariableHandle.h"
#include "GShaderProgram.h"
#include "GShaderProgramManager.h"
#include "GVertexBufferObject.h"
#include "GPrimativeFactory.h"
#include "GShaderHandle_T.h"
#include "GLight_T.h"
#include "GFrameBufferObject.h"
#include "StringFormat.h"


using util::StringFormat;
using glw::engine::GEngine;
using glw::engine::glsl::GShaderVariableHandle;
using glw::engine::glsl::GShaderProgram;
using glw::engine::glsl::GShaderProgramId;
using glw::engine::glsl::GShaderProgramManager;
using glw::engine::glsl::GShaderHandle_T;
using glw::GReturnCode::GLW_SUCCESS;
using glw::GReturnCode::GLW_FAIL;
using glw::engine::buffers::GVertexBufferObject;
using glw::engine::buffers::GPrimativeFactory;
using glw::engine::buffers::GArrayVertex;
using glw::engine::buffers::GArrayVec2;
using glw::engine::buffers::GArrayVec3;
using glw::engine::glsl::GLight_T;
using glw::engine::buffers::GFrameBufferObject;
using glw::engine::buffers::GFrameBufferObjectId;


namespace
{
  const char * TRG = "MAIN";
  const char * __CLASSNAME__ = "main";
  
  GEngine * engine;
  GShaderProgramManager * shaderProgramManager;
  GShaderProgram * shaderProgram;
  GShaderProgramId RENDER_PROGRAM;  
  GShaderProgramId PHONG_PROGRAM;
  GShaderProgramId BLUR_PROGRAM;
  GFrameBufferObject fbo;
  GFrameBufferObject fboBlur;
  GVertexBufferObject vbo, vbo2, vbo3;
  GCamera camera(glm::vec3(0, 0, 5), glm::vec3(), glm::vec3(0,0,-1), glm::vec3(0, 1, 0));
  GLight_T light = { glm::vec3(0,5,0), glw::WHITE, glm::vec3(2,25,100) };

  glm::vec3 blurProperties(0.06f,0.06f,0.02f);

  float color = 0.0f;

  glm::vec3 ambientColor;
}


GReturnCode loop()
{
  // Calculate some maths //
  color += 0.001f;
  color = fmod(color, 1.0f);
  //ambientColor = glm::vec3(color, fmod(color + 0.3f, 1.0f), fmod(color + 0.6f, 1.0f));
  //ambientColor /= 5.0f;
  engine->setClearColor(ambientColor);

  vbo.m_theta += 0.05f;
  light.pos.x = sin(vbo.m_theta) * 5.0f;
  light.pos.z = cos(vbo.m_theta) * 5.0f;


  glm::vec2 windowSize;
  engine->getWindowSize(windowSize);
  fboBlur.setSize(windowSize / 2.0f);


  // Update the camera
  camera.update(0.1f, 0.9f);
  engine->setCamera(camera);

  // Draw vbo to blur FBO //

  // Setup 3D perspective
  engine->clearAll();
  engine->loadPerspective();

  // Load the shader program to draw with
  shaderProgramManager->loadProgram(PHONG_PROGRAM);

  // Enable binding to FBO
  fboBlur.bind();
  // Draw the vbo
  shaderProgramManager->drawVBO(vbo);
  shaderProgramManager->drawVBO(vbo2);
    shaderProgramManager->drawVBO(vbo3);
  // Unbind the bound FBO
  fboBlur.unbind();


  // Draw the blur FBO //

  // Setup ortho perspective
  engine->setClearColor(glw::BLACK);
  engine->clearAll();
  engine->loadOrtho();

  fbo.bind();

  // Load the shader program to draw with
  shaderProgramManager->loadProgram(BLUR_PROGRAM);

  // Draw the FBO
  fboBlur.setTopLeft(windowSize * 0.25f);
  shaderProgramManager->drawFBO(fboBlur);

  fboBlur.setTopLeft(windowSize * -0.25f);
  shaderProgramManager->drawFBO(fboBlur);

  fboBlur.setTopLeft(windowSize * glm::vec2(1, -1) * 0.25f);
  shaderProgramManager->drawFBO(fboBlur);

  fboBlur.setTopLeft(windowSize * glm::vec2(-1, 1) * 0.25f);
  shaderProgramManager->drawFBO(fboBlur);

  fbo.unbind();

  // Setup ortho perspective
  engine->setClearColor(glw::BLACK);
  engine->clearAll();
  engine->loadOrtho();

  shaderProgramManager->loadProgram(BLUR_PROGRAM);
  shaderProgramManager->drawFBO(fbo);

  // Draw the vbo on its own //

  // Only clear depth buffer so we can have the blur FBO as a background
  engine->clearDepthBuffer();
  // Setup 3D perspective
  engine->loadPerspective();

  // Load the shader program to draw with
  shaderProgramManager->loadProgram(PHONG_PROGRAM);
  // Draw the vbo
  shaderProgramManager->drawVBO(vbo);
  shaderProgramManager->drawVBO(vbo2);
  shaderProgramManager->drawVBO(vbo3);

  return GLW_SUCCESS;
}


GReturnCode initShaderPrograms()
{
  GReturnCode success = GLW_SUCCESS;

  if (GLW_SUCCESS == success)
  {
    LINFO(TRG, "Initialising GLSL shader programs...");

    shaderProgramManager = new GShaderProgramManager();

    if (GLW_SUCCESS == shaderProgramManager->addProgram(
          "../shaders/basic_texture.vert", "../shaders/basic_texture.frag",
          engine->getModelMat(), engine->getViewMat(), engine->getProjMat(),
          RENDER_PROGRAM))
    {
      shaderProgram = shaderProgramManager->getProgram(RENDER_PROGRAM);
      if (NULL != shaderProgram)
      {
        shaderProgram->setTexHandle();
      }
      else
      {
        LERROR(TRG, "shaderProgram is NULL",
            __FILE__, __LINE__, __CLASSNAME__, __func__);
      }
    }
    else
    {
      success = GLW_FAIL;
    }


    if (GLW_SUCCESS == shaderProgramManager->addProgram(
          "../shaders/phong.vert", "../shaders/phong.frag",
          engine->getModelMat(), engine->getViewMat(), engine->getProjMat(),
          PHONG_PROGRAM))
    {
     shaderProgram = shaderProgramManager->loadProgram(PHONG_PROGRAM);
     if(NULL != shaderProgram)
     {
       shaderProgram->setTexHandle();
       shaderProgram->addHandle(GShaderVariableHandle(VAR_LIGHT_POSITION, &light.pos));
       shaderProgram->addHandle(GShaderVariableHandle(VAR_LIGHT_COLOR, &light.color));
       shaderProgram->addHandle(GShaderVariableHandle(VAR_LIGHT_PROPERTIES, &light.brightness_specscale_shinniness));
       shaderProgram->addHandle(GShaderVariableHandle(VAR_EYE_POSITION, engine->getEyePos()));
       shaderProgram->addHandle(GShaderVariableHandle(VAR_AMBIENT_COLOR, &ambientColor));
     }
     else
     {
       LERROR(TRG, "shaderProgram is NULL",
           __FILE__, __LINE__, __CLASSNAME__, __func__);
     }
    }


    if (GLW_SUCCESS == shaderProgramManager->addProgram(
          "../shaders/basic_texture.vert", "../shaders/basic_texture_blur.frag",
          engine->getModelMat(), engine->getViewMat(), engine->getProjMat(),
          BLUR_PROGRAM))
    {
      shaderProgram = shaderProgramManager->loadProgram(BLUR_PROGRAM);
      if(NULL != shaderProgram)
      {
        shaderProgram->setTexHandle();
        shaderProgram->addHandle(GShaderVariableHandle("u_glow_res", &blurProperties));
      }
      else
      {
        LERROR(TRG, "shaderProgram is NULL",
               __FILE__, __LINE__, __CLASSNAME__, __func__);
      }
    }
    else
    {
      success = GLW_FAIL;
    }
  }

  return success;
}

GReturnCode initFBOs()
{
  GReturnCode success = GLW_SUCCESS;

  if (GLW_SUCCESS == success)
  {
    LINFO(TRG, "Creating FBOs...");

    glm::vec2 windowSize;
    engine->getWindowSize(windowSize);

    fbo = GFrameBufferObject(windowSize);
    fboBlur = GFrameBufferObject(windowSize);
  }

  return success;
}

GReturnCode initVBOs()
{
  GReturnCode success = GLW_SUCCESS;

  if (GLW_SUCCESS == success)
  {
    GArrayVertex o;
    GArrayVec3 v, c, n, t;

    LINFO(TRG, "Generating Sphere...");
    GPrimativeFactory::sphere(v, 200, 200);
    GPrimativeFactory::packObject(o, v);

    vbo = GVertexBufferObject(
            o,
            glm::vec3(0, 0, 0),
            glm::vec3(-1, 1, 0.5), glm::radians(0.0f),
            glm::vec3(1, 0, 0), glm::radians(90.0f));


    GArrayVec2 uv;
    GPrimativeFactory::sphereicalUVs(uv, v);
    GPrimativeFactory::packObject(o, v, uv);


    vbo2 = GVertexBufferObject(
                    o,
                    glm::vec3(2, 0, 0),
                    glm::vec3(-1, 1, 0.5), glm::radians(0.0f),
                    glm::vec3(1, 0, 0), glm::radians(90.0f),
                    glm::vec3(1),
                    "../textures/mars.jpg");


    vbo3 = GVertexBufferObject(
                    o,
                    glm::vec3(-2, 0, 0),
                    glm::vec3(-1, 1, 0.5), glm::radians(0.0f),
                    glm::vec3(1, 0, 0), glm::radians(90.0f),
                    glm::vec3(1),
                    "../textures/151.bmp");
  }

  return success;
}

GReturnCode init()
{
  GReturnCode success = GLW_SUCCESS;

  // SHADER PROGRAM SETUP //
  success = initShaderPrograms();

  // FBO SETUP //
  if (GLW_SUCCESS == success)
  {
    success = initFBOs();
  }

  // MESH SETUP //
  if (GLW_SUCCESS == success)
  {
    success = initVBOs();
  }

  return success;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:		
      LINFO(TRG, "User left click");
			break;
		}
	}
}

static void	key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{		
		switch (key)
		{
    case GLFW_KEY_A:
      camera.applyForceLeft();
      break;
    case GLFW_KEY_D:
      camera.applyForceRight();
      break;      
    case GLFW_KEY_W:
      camera.applyForceForward();
      break;      
    case GLFW_KEY_S:
      camera.applyForceBackward();
      break;
	  case GLFW_KEY_Q:	
      camera.applyForceDown();
      break;
	  case GLFW_KEY_E:	
      camera.applyForceUp();
      break;
      
	  case GLFW_KEY_UP:	
      engine->setFpsCap(engine->getFpsCap()+1);
      break;
      
	  case GLFW_KEY_DOWN:
      engine->setFpsCap(engine->getFpsCap()-1);
      break;
      
		case GLFW_KEY_ESCAPE:	
      LINFO(TRG, "User triggered terminatation.");
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}
}

int main()
{
  LSTARTLOGGER("../logs/GLW");

  LINFO(TRG, "Program started.");

  engine = new GEngine();
  engine->setClearColor(glw::GREY_A);
  engine->setPrintFps(false);
  engine->setFpsCap(60);
  engine->setWindowSize(glm::vec2(1280,720));
  engine->run(loop, init, key_callback, mouse_button_callback);
  
  LINFO(TRG, "Program exit.");
  
  LENDLOGGER();
  
  return 0;
}
