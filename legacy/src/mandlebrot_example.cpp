#include <stdio.h>

#include "glcontent.h"

#include "glslprogram.h"
#include "colors.h"
#include "fbo.h"
#include "light.h"
#include "mesh.h"
#include "primitive_generators.h"

#include <vector>;

GLSLProgramManager program_manager;

FBOManager fbo_manager;

GLContent content;

Camera camera = Camera(glm::vec3(0, 0, -3), glm::vec3(), glm::vec3(0,0,3), glm::vec3(0, 1, 0));

Mesh
screen_texture,
sphere, stars, bunny;

FBOID
basic_fbo,
blur_fbo,
new_frame_fbo,
old_frame_fbo;

GLSLProgramID 
BASIC_PROGRAM, 
PHONG_PROGRAM,
PHONG_TEXTURE_PROGRAM,
RENDER_PROGRAM,
RENDER_ALPHA_PROGRAM,
BLUR_PROGRAM,
MIX_PROGRAM,
MANDLE_PROGRAM;

Light light = { glm::vec3(0,5,-5), WHITE, glm::vec3(1,1,100) };

glm::vec3 blur_properties, motionblur_properties, ambient_color, mandle_properties, mandle_target;

float motionblur_alpha = 0.9f;
float motionblur_scale = 1.0f;

bool GLOW_ON = 1;
bool BLUR_ON = 10;

