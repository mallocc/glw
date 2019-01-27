#pragma once

#include "Types.h"
#include "ImageLoader.h"

namespace gfx
{
	class TypeFactory
	{
	public:
		static Light_T * createLight_T(glm::vec3 pos, glm::vec3 color, glm::vec3 brightness_specscale_shinniness, Light_T * light_t)
		{
			if (light_t == nullptr)
				light_t = new Light_T();
			light_t->pos = pos;
			light_t->color = color;
			light_t->brightness_specscale_shinniness = brightness_specscale_shinniness;
			return light_t;
		}

		static Vertex_T * createVertex_T(glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec3 tangent, glm::vec2 uv, Vertex_T * vertex_t)
		{
			if (vertex_t == nullptr)
				vertex_t = new Vertex_T();
			vertex_t->position = position;
			vertex_t->color = color;
			vertex_t->normal = normal;
			vertex_t->tangent = tangent;
			vertex_t->uv = uv;
			return vertex_t;
		}

		static MatInfo_T * createMatInfo_T(glm::vec3 colour, GLuint textId, const char* name, const char* fileName, MatInfo_T * matInfo_t)
		{
			if (matInfo_t == nullptr)
				matInfo_t = new MatInfo_T();
			matInfo_t->colour = colour;
			matInfo_t->textId = textId;
			matInfo_t->name = name;
			matInfo_t->fileName = fileName;
			return matInfo_t;
		}

		static MVPHandle_T * createMVPHandle_T(engine::VarHandle * model, engine::VarHandle * view, engine::VarHandle * projection, MVPHandle_T * mvpHandle_t)
		{
			if (mvpHandle_t == nullptr)
				mvpHandle_t = new MVPHandle_T();
			mvpHandle_t->model = model;
			mvpHandle_t->view = view;
			mvpHandle_t->projection = projection;
			return mvpHandle_t;
		}
	};
}
