#pragma once

#include <vector>
#include <set>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "mesh.h"
#include "ImageLoader.h"
#include "TypeFactory.h"

namespace gfx
{
	namespace engine
	{
		class TexturedMesh : Mesh
		{
		public:
			virtual void draw(int wire_frame, VarHandle *model, VarHandle *texture_handle);

			void add_mesh(Mesh mesh);

			TexturedMesh();

			TexturedMesh(
				const char * obj_filename,
				const char * base_filename,
				glm::vec3 _pos,
				glm::vec3 _rotation,
				GLfloat _theta,
				glm::vec3 _pre_rotation,
				GLfloat _pre_theta,
				glm::vec3 _scale);

		private:
			std::vector<Mesh> m_meshes;

		public:
			glm::vec3 m_pos;

		};
	}
}
