#include <stdio.h>
#include "CLog.h"
#include "GEngine.h"
#include "colors.h"
#include "GShaderVariableHandle.h"
#include "GShaderProgram.h"
#include "GShaderProgramManager.h"
#include "GMesh.h"
#include "GPrimativeFactory.h"
#include "GMeshShaderHandle_T.h"
#include "GLight_T.h"
#include "GFrameBufferObject.h"


using glw::engine::GEngine;
using glw::engine::glsl::GShaderVariableHandle;
using glw::engine::glsl::GShaderProgram;
using glw::engine::glsl::GShaderProgramId;
using glw::engine::glsl::GShaderProgramManager;
using glw::engine::glsl::GMeshShaderHandle_T;
using glw::GReturnCode::GLW_SUCCESS;
using glw::GReturnCode::GLW_FAIL;
using glw::engine::buffers::GMesh;
using glw::engine::buffers::GPrimativeFactory;
using glw::engine::buffers::GArrayVertex;
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
  GFrameBufferObject fbo;
  GMesh mesh;  
  GCamera camera(glm::vec3(0, 0, 5), glm::vec3(), glm::vec3(0,0,-1), glm::vec3(0, 1, 0));
	GMeshShaderHandle_T meshShaderHandle;
  GLight_T light = { glm::vec3(0,5,0), glw::RED, glm::vec3(2,25,100) };

  float color = 0.0f;

  glm::vec3 ambientColor;
}


GReturnCode loop()
{
  // Calculate some maths
  color += 0.001f;
  color = fmod(color, 1.0f);
  ambientColor = glm::vec3(color, fmod(color + 0.3f, 1.0f), fmod(color + 0.6f, 1.0f));
  ambientColor /= 2.0f;
  engine->setClearColor(ambientColor);

  mesh.m_theta += 0.05f;
  light.pos.x = sin(mesh.m_theta) * 5.0f;
  light.pos.z = cos(mesh.m_theta) * 5.0f;
  //////////

  // Update the camera
  camera.update(0.1f, 0.9f);
  engine->setCamera(camera);

  // Setup 3D perspective
  engine->clearAll();
  engine->loadPerspective();

  // Load the shader program to draw with
  shaderProgramManager->loadProgram(PHONG_PROGRAM);

  // Enable binding to FBO
  fbo.bind();
  // Draw the meshes
  shaderProgramManager->drawMesh(mesh);
  // Unbind the bound FBO
  fbo.unbind();

  // Setup ortho perspective
  engine->setClearColor(glw::BLACK);
  engine->clearAll();
  engine->loadOrtho();

  // Load the shader program to draw with
  shaderProgramManager->loadProgram(RENDER_PROGRAM);

  // Set the size of the FBO mesh to a size of the window
  glm::vec2 windowSize;
  engine->getWindowSize(windowSize);
  fbo.setSize(windowSize / 2.0f);

  // Draw the FBO
  fbo.setTopLeft(windowSize * 0.25f);
  shaderProgramManager->drawFrameBufferObject(fbo);

  fbo.setTopLeft(windowSize * -0.25f);
  shaderProgramManager->drawFrameBufferObject(fbo);

  fbo.setTopLeft(windowSize * glm::vec2(1, -1) * 0.25f);
  shaderProgramManager->drawFrameBufferObject(fbo);

  fbo.setTopLeft(windowSize * glm::vec2(-1, 1) * 0.25f);
  shaderProgramManager->drawFrameBufferObject(fbo);

  return GLW_SUCCESS;
}


GReturnCode initShaderPrograms()
{
  GReturnCode success = GLW_SUCCESS;

  if (GLW_SUCCESS == success)
  {
    CINFO(TRG, "Initialising GLSL shader programs...");

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
        CERROR(TRG, "shaderProgram is NULL",
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
       shaderProgram->addHandle(GShaderVariableHandle(VAR_LIGHT_POSITION, &light.pos));
       shaderProgram->addHandle(GShaderVariableHandle(VAR_LIGHT_COLOR, &light.color));
       shaderProgram->addHandle(GShaderVariableHandle(VAR_LIGHT_PROPERTIES, &light.brightness_specscale_shinniness));
       shaderProgram->addHandle(GShaderVariableHandle(VAR_EYE_POSITION, engine->getEyePos()));
       shaderProgram->addHandle(GShaderVariableHandle(VAR_AMBIENT_COLOR, &ambientColor));
     }
     else
     {
       CERROR(TRG, "shaderProgram is NULL",
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
    CINFO(TRG, "Creating FBOs...");

    glm::vec2 windowSize;
    engine->getWindowSize(windowSize);
    fbo = GFrameBufferObject(windowSize);
  }

  return success;
}

GReturnCode initMeshes()
{
  GReturnCode success = GLW_SUCCESS;

  if (GLW_SUCCESS == success)
  {
    GArrayVertex o;
    GArrayVec3 v;

    CINFO(TRG, "Generating Sphere...");
    GPrimativeFactory::sphere(v, 200, 200);
    GPrimativeFactory::packObject(o, v);

    mesh = GMesh(
             o,
             glm::vec3(0, 0, 0),
             glm::vec3(-1, 1, 0.5), glm::radians(0.0f),
             glm::vec3(1, 0, 0), glm::radians(90.0f));
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
    success = initMeshes();
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
		  CINFO(TRG, "User left click");
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
		  CINFO(TRG, "User triggered terminatation.");
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}
}

int main()
{
  CSTARTLOGGER("../logs/GLW");

  CINFO(TRG, "Program started.");

  engine = new GEngine();
  engine->setClearColor(glw::GREY_A);
  engine->setPrintFps(false);
  engine->setFpsCap(60);
  engine->run(loop, init, key_callback, mouse_button_callback);
  
  CINFO(TRG, "Program exit.");
  
  CENDLOGGER();
  
  return 0;
}