//Returns random float
inline float		randf()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void init()
{
	blur_properties = glm::vec3(3 / content.get_window_size().x, 3 / content.get_window_size().y, 15);
	motionblur_properties = glm::vec3(1.0f, 0.9, 1.0f);
	mandle_properties = glm::vec3(100.0f, 0.25f, 0.0f);

	//// CREATE GLSL PROGAMS
	printf("\n");
	printf("Initialising GLSL programs...\n");
	BASIC_PROGRAM         =
		program_manager.add_program("shaders/basic.vert", "shaders/basic.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());
	PHONG_PROGRAM         =
		program_manager.add_program("shaders/phong.vert", "shaders/phong.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());
	RENDER_PROGRAM        =
		program_manager.add_program("shaders/basic_texture.vert", "shaders/basic_texture.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());
	RENDER_ALPHA_PROGRAM =
		program_manager.add_program("shaders/basic_texture.vert", "shaders/basic_texture_alpha.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());
	BLUR_PROGRAM          =
		program_manager.add_program("shaders/basic_texture.vert", "shaders/basic_texture_blur.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());
	MIX_PROGRAM           =
		program_manager.add_program("shaders/basic_texture.vert", "shaders/combine.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());
	PHONG_TEXTURE_PROGRAM =
		program_manager.add_program("shaders/phong_texture.vert", "shaders/phong_texture.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());	
	MANDLE_PROGRAM =
		program_manager.add_program("shaders/mandle.vert", "shaders/mandle.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());

	//// ADDING HANDLES TO PROGRAMS
	printf("\n");
	printf("Adding handles to GLSL programs...\n");
	program_manager.get_program(PHONG_PROGRAM)
		->add_handle(VarHandle("u_ambient_color", &ambient_color))
		->add_handle(VarHandle("u_light_color", &light.color))
		->add_handle(VarHandle("u_eye_pos", content.get_eye_pos()))
		->add_handle(VarHandle("u_light_pos", &light.pos))
		->add_handle(VarHandle("u_light_properties", &light.brightness_specscale_shinniness));
	program_manager.get_program(PHONG_TEXTURE_PROGRAM)
		->set_tex_handle()
		->add_handle(VarHandle("u_ambient_color", &ambient_color))
		->add_handle(VarHandle("u_light_color", &light.color))
		->add_handle(VarHandle("u_eye_pos", content.get_eye_pos()))
		->add_handle(VarHandle("u_light_pos", &light.pos))
		->add_handle(VarHandle("u_light_properties", &light.brightness_specscale_shinniness));
	program_manager.get_program(RENDER_PROGRAM)
		->set_tex_handle();
	program_manager.get_program(RENDER_ALPHA_PROGRAM)
		->set_tex_handle()
		->add_handle(VarHandle("u_alpha", &motionblur_alpha));
	program_manager.get_program(BLUR_PROGRAM)
		->set_tex_handle()
		->add_handle(VarHandle("u_glow_res", &blur_properties));
	program_manager.get_program(MIX_PROGRAM)
		->set_tex_handle()
		->set_tex1_handle()
		->add_handle(VarHandle("u_mix_properties", &motionblur_properties));
	program_manager.get_program(MANDLE_PROGRAM)
		->set_tex_handle()
		->add_handle(VarHandle("u_mandle_properties", &mandle_properties))
		->add_handle(VarHandle("u_target", &mandle_target));

	//// CREATE FBOS
	printf("\n");
	printf("Creating FBOs...\n");
	basic_fbo           = fbo_manager.add_fbo(content.get_window_size(), &screen_texture);
	blur_fbo            = fbo_manager.add_fbo(content.get_window_size(), &screen_texture);
	new_frame_fbo       = fbo_manager.add_fbo(content.get_window_size(), &screen_texture);
	old_frame_fbo       = fbo_manager.add_fbo(content.get_window_size(), &screen_texture);
	
	//// ADD OBJECTS TO FBOS
	fbo_manager.get_fbo(basic_fbo)
		->add_mesh(&bunny);
	fbo_manager.get_fbo(blur_fbo)
		->add_mesh(&bunny)
		->add_mesh(&stars);
	
	//// CREATE OBJECTS
	printf("\n");
	printf("Initialising objects...\n");

	std::vector<glm::vec3> v;

	v = generate_square_mesh(1, 1);
	screen_texture = Mesh(
		"",
		pack_object(&v, GEN_UVS_RECTS, BLACK),
		glm::vec3(),
		glm::vec3(0, 0, 1),	glm::radians(90.0f),
		glm::vec3(1, 0, 0),	glm::radians(0.0f),
		glm::vec3(1, 1, 1)
	);

	int res = 200;
	v = generate_sphere(res, res);
	sphere = Mesh(
		"textures/mars.jpg",
		pack_object(&v, GEN_UVS_SPHERE | GEN_NORMS, WHITE),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0), glm::radians(0.0f),
		glm::vec3(1, 0, 0), glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);

	v = generate_sphere_invert(20, 20);
	stars = Mesh(
		"textures/back.jpg",
		pack_object(&v, GEN_UVS_SPHERE | GEN_NORMS, BLACK),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 0, 0),	glm::radians(0.0f),
		glm::vec3(1, 0, 0),	glm::radians(90.0f),
		glm::vec3(1, 1, 1) * 50.0f
	);

	v = load_model("objects/bunny.obj");
	bunny = Mesh(		
		"textures/197.bmp",
		pack_object(&v, GEN_ALL, WHITE),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 0, 0), glm::radians(0.0f),
		glm::vec3(1, 0, 0), glm::radians(0.0f),
		glm::vec3(1, 1, 1)
	);
}

void physics()
{
	//light.pos = glm::quat(glm::vec3(0, glm::radians(1.0f), 0)) * light.pos;
	bunny.theta += glm::radians(0.2f);
}

void draw_loop()
{
	content.set_camera(&camera);

	physics();

	FBO::unbind();

	VarHandle 
		*model_mat_handle,
		*texture_handle,
		*texture1_handle;

	content.clearAll();
	content.loadOrtho();
	program_manager.load_program(MANDLE_PROGRAM);
	model_mat_handle = program_manager.get_current_program()->get_model_mat4_handle();
	texture_handle = program_manager.get_current_program()->get_tex_handle();
	screen_texture.set_tex(GL_TEXTURE0);
	screen_texture.draw(0, model_mat_handle, nullptr);
}

static void	key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == 2)
	{
		switch (key)
		{
		case GLFW_KEY_UP:
			mandle_target.y -= 0.01f / mandle_properties.y;
			break;
		case GLFW_KEY_DOWN:
			mandle_target.y += 0.01f / mandle_properties.y;
			break;
		case GLFW_KEY_RIGHT:
			mandle_target.x += 0.01f / mandle_properties.y;
			break;
		case GLFW_KEY_LEFT:
			mandle_target.x -= 0.01f / mandle_properties.y;
			break;

		case GLFW_KEY_O:
			mandle_properties.y *= 1.1f;
			break;
		case GLFW_KEY_P:
			mandle_properties.y /= 1.1f;
			break;

		case GLFW_KEY_K:
			mandle_properties.x += 10.0f;
			break;
		case GLFW_KEY_L:
			mandle_properties.x -= 10.0f;
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
