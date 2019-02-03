#include <stdio.h>
#include "Logger.h"
#include "GEngine.h"
#include "colors.h"
#include "GShaderProgram.h"
#include "GShaderProgramManager.h"
#include "GVertexBufferObject.h"
#include "GPrimativeFactory.h"
#include "StringFormat.h"

using util::StringFormat;

using glw::GReturnCode::GLW_SUCCESS;
using glw::GReturnCode::GLW_FAIL;

using glw::engine::GEngine;

using glw::engine::glsl::GShaderProgram;
using glw::engine::glsl::GShaderProgramId;
using glw::engine::glsl::GShaderProgramManager;

using glw::engine::buffers::GVertexBufferObject;
using glw::engine::buffers::GPrimativeFactory;
using glw::engine::buffers::GArrayVertex;
using glw::engine::buffers::GArrayVec3;


namespace
{
  const char * TRG = "MAIN";
  const char * __CLASSNAME__ = "main";

  GEngine * engine;
  GShaderProgramManager shaderProgramManager;
  GShaderProgramId BASIC_PROGRAM;
  GVertexBufferObject vbo;

  GCamera camera(glm::vec3(0, 0, 5), glm::vec3(), glm::vec3(0,0,-1), glm::vec3(0, 1, 0));
}


void handleInput()
{
  if(engine->getKeyboard()->isKeyDown(GLFW_KEY_LEFT_SHIFT))
  {
    camera.applyForceUp(engine->getMouse()->popScrollDelta().y);
  }
  else
  {
    camera.applyForceForward(engine->getMouse()->popScrollDelta().y);
  }
}

GReturnCode loop()
{
  handleInput();

  // (calculations should be done in a different thread)

  // Update the camera
  camera.update(0.1f, 0.9f);

  // Update the engine with the camera
  engine->setCamera(camera);

  // Rotate the sphere
  vbo.m_theta += 0.01f;


  // Clear the scene
  engine->clearAll();

  // Set the 3D perspective
  engine->load3DPerspective();

  // Load the shader program we want to draw with
  shaderProgramManager.loadProgram(BASIC_PROGRAM);

  // Draw the sphere VBO
  shaderProgramManager.drawVBO(vbo);

  return GLW_SUCCESS;
}

GReturnCode initShaderPrograms()
{
  GReturnCode success = GLW_SUCCESS;

  LINFO(TRG, "Initialising GLSL shader programs...");

  // Add a new program to the manager
  if (GLW_SUCCESS == shaderProgramManager.addNewProgram(
        "../shaders/basic.vert",  // Vertex shader
        "../shaders/basic.frag",  // Fragment shader
        engine->getModelMat(),            // Pass the engine's model matrix
        engine->getViewMat(),             // Pass the engine's view matrix
        engine->getProjMat(),             // Pass the engine's proj. matrix
        BASIC_PROGRAM))                   // Supply the id container
  {
    // Get the shader program we have just created
    GShaderProgram * shaderProgram = shaderProgramManager.getProgram(BASIC_PROGRAM);

    // Check the program has been created
    if (NULL != shaderProgram)
    {
      if(!shaderProgram->isValid())
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

  return success;
}

GReturnCode initVBOs()
{
  GReturnCode success = GLW_SUCCESS;

  LINFO(TRG, "Initialising VBOs...");

  // Create array containers
  GArrayVertex o;
  GArrayVec3 v, c, n, t;

  LINFO(TRG, "Generating Sphere...");

  // Generate an array of vec3s for a sphere
  GPrimativeFactory::sphere(v, 200, 200);

  // Pack the vec3s into a vertex array
  GPrimativeFactory::packObject(o, v);

  // Create a new VBO with our new vertex array
  vbo = GVertexBufferObject(
          o,                      // Supply the vertex array
          glm::vec3(),            // World position
          glm::vec3(0, 1, 0),     // Rotation axis
          glm::radians(0.0f),     // Rotation angle
          glm::vec3(1, 0, 0),     // Pre-rotation axis
          glm::radians(90.0f),    // Pre-rotation angle
          glm::vec3(1));          // Scale vector

  return success;
}

GReturnCode init()
{
  GReturnCode success = GLW_SUCCESS;

  // SHADER PROGRAM SETUP //
  success = initShaderPrograms();

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
      LINFO(TRG, "User triggered terminatation.");
      engine->exit();
      break;
    }
  }
}

int main()
{
  LSTARTLOGGER("../logs/GLW");

  LINFO(TRG, "Program started.");

  // Get instance pointer
  engine = GEngine::getInstancePtr();

  // Set the clear colour of the scene
  engine->setClearColor(glw::GREY_A);
  // Set the window size
  engine->setWindowSize(glm::vec2(1280,720));
  // Set the callbacks for the engine, and run
  engine->run(loop, init, key_callback, mouse_button_callback);

  LINFO(TRG, "Program exit.");

  LENDLOGGER();

  return 0;
}

