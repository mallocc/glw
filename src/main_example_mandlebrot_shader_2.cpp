#include <stdio.h>
#include "Logger.h"
#include "GEngine.h"
#include "colors.h"

#include "GShaderProgram.h"
#include "GShaderProgramManager.h"
#include "GShaderVariableHandle.h"

#include "GVertexBufferObject.h"
#include "GPrimativeFactory.h"

#include "GLight_T.h"

#include "GFrameBufferObject.h"


using glw::GReturnCode::GLW_SUCCESS;
using glw::GReturnCode::GLW_FAIL;

using glw::engine::GEngine;

using glw::engine::glsl::GShaderProgram;
using glw::engine::glsl::GShaderProgramId;
using glw::engine::glsl::GShaderProgramManager;
using glw::engine::glsl::GLight_T;
using glw::engine::glsl::GShaderVariableHandle;

using glw::engine::buffers::GVertexBufferObject;
using glw::engine::buffers::GPrimativeFactory;
using glw::engine::buffers::GArrayVertex;
using glw::engine::buffers::GArrayVec3;
using glw::engine::buffers::GArrayVec2;
using glw::engine::buffers::GArrayVBO;

using glw::engine::buffers::GFrameBufferObject;


namespace
{
  const char * TRG = "MAIN";
  const char * __CLASSNAME__ = "main";

  GEngine engine;

  GShaderProgramManager shaderProgramManager;
  GShaderProgramId MANDLE_PROGRAM, BASIC_PROGRAM;

  GCamera camera(glm::vec3(0, 0, 5), glm::vec3(), glm::vec3(0,0,-1), glm::vec3(0, 1, 0));

  GFrameBufferObject fboMandle, fbo;

  glm::vec3 mandleProperties = glm::vec3(100.0f, 0.25f, 0.0f);
  glm::vec3 mandleTarget;

  bool update = true;
}


GReturnCode loop()
{
  // Update the engine with the camera
  engine.setCamera(camera);

  // Draw the Mandlebrot through an FBO on a shader//

  // Clear the scene
  engine.clearAll();
  // Set the ortho perspective
  engine.loadOrtho();

  // Draw Mandlebrot generation to another FBO
  // to be drawn later, means that it only generates
  // when the user inputs something
  if(update)
  {
    // Bind the FBO to draw to
    fbo.bind();
    // Load the shader program we want to draw with
    shaderProgramManager.loadProgram(MANDLE_PROGRAM);
    // Draw the FBO
    shaderProgramManager.drawFBO(fboMandle);
    // When finised with the FBO, unbind
    fbo.unbind();

    // Reset update flag
    update = false;
  }

  // Load the shader program we want to draw with
  shaderProgramManager.loadProgram(BASIC_PROGRAM);
  // Draw the FBO
  shaderProgramManager.drawFBO(fbo);

  // The shader will draw over the texture that the FBO holds
  // as its generating the texture dynamically

  return GLW_SUCCESS;
}

