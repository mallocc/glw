#pragma once

#include "glm.h"
#include "opengl.h"
#include "VarHandle.h"

namespace gfx
{
	struct Light_T
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec3 brightness_specscale_shinniness;
	};

	struct MatInfo_T
	{
	public:
		glm::vec3 colour;
		GLuint textId; // HAS TO BE THE SAME AS TEXTURE_ID, the same thing that you pass in to partition vertices
		const char* name;
		const char* fileName;
	};

	struct MVPHandle_T
	{
		engine::VarHandle * model;
		engine::VarHandle * view;
		engine::VarHandle * projection;
	};

	struct Vertex_T
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec2 uv;
	};
}