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
  GShaderProgramId BASIC_PROGRAM;
  GShaderProgramId LIGHTING_PROGRAM;

  GArrayVBO vbos;

  GCamera camera(glm::vec3(0, 0, 5), glm::vec3(), glm::vec3(0,0,-1), glm::vec3(0, 1, 0));

  GLight_T light = { glm::vec3(5, 5, 5), glw::WHITE, glm::vec3(1, 25, 100) };

  glm::vec3 ambientColor = glw::BLACK;

  GFrameBufferObject fbo;
}


GReturnCode loop()
{
  // (calculations should be done in a different thread)

  // Update the camera
  camera.update(0.1f, 0.9f);
  // Update the engine with the camera
  engine.setCamera(camera);
  // Rotate the sphere
  vbos[0].m_theta += 0.01f;
  // Rotate the second VBO around the sphere using quaternions
  vbos[1].m_pos = glm::quat(1, 0, 0.001f, 0) * vbos[1].m_pos;

  // Set the scene clear color
  engine.setClearColor(ambientColor);


  // Draw scene to an FBO //

  // Clear the scene
  engine.clearAll();
  // Set the 3D perspective
  engine.load3DPerspective();

  // Bind the FBO to draw to
  fbo.bind();
  // Load the shader program we want to draw with
  shaderProgramManager.loadProgram(LIGHTING_PROGRAM);
  // Draw the sphere VBO
  shaderProgramManager.drawVBOs(vbos);
  // Finish with the FBO by unbinding
  fbo.unbind();


  // Draw the FBO as a VBO //

  // Clear the scene
  engine.clearAll();
  // Set the ortho perspective
  engine.loadOrtho();

  // Load the shader program we want to draw with
  shaderProgramManager.loadProgram(BASIC_PROGRAM);
  // Draw the FBO
  shaderProgramManager.drawFBO(fbo);

  return GLW_SUCCESS;
}

GReturnCode initShaderPrograms()
{
  GReturnCode success = GLW_SUCCESS;

  LINFO(TRG, "Initialising GLSL shader programs...");

  // Add a new program to the manager
  if (GLW_SUCCESS == shaderProgramManager.addNewProgram(
        "../shaders/phong.vert",          // Vertex shader
        "../shaders/phong.frag",          // Fragment shader
        engine.getModelMat(),             // Pass the engine's model matrix
        engine.getViewMat(),              // Pass the engine's view matrix
        engine.getProjMat(),              // Pass the engine's proj. matrix
        LIGHTING_PROGRAM))                // Supply the id container
  {
    // Get the shader program we have just created
    GShaderProgram * shaderProgram = shaderProgramManager.getProgram(LIGHTING_PROGRAM);

    // Check the program has been created
    if (NULL != shaderProgram)
    {
      if(shaderProgram->isValid())
      {
        // Set the variable handle for our textures
        if (GLW_FAIL == shaderProgram->setTexHandle())
        {
          success = GLW_FAIL;
        }

        // Add a global light properties //

        if (GLW_FAIL == shaderProgram->addHandle(GShaderVariableHandle(VAR_LIGHT_POSITION,
                                                                       &light.pos)))
        {
          success = GLW_FAIL;
        }
        if (GLW_FAIL == shaderProgram->addHandle(GShaderVariableHandle(VAR_LIGHT_COLOR,
                                                                       &light.color)))
        {
          success = GLW_FAIL;
        }
        if (GLW_FAIL == shaderProgram->addHandle(GShaderVariableHandle(VAR_LIGHT_PROPERTIES,
                                                                       &light.brightness_specscale_shinniness)))
        {
          success = GLW_FAIL;
        }
        if (GLW_FAIL == shaderProgram->addHandle(GShaderVariableHandle(VAR_EYE_POSITION,
                                                                       engine.getEyePos())))
        {
          success = GLW_FAIL;
        }
        if (GLW_FAIL == shaderProgram->addHandle(GShaderVariableHandle(VAR_AMBIENT_COLOR,
                                                                       &ambientColor)))
        {
          success = GLW_FAIL;
        }
      }
      else
      {
        success = GLW_FAIL;
        LERROR(TRG, "LIGHTING_PROGRAM is not valid",
               __FILE__, __LINE__, __CLASSNAME__, __func__);
      }
    }
    else
    {
      success = GLW_FAIL;
      LERROR(TRG, "LIGHTING_PROGRAM is NULL",
             __FILE__, __LINE__, __CLASSNAME__, __func__);
    }
  }
  else
  {
    success = GLW_FAIL;
    LERROR(TRG, "Failed to add LIGHTING_PROGRAM",
           __FILE__, __LINE__, __CLASSNAME__, __func__);
  }

  if(GLW_SUCCESS == success)
  {
    // Add a new program to the manager
    if (GLW_SUCCESS == shaderProgramManager.addNewProgram(
          "../shaders/basic_texture.vert",          // Vertex shader
          "../shaders/basic_texture.frag",          // Fragment shader
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
          // Set the variable handle for our textures
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
  fbo = GFrameBufferObject(windowSize);

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
  vbos.push_back(
        GVertexBufferObject(
                  o,                        // Supply the vertex array
                  glm::vec3(),              // World position
                  glm::vec3(0, 1, 0),       // Rotation axis
                  glm::radians(0.0f),       // Rotation angle
                  glm::vec3(1, 0, 0),       // Pre-rotation axis
                  glm::radians(90.0f),      // Pre-rotation angle
                  glm::vec3(1),             // Scale vector
                  "../textures/mars.jpg")   // Texture file
      );

  // Create a new VBO with our new vertex array
  vbos.push_back(
        GVertexBufferObject(
                  o,                        // Supply the vertex array
                  glm::vec3(4, 0, 0),              // World position
                  glm::vec3(0, 1, 0),       // Rotation axis
                  glm::radians(0.0f),       // Rotation angle
                  glm::vec3(1, 0, 0),       // Pre-rotation axis
                  glm::radians(90.0f),      // Pre-rotation angle
                  glm::vec3(0.1f),             // Scale vector
                  "../textures/151.bmp")   // Texture file
      );

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
  engine.setClearColor(ambientColor);
  // Set the window size
  engine.setWindowSize(glm::vec2(1280,720));
  // Set the callbacks for the engine, and run
  engine.run(loop, init, key_callback, mouse_button_callback);

  LINFO(TRG, "Program exit.");

  LENDLOGGER();

  return 0;
}