GReturnCode initShaderPrograms()
{
  GReturnCode success = GLW_SUCCESS;

  LINFO(TRG, "Initialising GLSL shader programs...");

  if(GLW_SUCCESS == success)
  {
    // Add a new program to the manager
    if (GLW_SUCCESS == shaderProgramManager.addNewProgram(
          "../shaders/mandle.vert",                 // Vertex shader
          "../shaders/mandle.frag",                 // Fragment shader
          engine.getModelMat(),                     // Pass the engine's model matrix
          engine.getViewMat(),                      // Pass the engine's view matrix
          engine.getProjMat(),                      // Pass the engine's proj. matrix
          MANDLE_PROGRAM))                          // Supply the id container
    {
      // Get the shader program we have just created
      GShaderProgram * shaderProgram = shaderProgramManager.getProgram(MANDLE_PROGRAM);

      // Check the program has been created
      if (NULL != shaderProgram)
      {
        if(shaderProgram->isValid())
        {
          // Create custom variable link
          if (GLW_FAIL == shaderProgram->addHandle(GShaderVariableHandle("u_mandle_properties",
                                                                         &mandleProperties)))
          {
            success = GLW_FAIL;
          }
          // Create custom variable link
          if (GLW_FAIL == shaderProgram->addHandle(GShaderVariableHandle("u_target",
                                                                         &mandleTarget)))
          {
            success = GLW_FAIL;
          }
        }
        else
        {
          success = GLW_FAIL;
          LERROR(TRG, "MANDLE_PROGRAM is not valid",
                 __FILE__, __LINE__, __CLASSNAME__, __func__);
        }
      }
      else
      {
        success = GLW_FAIL;
        LERROR(TRG, "MANDLE_PROGRAM is NULL",
               __FILE__, __LINE__, __CLASSNAME__, __func__);
      }
    }
    else
    {
      success = GLW_FAIL;
      LERROR(TRG, "Failed to add MANDLE_PROGRAM",
             __FILE__, __LINE__, __CLASSNAME__, __func__);
    }

    // Add a new program to the manager
    if (GLW_SUCCESS == shaderProgramManager.addNewProgram(
          "../shaders/basic.vert",          // Vertex shader
          "../shaders/basic.frag",          // Fragment shader
          engine.getModelMat(),                     // Pass the engine's model matrix
          engine.getViewMat(),                      // Pass the engine's view matrix
          engine.getProjMat(),                      // Pass the engine's proj. matrix
          BASIC_PROGRAM))                           // Supply the id container
    {
      // Get the shader program we have just created
      GShaderProgram * shaderProgram = shaderProgramManager.getProgram(BASIC_PROGRAM);

      // Check the program has been created
      if (NULL != shaderProgram)
      {
        if(shaderProgram->isValid())
        {
          // Create custom variable link
          if (GLW_FAIL == shaderProgram->setTexHandle())
          {
            success = GLW_FAIL;
          }
        }
        else
        {
          success = GLW_FAIL;
          LERROR(TRG, "BASIC_PROGRAM is not valid",
                 __FILE__, __LINE__, __CLASSNAME__, __func__);
        }
      }
      else
      {
        success = GLW_FAIL;
        LERROR(TRG, "BASIC_PROGRAM is NULL",
               __FILE__, __LINE__, __CLASSNAME__, __func__);
      }
    }
    else
    {
      success = GLW_FAIL;
      LERROR(TRG, "Failed to add BASIC_PROGRAM",
             __FILE__, __LINE__, __CLASSNAME__, __func__);
    }
  }

  return success;
}

GReturnCode initFBOs()
{
  GReturnCode success = GLW_SUCCESS;

  LINFO(TRG, "Creating FBOs...");

  // Get the engine window size
  glm::vec2 windowSize;
  engine.getWindowSize(windowSize);

  // Create a new FBO the size of the window
  fboMandle = GFrameBufferObject(windowSize);

  // Create a new FBO the size of the window
  fbo = GFrameBufferObject(windowSize);

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

void	key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    switch (key)
    {
    case GLFW_KEY_UP:
      mandleTarget.y -= 0.01f / mandleProperties.y;
      break;
    case GLFW_KEY_DOWN:
      mandleTarget.y += 0.01f / mandleProperties.y;
      break;
    case GLFW_KEY_RIGHT:
      mandleTarget.x += 0.01f / mandleProperties.y;
      break;
    case GLFW_KEY_LEFT:
      mandleTarget.x -= 0.01f / mandleProperties.y;
      break;

    case GLFW_KEY_O:
      mandleProperties.y *= 1.1f;
      break;
    case GLFW_KEY_P:
      mandleProperties.y /= 1.1f;
      break;

    case GLFW_KEY_K:
      mandleProperties.x += 10.0f;
      break;
    case GLFW_KEY_L:
      mandleProperties.x -= 10.0f;
      break;

    case GLFW_KEY_ESCAPE:
      LINFO(TRG, "User triggered terminatation.");
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;
    }

    // Set flag to regenerate the fractal
    update = true;
  }
}

int main()
{
  LSTARTLOGGER("../logs/GLW");

  LINFO(TRG, "Program started.");

  // Set the window size
  engine.setWindowSize(glm::vec2(800,800));
  // Set the callbacks for the engine, and run
  engine.run(loop, init, key_callback, mouse_button_callback);

  LINFO(TRG, "Program exit.");

  LENDLOGGER();

  return 0;
}
