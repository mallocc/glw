#include <stdio.h>
#include "Logger.h"
#include "GEngine.h"
#include "colors.h"
#include "GShaderProgram.h"
#include "GShaderProgramManager.h"
#include "GVertexBufferObject.h"
#include "GPrimativeFactory.h"


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
using glw::engine::buffers::GArrayVec2;


namespace
{
  const char * TRG = "MAIN";
  const char * __CLASSNAME__ = "main";

  GEngine engine;
  GShaderProgramManager shaderProgramManager;
  GShaderProgramId TEXTURE_PROGRAM;
  GVertexBufferObject vbo;

  GCamera camera(glm::vec3(0, 0, 5), glm::vec3(), glm::vec3(0,0,-1), glm::vec3(0, 1, 0));
}


GReturnCode loop()
{
  // (calculations should be done in a different thread)

  // Update the camera
  camera.update(0.1f, 0.9f);

  // Update the engine with the camera
  engine.setCamera(camera);

  // Rotate the sphere
  vbo.m_theta += 0.01f;


  // Clear the scene
  engine.clearAll();

  // Set the 3D perspective
  engine.load3DPerspective();

  // Load the shader program we want to draw with
  shaderProgramManager.loadProgram(TEXTURE_PROGRAM);

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
        engine.getModelMat(),            // Pass the engine's model matrix
        engine.getViewMat(),             // Pass the engine's view matrix
        engine.getProjMat(),             // Pass the engine's proj. matrix
        TEXTURE_PROGRAM))                   // Supply the id container
  {
    // Get the shader program we have just created
    GShaderProgram * shaderProgram = shaderProgramManager.getProgram(TEXTURE_PROGRAM);

    // Check the program has been created
    if (NULL != shaderProgram)
    {
      if(shaderProgram->isValid())
      {
        // Set the variable handle for our textures
        if (GLW_FAIL == shaderProgram->setTexHandle())
        {
          success = GLW_FAIL;
          LERROR(TRG, "BASIC_PROGRAM is not valid",
                 __FILE__, __LINE__, __CLASSNAME__, __func__);
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

  return success;
}

GReturnCode initVBOs()
{
  GReturnCode success = GLW_SUCCESS;

  LINFO(TRG, "Initialising VBOs...");

  // Create array containers
  GArrayVertex o;
  GArrayVec3 v, c, n, t;
  GArrayVec2 uv;

  LINFO(TRG, "Generating Sphere...");

  // Generate an array of vec3s for a sphere
  GPrimativeFactory::sphere(v, 200, 200);

  // Create the texture coordinates for a sphere
  GPrimativeFactory::sphereicalUVs(uv, v);

  // Pack the vec3s and texture coords into a vertex array
  GPrimativeFactory::packObject(o, v, uv);

  // Create a new VBO with our new vertex array
  vbo = GVertexBufferObject(
          o,                        // Supply the vertex array
          glm::vec3(),              // World position
          glm::vec3(0, 1, 0),       // Rotation axis
          glm::radians(0.0f),       // Rotation angle
          glm::vec3(1, 0, 0),       // Pre-rotation axis
          glm::radians(90.0f),      // Pre-rotation angle
          glm::vec3(1),             // Scale vector
          "../textures/mars.jpg");  // Texture file

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

void	key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

  // Set the clear colour of the scene
  engine.setClearColor(glw::GREY_A);
  // Set the window size
  engine.setWindowSize(glm::vec2(1280,720));
  // Set the callbacks for the engine, and run
  engine.run(loop, init, key_callback, mouse_button_callback);

  LINFO(TRG, "Program exit.");

  LENDLOGGER();

  return 0;
}

