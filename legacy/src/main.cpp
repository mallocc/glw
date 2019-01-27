
#include <stdio.h>
#include <iostream>
#include <vector>

#include "GLContent.h"
#include "GLCamera.h"
#include "GLSLProgram.h"
#include "GLSLProgramManager.h"
#include "colors.h"
#include "FBO.h"
#include "FBOManager.h"
#include "Mesh.h"
#include "PrimativeGenerator.h"

#include "CLog.h"

#include "StringFormat.h"

#include "GUIManager.h"

gfx::engine::GLSLProgramManager program_manager;
gfx::engine::GLContent content;
gfx::engine::GLCamera camera = gfx::engine::GLCamera(glm::vec3(0, 0, 0), glm::vec3(), glm::vec3(0,0,-1), glm::vec3(0, 1, 0));

gfx::gui::GFXManager gfxManager;

gfx::engine::Mesh
screen_texture,
sphere;

gfx::engine::GLSLProgramID
RENDER_PROGRAM,
GUI_PROGRAM;

glm::vec3 ambient_color;

float alpha = 0.5f;

gfx::gui::GFXMesh mesh, mesh2;
gfx::gui::GFXContainer container;

//Returns random float
inline float		randf()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void printMousePos()
{
	glm::vec2 pos = content.getMousePos();
	CINFO(alib::StringFormat("x = %0 y = %1").arg(pos.x).arg(pos.y).str());
}

void onClosedWindow()
{
	CINFO("Window has closed.");
}

void init()
{
	//// CREATE GLSL PROGAMS
	CINFO("Initialising GLSL programs...");
	RENDER_PROGRAM =
		program_manager.addProgram("shaders/basic_texture.vert", "shaders/basic_texture.frag",
			content.getModelMat(), content.getViewMat(), content.getProjMat());
	GUI_PROGRAM =
		program_manager.addProgram("shaders/basic_gui.vert", "shaders/basic_gui.frag",
			content.getModelMat(), content.getViewMat(), content.getProjMat());

	//// ADDING HANDLES TO PROGRAMS
	CINFO("Adding handles to GLSL programs...");
	program_manager.getProgram(RENDER_PROGRAM)
		->setTexHandle();
	program_manager.getProgram(GUI_PROGRAM)
		->setColorHandle()
		->setFlagHandle()
		->setTexHandle();

	//// CREATE OBJECTS
	CINFO("Initialising objects...");

	std::vector<glm::vec3> v;
	v = gfx::PrimativeGenerator::generate_cube();
	sphere = gfx::engine::Mesh(
		"",
		gfx::PrimativeGenerator::pack_object(&v, GEN_COLOR_RAND, gfx::WHITE),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0), glm::radians(0.0f),
		glm::vec3(1, 0, 0), glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);

	gfx::gui::GFXColorStyle_T * colorStyle = new gfx::gui::GFXColorStyle_T;
	colorStyle->colors[0] = gfx::ORANGE_A;
	colorStyle->colors[1] = gfx::OFF_WHITE_A;
	colorStyle->colors[2] = gfx::OFF_BLACK_A;
	gfxManager.setColorStyle(colorStyle);

	gfx::gui::GFXWindow * window = new gfx::gui::GFXWindow(glm::vec2(50, 50), glm::vec2(300, 300));
	window->link(&gfx::gui::GFXWindow::onClose, gfx::gui::ACTION(onClosedWindow));
	gfxManager.addComponent(window);
	gfx::gui::GFXButton * button = new gfx::gui::GFXButton(glm::vec2(25, 50), glm::vec2(300, 50), "Press Me");
	window->addComponent(button);
	gfx::gui::GFXSpinner * spinner = new gfx::gui::GFXSpinner(glm::vec2(25, 150), glm::vec2(100, 25), 0, 0.33f);
	window->addComponent(spinner);

	window = new gfx::gui::GFXWindow(glm::vec2(50, 50), glm::vec2(300, 300));
	window->link(&gfx::gui::GFXWindow::onClose, gfx::gui::ACTION(onClosedWindow));
	gfxManager.addComponent(window);
	spinner = new gfx::gui::GFXSpinner(glm::vec2(25, 50), glm::vec2(100, 25), 0, 0.33f);
	window->addComponent(spinner);
	spinner = new gfx::gui::GFXSpinner(glm::vec2(100, 150), glm::vec2(100, 50), 0, 0.33f);
	window->addComponent(spinner);
	spinner = new gfx::gui::GFXSpinner(glm::vec2(200, 250), glm::vec2(200, 100), 0, 0.33f);
	window->addComponent(spinner);

	window = new gfx::gui::GFXWindow(glm::vec2(50, 50), glm::vec2(300, 300));
	window->link(&gfx::gui::GFXWindow::onClose, gfx::gui::ACTION(onClosedWindow));
	gfxManager.addComponent(window);
	gfx::gui::GFXTextEdit * textEdit = new gfx::gui::GFXTextEdit("hello world!", GFX_GUI_DEFAULT_FONT_SIZE, glm::vec2(25, 150), glm::vec2(300,100));
	textEdit->setColor(gfx::RED_A);
	textEdit->setMultiline(true);
	window->addComponent(textEdit);
	gfx::gui::GFXScrollBar * vScrollBar = new gfx::gui::GFXScrollBar(glm::vec2(200, 25), glm::vec2(25, 200), 0,0.2f, true);
	window->addComponent(vScrollBar);

	gfxManager.init();
	gfxManager.validate();
}

void physics()
{
	sphere.m_theta += 0.001f;
}

void draw_loop()
{
	content.setCamera(&camera);

	physics();

	gfxManager.checkEvents(&content);
	gfxManager.update(&content);

	gfx::engine::FBO::unbind();

	gfx::engine::MeshHandle_T meshHandle;

	content.clearAll();
	content.loadPseudoIsometric();
	program_manager.loadProgram(RENDER_PROGRAM);
	meshHandle = program_manager.getCurrentProgram()->getMeshHandle();
	sphere.draw(0, meshHandle);

	content.clearDepthBuffer();
	content.loadExternalOrtho();
	program_manager.loadProgram(GUI_PROGRAM);
	meshHandle = program_manager.getCurrentProgram()->getMeshHandle();
	gfxManager.draw(glm::mat4(1.0f), meshHandle);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			printMousePos();
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
		case GLFW_KEY_B:
			break;
		case GLFW_KEY_G:
			break;

		case GLFW_KEY_UP:
			content.setIsometricDepth(content.getIsometricDepth() + 0.25f);
			break;
		case GLFW_KEY_DOWN:
			content.setIsometricDepth(content.getIsometricDepth() - 0.25f);
			break;

		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_Q:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}
}

int main()
{
	content.setClearColor(gfx::GREY);
	content.run(draw_loop, init, key_callback, mouse_button_callback);
	return 0;
}
