#include <stdio.h>
#include "Logger.h"
#include "GContent.h"
#include "colors.h"
#include "GShaderProgram.h"
#include "GShaderProgramManager.h"
#include "GVertexBufferObject.h"
#include "GPrimativeFactory.h"
#include "StringFormat.h"

#include "GGUI.h"


using util::StringFormat;

using glw::GReturnCode::GLW_SUCCESS;
using glw::GReturnCode::GLW_FAIL;

using glw::engine::GContent;

using glw::engine::glsl::GShaderProgram;
using glw::engine::glsl::GShaderProgramId;
using glw::engine::glsl::GShaderProgramManager;

using glw::engine::buffers::GVertexBufferObject;
using glw::engine::buffers::GPrimativeFactory;
using glw::engine::buffers::GArrayVertex;
using glw::engine::buffers::GArrayVec3;
using glw::engine::buffers::GArrayVBO;

using glw::gui::GContext;
using glw::gui::GButton;
using glw::gui::GWindow;
using glw::gui::GLabel;
using glw::gui::GTextEdit;
using glw::gui::GSlider;
using glw::gui::GSpinner;
using glw::gui::GDropdown;
using glw::gui::GImageView;
using glw::gui::GProgressBar;
using glw::gui::GCheckBox;


namespace
{
  const char * TRG = "MAIN";
  const char * __CLASSNAME__ = "main";

  GContent * content;
  GShaderProgramManager shaderProgramManager;
  GShaderProgramId BASIC_PROGRAM;
  GShaderProgramId GUI_PROGRAM;

  GVertexBufferObject vbo;

  GArrayVBO guiVBOs;

  GCamera camera(glm::vec3(0, 0, 5), glm::vec3(), glm::vec3(0,0,-1), glm::vec3(0, 1, 0));

  GContext context;

  GLabel * fpsLabel;

  GProgressBar * progressbar;
}



void handleInput()
{
  if(content->getKeyboard()->isKeyDown(GLFW_KEY_LEFT_SHIFT))
  {
    camera.applyForceUp(content->getMouse()->popScrollDelta().y);
  }
  else
  {
    camera.applyForceForward(content->getMouse()->popScrollDelta().y);
  }

  if (content->getKeyboard()->isKeyDown(GLFW_KEY_DOWN))
  {
    progressbar->setProgress(progressbar->getProgress() - 1);
  }

  if (content->getKeyboard()->isKeyDown(GLFW_KEY_UP))
  {
    progressbar->setProgress(progressbar->getProgress() + 1);
  }
}

GReturnCode loop()
{
  handleInput();

  fpsLabel->setText(StringFormat("%0 FPS").arg(content->getFps()).str());

  context.update();

  // (calculations should be done in a different thread)

  // Update the camera
  camera.update(0.1f, 0.9f);

  // Update the engine with the camera
  content->setCamera(camera);

  // Rotate the sphere
  vbo.m_theta += 0.01f;

  // Clear the scene
  content->clearAll();

  // Set the 3D perspective
  content->load3DPerspective();

  // Load the shader program we want to draw with
  shaderProgramManager.loadProgram(BASIC_PROGRAM);

  // Draw the sphere VBO
  shaderProgramManager.drawVBO(vbo);

  // Clear the scene
  content->clearDepthBuffer();

  // Set the 3D perspective
  content->loadExternalOrtho();

  context.draw();


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
        content->getModelMat(),            // Pass the engine's model matrix
        content->getViewMat(),             // Pass the engine's view matrix
        content->getProjMat(),             // Pass the engine's proj. matrix
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
      else
      {
        shaderProgram->setTexHandle();
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

  context.initShaderProgram(content->getModelMat(),
                            content->getViewMat(),
                            content->getProjMat());

  return success;
}

void onButtonPress()
{
  LINFO(TRG, "This is a test");
  context.validate();
}

void onToggledOn()
{
  LINFO(TRG, "Toggled On");
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
  GPrimativeFactory::sphere(v, 12, 12);

  // Create the texture coordinates for a sphere
  GPrimativeFactory::sphereicalUVs(uv, v);

  // Pack the vec3s into a vertex array
  GPrimativeFactory::packObject(o, v, uv);

  // Create a new VBO with our new vertex array
  vbo = GVertexBufferObject(
          o,                      // Supply the vertex array
          glm::vec3(),            // World position
          glm::vec3(0, 1, 0),     // Rotation axis
          glm::radians(0.0f),     // Rotation angle
          glm::vec3(1, 0, 0),     // Pre-rotation axis
          glm::radians(90.0f),    // Pre-rotation angle
          glm::vec3(1),
          "../textures/mars.jpg");          // Scale vector

  glm::vec2 windowSize;
  content->getWindowSize(windowSize);

  context.setColorStyle({glw::WHITE_A, glw::BLACK_A, glw::SKY_BLUE_A});

  context.addComponent(glw::gui::createLabel("fucking yeeeeeeeeeet",  windowSize / 2.0f, 100.0f, glm::vec4(1.0f,0.0,0.0f,0.5f), true));

  fpsLabel = glw::gui::createLabel("fps", glm::vec2(), 20, glw::BLACK_A);
  context.addComponent(fpsLabel);

  GButton * button = new GButton(glm::vec2(100), glm::vec2(500, 100), "yeet me");
  LINK(TRIGGER(button, &GButton::onPressed), ACTION(onButtonPress));
  LINK(TRIGGER(button, &GButton::onToggledOn), ACTION(onToggledOn));
  context.addComponent(button);

  GWindow * window = new GWindow(glm::vec2(125,250), glm::vec2(300,300), "yoteth");
  context.addComponent(window);

  window->addChildComponent(new GTextEdit(glm::vec2(300), glm::vec2(100,100),"this is some text", 20, glw::WHITE_A));

  window->addChildComponent(new GSlider(glm::vec2(150), glm::vec2(300,10), 0.25f, 0.5f));

  window->addChildComponent(new GSlider(glm::vec2(150, 200), glm::vec2(25,200), 0.25f, 0.05f, true));

  window->addChildComponent(new GSpinner(glm::vec2(200), glm::vec2(100, 25)));

  GDropdown<std::string> * dropdown = new GDropdown<std::string>(glm::vec2(400), glm::vec2(100, 25));
  window->addChildComponent(dropdown);

  window->addChildComponent(new GImageView(glm::vec2(0, 300), glm::vec2(100), "../textures/151.bmp"));

  progressbar = new GProgressBar(glm::vec2(400, 500), glm::vec2(100, 20), 0,100, true);
  window->addChildComponent(progressbar);

  window->addChildComponent(new GCheckBox(glm::vec2(550, 400), glm::vec2(30)));

  context.setContent(content);
  context.init();

  std::string str("");
  dropdown->add(str, "one");
  dropdown->add(str, "two");
  dropdown->add(str, "three");
  dropdown->setSelectedId(2);

  context.validate();

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
  context.checkMouseEvents(button, action);

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
  context.checkKeyEvents(key, action);

  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
      LINFO(TRG, "User triggered terminatation.");
      content->exit();
      break;
    }
  }
}

int main()
{
  LSTARTLOGGER("../logs/GLW");

  LINFO(TRG, "Program started.");

  // Get instance pointer
  content = GContent::getInstancePtr();

  // Set the clear colour of the scene
  content->setClearColor(glw::GREY_A / 2.0f);
  // Set the window size
  content->setWindowSize(glm::vec2(1280,720));
  // Set the callbacks for the engine, and run
  content->run(loop, init, key_callback, mouse_button_callback);

  LINFO(TRG, "Program exit.");

  LENDLOGGER();

  return 0;
}
