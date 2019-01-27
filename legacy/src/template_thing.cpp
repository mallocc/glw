#include <stdio.h>

#include "glcontent.h"

#include "glslprogram.h"
#include "colors.h"
#include "fbo.h"
#include "light.h"
#include "mesh.h"
#include "primitive_generators.h"
#include "save_image.h"

#include <vector>;

GLSLProgramManager program_manager;

FBOManager fbo_manager;

GLContent content;

Camera camera = Camera(glm::vec3(0, 0, -3), glm::vec3(), glm::vec3(0,0,3), glm::vec3(0, 1, 0));

Mesh
screen_texture,
image,
block[5],
background;

FBOID
basic_fbo;

GLSLProgramID
RENDER_PROGRAM,
RENDER_ALPHA_PROGRAM;

Light light = { glm::vec3(0,5,-5), WHITE, glm::vec3(1,1,100) };

glm::vec3 ambient_color;

float alpha = 0.9f;

bool GLOW_ON = 1;
bool BLUR_ON = 10;

//Returns random float
inline float		randf()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void init()
{
	//// CREATE GLSL PROGAMS
	printf("\n");
	printf("Initialising GLSL programs...\n");
	RENDER_PROGRAM        =
		program_manager.add_program("shaders/basic_texture.vert", "shaders/basic_texture.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());
	RENDER_ALPHA_PROGRAM =
		program_manager.add_program("shaders/basic_texture.vert", "shaders/basic_texture_alpha.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());

	//// ADDING HANDLES TO PROGRAMS
	printf("\n");
	printf("Adding handles to GLSL programs...\n");
	program_manager.get_program(RENDER_PROGRAM)
		->set_tex_handle();
	program_manager.get_program(RENDER_ALPHA_PROGRAM)
		->set_tex_handle()
		->add_handle(VarHandle("u_alpha", &alpha));

	//// CREATE FBOS
	printf("\n");
	printf("Creating FBOs...\n");
	basic_fbo           = fbo_manager.add_fbo(content.get_window_size(), &screen_texture);
	
	//// ADD OBJECTS TO FBOS
	fbo_manager.get_fbo(basic_fbo);
		//->add_mesh(&bunny);

	//// CREATE OBJECTS
	printf("\n");
	printf("Initialising objects...\n");

	std::vector<glm::vec3> v;

	v = generate_square_mesh(1, 1);
	screen_texture = Mesh(
		"",
		pack_object(&v, GEN_UVS_RECTS, BLACK),
		glm::vec3(),
		glm::vec3(0, 0, 1),	glm::radians(0.0f),
		glm::vec3(0, 0, 1),	glm::radians(0.0f),
		glm::vec3(content.get_window_size().x, content.get_window_size().y, 1)
	);

	background = Mesh(
		"",
		pack_object(&v, GEN_COLOR, WHITE),
		glm::vec3(),
		glm::vec3(0, 0, 1), glm::radians(0.0f),
		glm::vec3(0, 0, 1), glm::radians(0.0f),
		glm::vec3(content.get_window_size().x, content.get_window_size().y, 1)
	);

	image = Mesh(
		"textures/name.png",
		pack_object(&v, GEN_UVS_RECTS, BLACK),
		glm::vec3(100,100,0),
		glm::vec3(0, 1, 0), glm::radians(0.0f),
		glm::vec3(0, 0, 1), glm::radians(0.0f),
		glm::vec3(content.get_window_size().x, content.get_window_size().y, 1)
	);

	float top = 1.0f;
	float bottom = 0.0f;
	float height = (top - bottom) / 5.0f;
	int i = 0; 
	float offset = bottom;
	for (; offset < top; offset+=height, i++)
	{
		block[i] = Mesh(
			"",
			pack_object(&v, GEN_COLOR, WHITE * randf()),
			glm::vec3(0.5f, offset ,0),
			glm::vec3(0, 0, 1), glm::radians(0.0f),
			glm::vec3(0, 0, 1), glm::radians(0.0f),
			glm::vec3(content.get_window_size().x, content.get_window_size().y, 1) * glm::vec3(0.5f, height,0)
		);
	}
}

void physics()
{
}

void draw_loop()
{
	content.set_camera(&camera);

	physics();

	FBO::unbind();

	VarHandle
		*model_mat_handle,
		*texture_handle;

	content.clearAll();
	content.loadOrtho();
	program_manager.load_program(RENDER_PROGRAM);
	model_mat_handle = program_manager.get_current_program()->get_model_mat4_handle();
	texture_handle = program_manager.get_current_program()->get_tex_handle();
	//fbo_manager.get_fbo(basic_fbo)->bind();
	image.draw(0, model_mat_handle, texture_handle);
	for(int i = 0; i < 5; ++i)
		block[i].draw(0, model_mat_handle, texture_handle);
	//fbo_manager.get_fbo(basic_fbo)->unbind();

	//content.clearAll();
	//content.loadOrtho();
	program_manager.load_program(RENDER_PROGRAM);
	model_mat_handle = program_manager.get_current_program()->get_model_mat4_handle();
	texture_handle = program_manager.get_current_program()->get_tex_handle();
	//fbo_manager.get_fbo(basic_fbo)->draw_render_mesh(model_mat_handle, texture_handle);
}

unsigned char * image_data_pixels = new unsigned char[content.get_window_size().x * content.get_window_size().y * 3];

static void	key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == 2)
	{
		switch (key)
		{
		case GLFW_KEY_ENTER:
			glReadPixels(0, 0, content.get_window_size().x, content.get_window_size().y, GL_RGB, GL_UNSIGNED_BYTE, image_data_pixels);
			createBMP(image_data_pixels, content.get_window_size().x, content.get_window_size().y, 3, "test.bmp");
		break;

		case GLFW_KEY_B:
			BLUR_ON = !BLUR_ON;
			break;
		case GLFW_KEY_G:
			GLOW_ON = !GLOW_ON;
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
	content.set_clear_color(GREEN);
	content.set_eye_pos(glm::vec3(0,0,-3));
	content.run(draw_loop, init, key_callback);

	return 0;
}
