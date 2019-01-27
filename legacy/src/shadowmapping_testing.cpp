#include <stdio.h>

#include "glcontent.h"

#include "glslprogram.h"
#include "colors.h"
#include "fbo.h"
#include "light.h"
#include "mesh.h"
#include "primitive_generators.h"
#include "save_image.h"

#include "shadow_map.h"

#include "lerp.h"

#include <vector>;

GLSLProgramManager program_manager;

FBOManager fbo_manager;

GLContent content;

Camera camera = Camera(glm::vec3(0, 0, -3), glm::vec3(), glm::vec3(0,0,1), glm::vec3(0, 1, 0));

Mesh
screen_texture,
sphere,
ground
;

FBOID
basic_fbo,
final_fbo;

GLSLProgramID
RENDER_PROGRAM,
RENDER_ALPHA_PROGRAM,
PHONG_TEXTURE_PROGRAM,
SHADOWMAP_PROGRAM;

Light light = { glm::vec3(0,5,-5), WHITE, glm::vec3(1,1,100) };

ShadowMapper shadow_map1;

glm::vec3 ambient_color;

float alpha = 0.9f;

bool GLOW_ON = 1;
bool BLUR_ON = 10;

BezierLerper lerp(glm::vec3(), glm::vec3(0,0,2), glm::vec3(2,0,0), glm::vec3(2,0,2), 0.1f, 0);

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
	PHONG_TEXTURE_PROGRAM =
		program_manager.add_program("shaders/phong_texture.vert", "shaders/phong_texture.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());
	SHADOWMAP_PROGRAM =
		program_manager.add_program("shaders/shadowmap.vert", "shaders/shadowmap.frag",
			content.get_model_mat(), content.get_view_mat(), content.get_proj_mat());
	//// ADDING HANDLES TO PROGRAMS
	printf("\n");
	printf("Adding handles to GLSL programs...\n");
	program_manager.get_program(RENDER_PROGRAM)
		->set_tex_handle();
	program_manager.get_program(RENDER_ALPHA_PROGRAM)
		->set_tex_handle()
		->add_handle(VarHandle("u_alpha", &alpha));
	program_manager.get_program(PHONG_TEXTURE_PROGRAM)
		->set_tex_handle()
		->add_handle(VarHandle("u_ambient_color", &ambient_color))
		->add_handle(VarHandle("u_light_color", &light.color))
		->add_handle(VarHandle("u_eye_pos", content.get_eye_pos()))
		->add_handle(VarHandle("u_light_pos", &light.pos))
		->add_handle(VarHandle("u_light_properties", &light.brightness_specscale_shinniness));
	

	//// CREATE FBOS
	printf("\n");
	printf("Creating FBOs...\n");
	basic_fbo           = fbo_manager.add_fbo(content.get_window_size(), &screen_texture);
	final_fbo           = fbo_manager.add_fbo(content.get_window_size(), &screen_texture);
	
	program_manager.get_program(SHADOWMAP_PROGRAM)
		->add_handle(VarHandle("u_d_v", shadow_map1.get_view_mat4()))
		->add_handle(VarHandle("u_d_p", shadow_map1.get_proj_mat4()))
		->add_handle(VarHandle("u_final_depth", fbo_manager.get_fbo(basic_fbo)->get_depth()))
		->add_handle(VarHandle("u_shadow_depth", shadow_map1.get_depth()))
		->add_handle(VarHandle("u_shadow_color", shadow_map1.get_color()));

	//// ADD OBJECTS TO FBOS
	fbo_manager.get_fbo(basic_fbo)
		->add_mesh(&sphere)
		->add_mesh(&ground);

	//// CREATE SHADOWMAPPERS
	printf("\n");
	printf("Creating Shadow Mappers...\n");
	shadow_map1
		 .set_fbo(FBO(content.get_window_size()))
		->set_light_source(light)
		->set_mvp(glm::vec3(), *content.get_up());
	shadow_map1
		 .add_mesh(&sphere)
		->add_mesh(&ground);
	shadow_map1.fbo.set_render_mesh(&screen_texture);


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
	int res = 200;
	v = generate_sphere(res, res);
	sphere = Mesh(
		"",
		pack_object(&v, GEN_DEFAULT, WHITE),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0), glm::radians(0.0f),
		glm::vec3(1, 0, 0), glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);

	v = generate_centered_square_mesh(res, res);
	ground = Mesh(
		"",
		pack_object(&v, GEN_DEFAULT, WHITE),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 1), glm::radians(-90.0f),
		glm::vec3(1, 0, 0), glm::radians(0.0f),
		glm::vec3(1, 1, 1) * 2.0f
	);

}

void physics()
{
	sphere.pos = lerp.lerpStepSmooth(0.001f);
}

void draw_loop()
{
	content.set_camera(&camera);

	physics();

	FBO::unbind();

	VarHandle
		*model_mat_handle,
		*texture_handle,
		*view_mat_handle,
		*proj_mat_handle;

	content.clearAll();
	content.loadPerspective();
	program_manager.load_program(PHONG_TEXTURE_PROGRAM);
	model_mat_handle = program_manager.get_current_program()->get_model_mat4_handle();
	view_mat_handle = program_manager.get_current_program()->get_view_mat4_handle();
	proj_mat_handle = program_manager.get_current_program()->get_proj_mat4_handle();
	texture_handle = program_manager.get_current_program()->get_tex_handle();	
	shadow_map1.load_mats(view_mat_handle, proj_mat_handle);
	shadow_map1.render_shadowmap(model_mat_handle, texture_handle);

	content.clearAll();
	content.loadPerspective();
	program_manager.load_program(RENDER_PROGRAM);
	model_mat_handle = program_manager.get_current_program()->get_model_mat4_handle();
	texture_handle = program_manager.get_current_program()->get_tex_handle();
	fbo_manager.get_fbo(basic_fbo)->binding_draw_meshes(model_mat_handle, texture_handle);

	content.clearAll();
	content.loadOrtho();
	program_manager.load_program(RENDER_PROGRAM);
	model_mat_handle = program_manager.get_current_program()->get_model_mat4_handle();
	texture_handle = program_manager.get_current_program()->get_tex_handle();
	screen_texture.set_tex(*shadow_map1.get_color());
	screen_texture.draw(0,model_mat_handle, texture_handle);


	//shadow_map1.fbo.draw_render_mesh(model_mat_handle, texture_handle);
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
	content.set_clear_color(1.0f-GREEN);
	content.set_eye_pos(glm::vec3(0,0,-3));
	content.run(draw_loop, init, key_callback);

	return 0;
}
